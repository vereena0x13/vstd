#ifndef ARENA_DEFAULT_BLOCK_SIZE
#define ARENA_DEFAULT_BLOCK_SIZE KIBIBYTES(64)
#endif

#ifndef ARENA_DEFAULT_PREALLOC_BLOCKS
#define ARENA_DEFAULT_PREALLOC_BLOCKS 1
#endif

#ifndef ARENA_MINIMUM_BLOCK_SIZE
#define ARENA_MINIMUM_BLOCK_SIZE KIBIBYTE
#endif

struct Arena : public Allocator {
	struct Block {
		struct Block *prev;
		u64 used = 0;
		u8 base[];

		Block(struct Block *_prev) : prev(_prev) {}
	};

	Block *current_block = NULL;
    Block *free_list = NULL;
	u64 block_size;

	Arena(Allocator *parent = allocator, u64 _block_size = ARENA_DEFAULT_BLOCK_SIZE, u32 prealloc_blocks = ARENA_DEFAULT_PREALLOC_BLOCKS) : Allocator(parent), block_size(_block_size) {
        for(u32 i = 0; i < prealloc_blocks; i++) {
            free_list = pnew(Block, xalloc(sizeof(Block) + block_size, parent), free_list);
        }
    }

    void deinit() override {
        auto b = current_block;
        while(b) {
            auto t = b->prev;
            xfree(b, parent);
            b = t;
        }
        b = free_list;
        while(b) {
            auto t = b->prev;
            xfree(b, parent);
            b = t;
        }
        Allocator::deinit();
    }

    void* alloc(u64 n) override {
        auto blk = current_block;

        if(!blk || blk->used + n >= block_size) {
            if(free_list) {
                blk = free_list;
                free_list = blk->prev;
            } else {
                blk = pnew(Block, xalloc(sizeof(Block) + block_size, parent), NULL);
            }
            current_block = blk;
        }

        assert(blk->used + n < block_size);

        auto p = blk->base + blk->used;
        blk->used += n;
        return p;
    }

    void free(void *p) override {}

    void clear() {
        auto b = current_block;
        while(b) {
            auto n = b->prev;
            b->prev = free_list;
            b->used = 0;
            free_list = b;
            b = n;
        }
        current_block = NULL;
    }
};