template<typename T, u32 size>
struct Slot_Allocator {
    T slots[size];
    s32 count;

    Slot_Allocator() {
        memset(slots, 0, sizeof(slots));
        count = 0;
    }

    void clear() {
        count = 0;
    }

    s32 index_of(T x) {
        for(s32 i = 0; i < count; i++) {
            if(slots[i] == x) return i;
        }
        return -1;
    }

    s32 alloc(T x) {      
        s32 i = index_of(x);
        if(i != -1) return i;

        if(count == size) return -1;

        slots[count] = x;
        return count++;
    }
};