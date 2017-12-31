struct Slab;

/**
 * Initialize the system.
 */
int slab_init();

/**
 * Create a slab.
 */
int slab_create(struct Slab *slab);

/**
 * Get some space to use in the slab.
 */
void *slab_alloc(struct Slab * slab, size_t size);

/**
 * Reset the slab.
 */
int slab_reset(struct Slab *slab);

/**
 * Completely destroy the slab.
 */
int slab_destroy(struct Slab *slab);
