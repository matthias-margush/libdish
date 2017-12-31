#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

struct Slab {
  uint8_t *start;
  uint8_t *end;
  uint8_t *free;
};

#define PAGES_PER_ALLOC 1
static long page_size = -1;

/**
 * Initialize the system.
 */
int slab_init() {
  page_size = sysconf(_SC_PAGESIZE);
  return page_size == -1 ? -1 : 0;
}

/**
 * Create a slab.
 */
int slab_create(struct Slab *slab) {
  uint8_t *start = mmap(NULL, PAGES_PER_ALLOC, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (start == MAP_FAILED) {
    return (int)start;
  }

  *slab =
      (struct Slab){.start = start, .end = start + page_size, .free = start};

  return 0;
}

/**
 * Get some space to use in the slab.
 */
void *slab_alloc(struct Slab *slab, size_t size) {
  uint8_t *free = slab->free + size;
  if (free > slab->end) {
    return NULL;
  }
  slab->free = free;
  return free;
}

/**
 * Reset the slab.
 */
int slab_reset(struct Slab *slab) {
  slab->free = slab->start;
  return 0;
}

/**
 * Completely destroy the slab.
 */
int slab_destroy(struct Slab *slab) { return munmap(slab->start, page_size); }

int main(void) {
  int err;

  err = slab_init();
  if (err != 0) {
    printf("Failed to init slab: %d.\n", err);
    return 1;
  }

  printf("Initialized slab.\n");

  struct Slab slab;
  err = slab_create(&slab);
  if (err != 0) {
    printf("Failed to create slab: %d.\n", err);
    return 1;
  }

  printf("Created slab.\n");

  char *buf = slab_alloc(&slab, 4096);
  if (!buf) {
    printf("Slab alloc failed.\n");
    return 1;
  }

  printf("Allocated 4096 bytes.\n");

  buf = slab_alloc(&slab, 1);
  if (buf) {
    printf("Allocating more memory should have failed.\n");
    return 1;
  }

  printf("Allocation successfully denied.\n");

  err = slab_reset(&slab);
  if (err != 0) {
    printf("Failed to reset slab: %d\n", err);
    return 1;
  }

  buf = slab_alloc(&slab, 4086);
  if (!buf) {
    printf("Slab realloc failed.\n");
    return 1;
  }

  printf("Reallocated 4096 bytes.\n");

  err = slab_destroy(&slab);
  if (err != 0) {
    printf("Failed to destroy slab: %d.\n", err);
    return 1;
  }

  return 0;
}
