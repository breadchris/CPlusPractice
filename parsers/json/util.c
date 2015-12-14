
void *safe_malloc(unsigned size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("[-] Error: Allocation failed");
    }
    return ptr;
}
