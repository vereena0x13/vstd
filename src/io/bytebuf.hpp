#ifndef BYTEBUF_DEFAULT_SIZE
#define BYTEBUF_DEFAULT_SIZE 4096
#endif

struct ByteBuf : public DataInput, public DataOutput, public RandomAccessDataOutput {
    static ByteBuf wrap(cstr s) { return ByteBuf(cast(u8*, s), 0, strlen(s)); }
    static ByteBuf wrap(u8 *p, u64 n) { return ByteBuf(p, 0, n); }

    u8 *data = NULL;
    u64 index = 0;
    u64 size;

    ByteBuf() : ByteBuf(BYTEBUF_DEFAULT_SIZE) {}
    ByteBuf(u64 _size) : size(_size) {}
    ByteBuf(u8 *_data, u64 _index, u64 _size) : data(_data), index(_index), size(_size) {}

    void deinit() {
        xfree(data);
    }

    void write_to_file(cstr path) {
        FILE *fh = fopen(path, "w");
        assert(fwrite(data, sizeof(char), index, fh) == index);
        fflush(fh);
        fclose(fh);
    }

    bool read_from_file(cstr path) {
        FILE *fh = fopen(path, "r");
        if(!fh) return false;

        fseek(fh, 0L, SEEK_END);
	    u64 size = ftell(fh);
	    rewind(fh);

        data = cast(u8*, xalloc(size));
    	assert(fread(data, sizeof(char), size, fh) == size);

        fclose(fh);

        return true;
    }

    str tostr() {
        auto s = mkstr(NULL, index);
        memcpy(s, data, index);
        return s;
    }

    void reset() {
        index = 0;
    }

    void expand() {
        if(data) {
            u64 new_size = size * 2;
            u8 *new_data = cast(u8*, xalloc(new_size));
            memcpy(new_data, data, size);
            xfree(data);
            data = new_data;
            size = new_size;
        } else {
            assert(size);
            data = cast(u8*, xalloc(size));
        }
    }

    u8 read_u8() override {
        return data[index++];
    }

    void write_u8(u8 x) override {
        if(index >= size || data == NULL) {
            expand();
        }
    
        data[index++] = x;
    }

    u64 reserve(u64 n) override {
        if(index + n >= size || data == NULL) {
            expand();
        }
        u64 i = index;
        index += n;
        return i;
    }

    void set_u8(u64 i, u8 x) override {
        data[i] = x;
    }
};