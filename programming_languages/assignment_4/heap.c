#include <stddef.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdint.h>

#define HEAP_COOKIE 0xdeadbeefcafebabe
#define ALLOC_HAS_NEXT 2
#define ALLOC_IS_FREE 1

struct heap {
    size_t available;
};

struct alloc_hdr {
    size_t chunk_size;               // Size of entire allocation
    size_t alloc_size;               // User requested size
};

extern struct heap *heap_arena;

struct heap *heap_arena = NULL;

void heap_arena_init()
{
    size_t heap_size = 1 << 26;

    heap_arena = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    heap_arena->available = heap_size - sizeof(struct heap);
}

bool heap_check_range(void *start, void *end)
{
    return start > (void *)heap_arena && end < ((void *)heap_arena) + heap_arena->available;
}

size_t heap_apply_flags(size_t num, size_t flags)
{
    return (num & (size_t) ~7) | flags;
}

struct alloc_hdr *heap_ptr_offset(struct alloc_hdr *ptr, size_t size)
{
    return ((void *)ptr) + size;
}

void *allocate(size_t request, size_t *info)
{
    struct alloc_hdr *cur_hdr, *prev_hdr, *new_hdr;
    bool alloc_freed, has_next;
    size_t chunk_size, actual_request, padding;
    void *new_ptr;

    if (heap_arena == NULL) heap_arena_init();

    // Add in sizeof alloc_hdr, heap cookie and pad request to align to 8 bytes
    actual_request = request;

    padding = 8 - (request % 8);
    request += sizeof(struct alloc_hdr) + 8 + (padding == 8 ? 0 : padding);
    // Go through list of already allocated allocations
    // If free allocation is >= request + sizeof(alloc_hdr)
    // -> split free allocation, subtracting from chunk_size
    // If no free allocation is big enough, create a new allocation

    cur_hdr = (struct alloc_hdr *)heap_arena + sizeof(struct heap);
    prev_hdr = NULL;
    while (true) {
        if (!heap_check_range(cur_hdr, cur_hdr + sizeof(struct alloc_hdr))) {
            *info = 0;
            return NULL;
        }

        has_next = cur_hdr->chunk_size & ALLOC_HAS_NEXT;
        if (0 == has_next) break;

        alloc_freed = cur_hdr->chunk_size & ALLOC_IS_FREE;
        // Get actual alloc size alligned to 8 bytes
        chunk_size = heap_apply_flags(cur_hdr->chunk_size, 0);
        if (0 != alloc_freed && (chunk_size - sizeof(struct alloc_hdr)) >= request) {
            new_hdr = heap_ptr_offset(cur_hdr, chunk_size - request);
            new_hdr->chunk_size = heap_apply_flags(request, ALLOC_HAS_NEXT);
            new_hdr->alloc_size = actual_request;

            new_ptr = heap_ptr_offset(new_hdr, sizeof(struct alloc_hdr));
            *((uint64_t *) (((void *)new_ptr) + actual_request)) = HEAP_COOKIE;

            chunk_size -= request;
            cur_hdr->chunk_size = heap_apply_flags(chunk_size, ALLOC_HAS_NEXT | ALLOC_IS_FREE);

            *info = request - sizeof(struct alloc_hdr);
            return new_ptr;
        }
        prev_hdr = cur_hdr;
        cur_hdr = heap_ptr_offset(cur_hdr, chunk_size);
    }

    // TODO Check if there is enough space to do this
    // No success finding it in freelist, need to make new ptr
    chunk_size = heap_apply_flags(cur_hdr->chunk_size, 0);

    if (prev_hdr == NULL) {
        new_hdr = heap_ptr_offset(cur_hdr, 0);
    } else {
        new_hdr = heap_ptr_offset(cur_hdr, chunk_size);
        prev_hdr->chunk_size = heap_apply_flags(prev_hdr->chunk_size, ALLOC_HAS_NEXT);
    }

    if (!heap_check_range(new_hdr, heap_ptr_offset(new_hdr, request))) {
        *info = 0;
        return NULL;
    }

    new_hdr->chunk_size = heap_apply_flags(request, ALLOC_HAS_NEXT);
    new_hdr->alloc_size = actual_request;

    new_ptr = heap_ptr_offset(new_hdr, sizeof(struct alloc_hdr));
    *((uint64_t *) (new_ptr + actual_request)) = HEAP_COOKIE;

    *info = request - sizeof(struct alloc_hdr);
    return new_ptr;
}

