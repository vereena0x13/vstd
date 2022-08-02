struct DataOutput {
    virtual void write_u8(u8 x) = 0;

    inline void write_u16(u16 x) {
        write_u8((x >> 8) & 0xFF);
        write_u8(x & 0xFF);
    }

    inline  void write_u32(u32 x) {
        write_u8((x >> 24) & 0xFF);
        write_u8((x >> 16) & 0xFF);
        write_u8((x >> 8) & 0xFF);
        write_u8(x & 0xFF);
    }

    inline void write_u64(u64 x) {
        write_u8((x >> 56) & 0xFF);
        write_u8((x >> 48) & 0xFF);
        write_u8((x >> 40) & 0xFF);
        write_u8((x >> 32) & 0xFF);
        write_u8((x >> 24) & 0xFF);
        write_u8((x >> 16) & 0xFF);
        write_u8((x >> 8) & 0xFF);
        write_u8(x & 0xFF);
    }

    inline void write_s8(s8 x) { write_u8(cast(u8, x)); }
    inline void write_s16(s16 x) { write_u16(cast(u16, x)); }
    inline void write_s32(s32 x) { write_u32(cast(u32, x)); }
    inline void write_s64(s64 x) { write_u64(cast(u64, x)); }

    inline void write_f32(f32 x) {
        union {
            f32 f;
            u32 u;
        };
        f = x;
        write_u32(u);
    }

    inline void write_f64(f64 x) {
        union {
            f64 f;
            u64 u;
        };
        f = x;
        write_u64(u); 
    }

    inline void write_str(str s) {
        u64 len = strsz(s);
        write_u64(len);
        for(u64 i = 0; i < len; i++) write_u8(s[i]);
    }

    inline void write_cstr(cstr s) {
        u64 len = strlen(s);
        write_u64(len);
        for(u64 i = 0; i < len; i++) write_u8(s[i]);
    }
};

struct RandomAccessDataOutput {
    virtual u64 reserve(u64 n) = 0;
    virtual void set_u8(u64 i, u8 x) = 0;

    inline void set_u16(u64 i, u16 x) {
        set_u8(i, (x >> 8) & 0xFF);
        set_u8(i + 1, x & 0xFF);
    }

    inline void set_u32(u64 i, u32 x) {
        set_u8(i, (x >> 24) & 0xFF);
        set_u8(i + 1, (x >> 16) & 0xFF);
        set_u8(i + 2, (x >> 8) & 0xFF);
        set_u8(i + 3, x & 0xFF);
    }

    inline void set_u64(u64 i, u64 x) {
        set_u8(i, (x >> 56) & 0xFF);
        set_u8(i + 1, (x >> 48) & 0xFF);
        set_u8(i + 2, (x >> 40) & 0xFF);
        set_u8(i + 3, (x >> 32) & 0xFF);
        set_u8(i + 4, (x >> 24) & 0xFF);
        set_u8(i + 5, (x >> 16) & 0xFF);
        set_u8(i + 6, (x >> 8) & 0xFF);
        set_u8(i + 7, x & 0xFF);
    }

    inline void set_s8(u64 i, s8 x) { set_u8(i, cast(u8, x)); }
    inline void set_s16(u64 i, s16 x) { set_u16(i, cast(u16, x)); }
    inline void set_s32(u64 i, s32 x) { set_u32(i, cast(u32, x)); }
    inline void set_s64(u64 i, s64 x) { set_u64(i, cast(u64, x)); }
};