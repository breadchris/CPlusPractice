#include <stddef.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HEAP_MAX (1 << 26)

#ifdef DEBUG
    #define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define log_info(M, ...)
#endif

struct alloc_hdr {
    void *fwd_ptr;
    void *next;
    size_t size;
};

struct heap {
    void *heap_ptr;
    size_t next_alloc;
    struct alloc_hdr *root_alloc;
    struct alloc_hdr *tail_alloc;
};

struct heap from_heap, to_heap;

struct scan_root {
    void *addr;
    size_t len;
};

size_t roots_size = 0;
struct scan_root roots[HEAP_MAX / sizeof(struct alloc_hdr)];

void add_root (void *, size_t);
void *allocate (size_t, size_t *);
void del_root (void *addr);
size_t heap_max ();

bool ptr_in_heap(void *ptr)
{
    // Check which heap is being used, if it is not in heap,
    void *start_ptr, *end_ptr;

    start_ptr = from_heap.heap_ptr;
    end_ptr = start_ptr + HEAP_MAX;
    log_info("Check if %p is range %p - %p", ptr, start_ptr, end_ptr);
    return ptr >= start_ptr && ptr <= end_ptr;
}

void *get_alloc_from_ptr(void *ptr)
{
    struct alloc_hdr *cur_alloc;
    void *start_ptr, *end_ptr;

    if (!ptr_in_heap(ptr)) return NULL;

    cur_alloc = from_heap.root_alloc;
    log_info("Root alloc: %p", from_heap.root_alloc);
    while (cur_alloc != NULL) {
        // if ptr in cur_alloc range then gucci
        start_ptr = ((void *)cur_alloc) + sizeof(struct alloc_hdr);
        end_ptr = start_ptr + cur_alloc->size;

        log_info("\tCheck if %p is range %p - %p", ptr, start_ptr, end_ptr);
        if (ptr >= start_ptr && ptr < end_ptr) {
            // ptr in range
            log_info("FOUND PTR");
            return cur_alloc;
        }
        cur_alloc = cur_alloc->next;
    }

    // No matching allocation
    return NULL;
}