int deallocate(void *ptr)
{
    // If valid ptr, set last freelist ptr to ptr
    struct alloc_hdr *cur_hdr, *prev_hdr, *next_hdr;
    bool alloc_freed, has_next, next_free, prev_free;
    size_t chunk_size, actual_request, padding;
    uint64_t *cookie_check;

    if (heap_arena == NULL) return -1;
    // Look before and after ptr to see if you can combine allocations
    /*
    If the pointer I give you does not point into your heap, return the value -1.
    If the pointer I give you has explicitly been freed before (and not reallocated), return the value -2.
    If the pointer I give you points into your heap, but is not a pointer that you returned from a call to
    allocate(), then return -3.
    If the pointer I give you is valid, but in the course of using it, I did an out-of-bounds write, then
    return -4. In this case, you should still return the pointer to the free list
    */
    if (!heap_check_range(ptr, ptr)) return -1;

    cur_hdr = (struct alloc_hdr *)heap_arena + sizeof(struct heap);
    prev_hdr = NULL;
    next_hdr = NULL;
    while (true) {
        if (!heap_check_range(cur_hdr, cur_hdr + sizeof(struct alloc_hdr))) {
            return -1;
        }

        has_next = cur_hdr->chunk_size & ALLOC_HAS_NEXT;
        if (0 == has_next) break;

        alloc_freed = cur_hdr->chunk_size & ALLOC_IS_FREE;
        // Get actual alloc size alligned to 8 bytes
        chunk_size = heap_apply_flags(cur_hdr->chunk_size, 0);

        if (heap_ptr_offset(cur_hdr, sizeof(struct alloc_hdr)) == ptr) {
            // Found match
            if (0 != alloc_freed) {
                // Already freed
                return -2;
            } else {
                // Hasn't been freed, good to go
                // Check before and after to see if things can get combined
                // ...if not, check to see if cookie got modified
                cookie_check = (uint64_t *)heap_ptr_offset(cur_hdr, sizeof(struct alloc_hdr) + cur_hdr->alloc_size);
                if (*cookie_check == HEAP_COOKIE) {
                    // Good cookie
                    next_hdr = heap_ptr_offset(cur_hdr, chunk_size);
                    if (prev_hdr != NULL) {
                        prev_free = ALLOC_IS_FREE == (prev_hdr->chunk_size & ALLOC_IS_FREE);
                        if (prev_free) {
                            chunk_size = ((void *)heap_ptr_offset(cur_hdr, chunk_size)) - ((void *)heap_ptr_offset(prev_hdr, 0));
                            prev_hdr->chunk_size = chunk_size;

                            // Zero it out
                            cur_hdr->chunk_size = 0;
                            cur_hdr->alloc_size = 0;

                            cur_hdr = prev_hdr;
                        }
                    }

                    next_free = ALLOC_IS_FREE == (next_hdr->chunk_size & ALLOC_IS_FREE);
                    if (next_free) {
                        chunk_size = heap_ptr_offset(next_hdr, chunk_size) - heap_ptr_offset(cur_hdr, 0);
                        cur_hdr->chunk_size = chunk_size;

                        // Zero it out
                        next_hdr->chunk_size = 0;
                        next_hdr->alloc_size = 0;
                    }

                    cur_hdr->chunk_size = heap_apply_flags(cur_hdr->chunk_size, ALLOC_HAS_NEXT | ALLOC_IS_FREE);
                    return 0;
                } else {
                    // Bad cookie
                    return -4;
                }
            }
        }

        prev_hdr = cur_hdr;
        cur_hdr = heap_ptr_offset(cur_hdr, chunk_size);
    }

    // It is a valid pointer in the heap, but not a valid allocation
    return -3;
}

size_t heap_max()
{
    size_t available_size, chunk_size;
    struct alloc_hdr *cur_hdr;
    bool has_next, alloc_freed;
    int biggest_size;

    if (heap_arena == NULL) heap_arena_init();

    biggest_size = 0;
    cur_hdr = (struct alloc_hdr *)heap_arena + sizeof(struct heap);
    while (true) {
        if (!heap_check_range(cur_hdr, cur_hdr + sizeof(struct alloc_hdr))) {
            break;
        }

        has_next = cur_hdr->chunk_size & ALLOC_HAS_NEXT;
        if (0 == has_next) break;

        alloc_freed = cur_hdr->chunk_size & ALLOC_IS_FREE;
        chunk_size = heap_apply_flags(cur_hdr->chunk_size, 0);
        if (alloc_freed) {
            if (chunk_size > biggest_size) {
                biggest_size = (int)chunk_size;
            }
        }

        cur_hdr = heap_ptr_offset(cur_hdr, chunk_size);
    }

    void *end_of_the_line = ((void *)heap_arena) + sizeof(struct heap) + heap_arena->available;

    void *end_of_allocation = (((void *)cur_hdr) + heap_apply_flags(cur_hdr->chunk_size, 0));

    available_size = 0;
    if (heap_check_range(end_of_allocation, end_of_allocation)) {
        available_size = end_of_the_line - end_of_allocation;
    }

    biggest_size = available_size > biggest_size ? available_size : biggest_size;
    biggest_size -= (sizeof(struct alloc_hdr) + 0x20);

    return biggest_size > 0 ? biggest_size : 0;
}
