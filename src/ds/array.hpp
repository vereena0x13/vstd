#ifndef VSTD_ARRAY_DEFAULT_SIZE
#define VSTD_ARRAY_DEFAULT_SIZE 64
#endif

template<typename T>
struct Array {
    struct Iterator {
        Iterator() : a(NULL) {}
        Iterator(Array<T> *_a, u64 _index) : a(_a), index(_index) {}

        Iterator& operator++() { index++; return *this; }
        Iterator operator++(int) { auto it = *this; operator++(); return it; }
        Iterator& operator--() { index--; return *this; }
        Iterator operator--(int) { auto it = *this; operator--(); return it; }

        T& operator*() { return (*a)[index]; }

        bool operator==(Iterator const& b) const {
            if(a != b.a) return false;
            return index == b.index;
        }

        bool operator!=(Iterator const& b) const { return !(*this == b ); }
    private:
        Array<T> *a;
        u64 index = 0;
    };

    struct Const_Iterator {
        Const_Iterator() : a(NULL) {}
        Const_Iterator(Array<T> const* _a, u64 _index) : a(_a), index(_index) {}

        Const_Iterator& operator++() { index++; return *this; }
        Const_Iterator operator++(int) { auto it = *this; operator++(); return it; }
        Const_Iterator& operator--() { index--; return *this; }
        Const_Iterator operator--(int) { auto it = *this; operator--(); return it; }

        T const& operator*() { return (*a)[index]; }

        bool operator==(Const_Iterator const& b) const {
            if(a != b.a) return false;
            return index == b.index;
        }

        bool operator!=(Const_Iterator const& b) const { return !(*this == b ); }
    private:
        Array<T> const* a;
        u64 index = 0;
    };

	u64 count = 0;
	u64 size = 0;
	T *data = NULL;
	Allocator *a;

	Array(Allocator *_a = ::allocator) : a(_a) {}

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, count); }
    Const_Iterator begin() const { return Const_Iterator(this, 0); }
    Const_Iterator end() const { return Const_Iterator(this, count); }

	void free() {
		if(data) {
			xfree(data, a);
			count = 0;
			size = 0;
			data = NULL;
		}
	}

	void resize(u64 size) {
		if(size < VSTD_ARRAY_DEFAULT_SIZE) return;

		assert(size >= count);

		T *new_data = cast(T*, xalloc(sizeof(T) * size, a));
		assert(new_data);
		memcpy(new_data, data, sizeof(T) * count);
		xfree(data, a);

		this->size = size;
		this->data = new_data;
	}

	void clear() {
		count = 0;
	}

	u64 push(T v) {
		check_init();

		if(count == size) {
			resize(size * 2);
		}

		u64 i = count++;
		data[i] = v;
		return i;
	}

	T pop() {
		assert(data);
		assert(count > 0);
		return data[--count];
	}

	void insert(T v, u64 index) {
		check_init();

		if(count == size) {
			resize(size * 2);
		}

		for(u64 i = count - 1; i >= index; i--) {
			if(i > count) break;
			data[i + 1] = data[i];
		}
		data[index] = v;
		count++;
	}

	T ordered_remove(u64 i) {
		assert(data);
		assert(i < count);
		T r = data[i];
		for(u64 j = i; j < count - 1; j++) {
			data[j] = data[j + 1];
		}
		count--;
		return r;
	}

	T unordered_remove(u64 i) {
		assert(data);
		assert(i < count);
		T r = data[i];
		data[i] = data[count - 1];
		count--;
		return r;
	}

	bool contains(T x) {
		if(!data) return false;
		return index(x) != -1;
	}

	s64 index(T x) {
		for(u64 i = 0; i < count; i++) {
			if(data[i] == x) return i;
		}
		return -1;
	}

	T& operator[](u64 x) { return data[x]; }
	T const& operator[](u64 x) const { return data[x]; }

private:
	void check_init() {
		if(data == NULL) {
			size = VSTD_ARRAY_DEFAULT_SIZE;
			data = cast(T*, xalloc(sizeof(T) * size, a));
		}
	}
};