void heap_arena_init()
{
    from_heap.heap_ptr = mmap(NULL, HEAP_MAX, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    to_heap.heap_ptr   = mmap(NULL, HEAP_MAX, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    log_info("from_heap base %p and to_heap base %p", from_heap.heap_ptr, to_heap.heap_ptr);

    from_heap.next_alloc = 0;

    from_heap.root_alloc = NULL;
    from_heap.tail_alloc = NULL;
}

void add_root (void *addr, size_t len)
{
    // Follow start_root until next_root is null
    size_t i;

    log_info("Adding root %p", addr);
    for (i = 0; i < roots_size; i++) {
        if (roots[i].addr == NULL) {
            roots[i].addr = addr;
            roots[i].len = len;
            return;
        }
    }

    if (i == roots_size) {
        roots[i].addr = addr;
        roots[i].len = len;
        roots_size++;
    }
}

void del_root (void *addr)
{
    size_t i;

    for (i = 0; i < roots_size; i++) {
        if (roots[i].addr == addr) {
            log_info("Deleting root %p", addr);
            roots[i].addr = NULL;
            roots[i].len = 0;
            break;
        }
    }
}

void fix_heap_ptrs(void *ptr, size_t len, size_t *collected)
{
    struct alloc_hdr *alloc_ptr, *dst_ptr;
    void **scan;
    size_t offset;
    bool keep_fixing;

    for (scan = ptr; (void *)scan < ptr + len; scan++) {
        alloc_ptr = get_alloc_from_ptr(*scan);
        log_info("Scanning for %p allocation and found %p", *scan, alloc_ptr);
        if (alloc_ptr != NULL) {
            if (alloc_ptr->fwd_ptr == NULL) {
                dst_ptr = to_heap.heap_ptr + to_heap.next_alloc;
                to_heap.next_alloc += sizeof(struct alloc_hdr) + alloc_ptr->size;

                log_info("Moving %p to %p", alloc_ptr, dst_ptr);
                memcpy(dst_ptr, alloc_ptr, sizeof(struct alloc_hdr) + alloc_ptr->size);
                alloc_ptr->fwd_ptr = dst_ptr;

                if (to_heap.root_alloc == NULL) {
                    to_heap.root_alloc = dst_ptr;
                } else {
                    to_heap.tail_alloc->next = dst_ptr;
                    log_info("Pointing tail %p to %p", to_heap.tail_alloc, dst_ptr);
                }

                dst_ptr->next = NULL;
                to_heap.tail_alloc = dst_ptr;

                *collected += 1;

                keep_fixing = true;
            } else {
                // If already copied, just use the forwarding ptr
                dst_ptr = alloc_ptr->fwd_ptr;
                log_info("Found forwarding ptr %p", dst_ptr);
                keep_fixing = false;
            }

            offset = ((void *)*scan) - ((void *)alloc_ptr);
            log_info("Updating pointer %p to now point to %p", *scan, ((void *)dst_ptr) + offset);
            *scan = ((void *)dst_ptr) + offset;

            if (keep_fixing) {
                fix_heap_ptrs(alloc_ptr, sizeof(struct alloc_hdr) + alloc_ptr->size, collected);
            }
        }
    }
}

size_t garbage_collect()
{
    size_t i, collected;
    void *scan;
    void *tmp;

    to_heap.root_alloc = NULL;
    to_heap.tail_alloc = NULL;
    to_heap.next_alloc = 0;

    collected = 0;
    for (i = 0; i < roots_size; i++) {
        // Iterate over all roots
        if (roots[i].addr != NULL) {
            log_info("Found ptr %p with len %zu", roots[i].addr, roots[i].len);
            fix_heap_ptrs(roots[i].addr, roots[i].len, &collected);
        }
    }

    // Switch heaps around
    tmp = to_heap.heap_ptr;
    to_heap.heap_ptr = from_heap.heap_ptr;
    from_heap.heap_ptr = tmp;

    from_heap.next_alloc = to_heap.next_alloc;
    from_heap.root_alloc = to_heap.root_alloc;
    from_heap.tail_alloc = to_heap.tail_alloc;

    return collected;
}

void *allocate (size_t request, size_t *collected)
{
    struct alloc_hdr *new_alloc, *cur_alloc;
    void *new_alloc_ptr, *base_ptr;
    size_t next_alloc, padding, collected_bytes;

    if (from_heap.heap_ptr == NULL) heap_arena_init();

    collected_bytes = 0;
    cur_alloc = NULL;

    next_alloc = request + from_heap.next_alloc;
    if (next_alloc + sizeof(struct alloc_hdr) > HEAP_MAX) {
        // Out of mem, need to garbage collect
        log_info("Calling the garbage collector, need %zu more bytes", request + sizeof(struct alloc_hdr));
        collected_bytes = garbage_collect();
        log_info("Done garbage collecting with %zu bytes left in heap", heap_max());
        // if still cant alloc, then fucked
        next_alloc = request + from_heap.next_alloc;
        if (next_alloc + sizeof(struct alloc_hdr) > HEAP_MAX) {
            log_info("No more fucking room");
            *collected = 0;
            return NULL;
        }
    }

    base_ptr = from_heap.heap_ptr + from_heap.next_alloc;
    new_alloc = (struct alloc_hdr *)base_ptr;
    new_alloc_ptr = base_ptr + sizeof(struct alloc_hdr);

    new_alloc->fwd_ptr = NULL;
    new_alloc->next = NULL;

    padding = 8 - (request % 8);
    new_alloc->size = request + (padding == 8 ? 0 : padding);

    if (from_heap.root_alloc == NULL) {
        from_heap.root_alloc = new_alloc;
    } else {
        log_info("Pointing tail %p to %p", from_heap.tail_alloc, new_alloc);
        from_heap.tail_alloc->next = new_alloc;
    }
    from_heap.tail_alloc = new_alloc;

    from_heap.next_alloc += sizeof(struct alloc_hdr) + new_alloc->size;

    log_info("Created ptr %p, next ptr will be %p", new_alloc, from_heap.heap_ptr + from_heap.next_alloc);

    *collected = collected_bytes;
    return new_alloc_ptr;
}

size_t heap_max ()
{
    if (from_heap.next_alloc + sizeof(struct alloc_hdr) > HEAP_MAX) return 0;
    return HEAP_MAX - from_heap.next_alloc - sizeof(struct alloc_hdr);
}
