template<typename T, u64 capacity>
struct Static_Array {
    static constexpr u64 size = capacity;
    using value_type = T;
	using type = Static_Array<T, capacity>;
    using ref_type = type&;
    using ref_const_type = type const&;
    using ptr_type = type*;
    using ptr_const_type = type const*;

    template<typename AT>
    struct IteratorBase {
        using type = IteratorBase<AT>;

        IteratorBase() : a(NULL) {}
        IteratorBase(AT _a, u64 _index) : a(_a), index(_index) {}

        type& operator++() { index++; return *this; }
        type operator++(int) { auto it = *this; operator++(); return it; }
        type& operator--() { index--; return *this; }
        type operator--(int) { auto it = *this; operator--(); return it; }

        T& operator*() { return (*a)[index]; }

        bool operator==(type const& b) const {
            if(a != b.a) return false;
            return index == b.index;
        }

        bool operator!=(type const& b) const { return !(*this == b ); }
    private:
        AT a;
        u64 index = 0;
    };

    using Iterator = IteratorBase<ptr_type>;
    using Const_Iterator = IteratorBase<ptr_const_type>;

	T data[capacity];
	u64 count = 0;

    Iterator begin() { return Iterator (this, 0); }
    Iterator end() { return Iterator(this, count); }
    Const_Iterator begin() const { return Const_Iterator(this, 0); }
    Const_Iterator end() const { return Const_Iterator(this, count); }

	void clear() {
		count = 0;
	}

	u64 push(T datum) {
		assert(count < capacity);
		data[count] = datum;
		return count++;
	}

	T pop() {
		assert(count > 0);
		return data[--count];
	}

	T& operator[](u64 x) { return data[x]; }
	T const& operator[](u64 x) const { return data[x]; }
};