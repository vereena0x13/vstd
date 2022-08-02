struct DataInput {
    virtual u8 read_u8() = 0;

    inline u16 read_u16() {
        return (cast(u16, read_u8()) << 8) | 
                cast(u16, read_u8());
    }

    inline u32 read_u32() {
        return (cast(u32, read_u8()) << 24) |
               (cast(u32, read_u8()) << 16) |
               (cast(u32, read_u8()) << 8) |
                read_u8();
    }

    inline u64 read_u64() {
        return (cast(u64, read_u8()) << 56) |
               (cast(u64, read_u8()) << 48) |
               (cast(u64, read_u8()) << 40) |
               (cast(u64, read_u8()) << 32) |
               (cast(u64, read_u8()) << 24) |
               (cast(u64, read_u8()) << 16) |
               (cast(u64, read_u8()) << 8) |
                cast(u64, read_u8());
    }

    inline s8 read_s8() { return cast(s8, read_u8()); }
    inline s16 read_s16() { return cast(s16, read_u16()); }
    inline s32 read_s32() { return cast(s32, read_u32()); }
    inline s64 read_s64() { return cast(s64, read_u64()); }

    inline f32 read_f32() {
        union {
            u32 u;
            f32 f;
        };
        u = read_u32();
        return f;
    }

    inline f64 read_f64() {
        union {
            u64 u;
            f64 f;
        };
        u = read_u64();
        return f;
    }

    inline str read_str() {
        u64 len = read_u64();
        str s = mkstr(NULL, len, allocator);
        for(u64 i = 0; i < len; i++) s[i] = read_u8();
        return s;
    }

    inline cstr read_cstr() {
        u64 len = read_u64();
        cstr s = cast(cstr, xalloc(len + 1, allocator));
        for(u64 i = 0; i < len; i++) s[i] = read_u8();
        return s;
    }
};