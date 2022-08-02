// TODO: use 64-bit hashes?
// TODO: switch hash table to use a 64-bit size and count?


constexpr u32 HASH_TABLE_DEFAULT_SEED = 0xB23D66D5;

template<typename K>
using HashFN = u32 (*)(K const&);

template<typename K>
using EqFN = bool (*)(K const&, K const&);

template<typename T>
u32 default_hash_fn(T const& v) {
	return murmur3((void const *) &v, sizeof(T), HASH_TABLE_DEFAULT_SEED);
}

template<typename T>
bool default_eq_fn(T const& a, T const& b) {
	return memcmp((void const*) &a, (void const*) &b, sizeof(T)) == 0;
}


#ifndef HASH_TABLE_LOAD_FACTOR_SHRINK_THRESHOLD
#define HASH_TABLE_LOAD_FACTOR_SHRINK_THRESHOLD 0.1f
#endif
#ifndef HASH_TABLE_LOAD_FACTOR_EXPAND_THRESHOLD
#define HASH_TABLE_LOAD_FACTOR_EXPAND_THRESHOLD 0.85f
#endif
#ifndef HASH_TABLE_DEFAULT_SIZE
#define HASH_TABLE_DEFAULT_SIZE 256
#endif


// NOTE: This hash table uses "Robin Hood Hashing":
// " Robin Hood hashing is a technique for implementing hash tables.   				"
// " It is based on open addressing with a simple but clever twist: As new   		"
// " keys are inserted, old keys are shifted around in a way such that all   		"
// " keys stay reasonably close to the slot they originally hash to. In particular, "
// " the variance of the keys distances from their "home" slots is minimized.  		"

template<typename K, typename V, HashFN<K> hash_fn = default_hash_fn, EqFN<K> eq_fn = default_eq_fn>
struct Hash_Table {
	struct Slot {
		K key;
		V value;
		u32 hash;
	};

	u32 count;
	u32 size;
	u32 mask;
	Slot *slots;

	// TODO: make this a lazy-init structure
	void init(u32 size = HASH_TABLE_DEFAULT_SIZE) {
		assert(is_pow2(size));
		this->count = 0;
		this->size = size;
		this->mask = size - 1;
		slots = cast(Slot*, xalloc(size * sizeof(Slot)));
		for(u32 i = 0; i < size; i++) slots[i].hash = 0;
	}

	void free() {
		xfree(slots);
	}

	void resize(u32 new_size) {
		assert(new_size);
		new_size = next_pow2(new_size);
        assert(is_pow2(new_size));
        assert(new_size >= count);

		u32 old_count = count;
		u32 old_size = size;
		Slot *old_slots = slots;

		count = 0;
		size = new_size;
		mask = size - 1;
		slots = cast(Slot*, xalloc(size * sizeof(Slot)));
		for(u32 i = 0; i < size; i++) slots[i].hash = 0;

		for(u32 i = 0; i < old_size; i++) {
			if(old_slots[i].hash) {
				set(old_slots[i].key, old_slots[i].value);
			}
		}

		assert(count == old_count);

		xfree(old_slots);
	}

	void set(K _key, V _value) {
		if(load_factor() > HASH_TABLE_LOAD_FACTOR_EXPAND_THRESHOLD) {
			resize(size * 2);
		}

		K key = _key;
		V value = _value;
		u32 hash = hash_key(key);

		s32 i = hash & mask;
		s32 dist = 0;
		for(;;) {
			if(slots[i].hash == 0) {
				slots[i].key = key;
				slots[i].value = value;
				slots[i].hash = hash;
				count++;
				return;
			}

            s32 epd = probe_dist(i, slots[i].hash);
			if(epd < dist) {
				assert(slots[i].hash);

				K _k = slots[i].key;
				V _v = slots[i].value;
				u32 _h = slots[i].hash;

				slots[i].key = key;
				slots[i].value = value;
				slots[i].hash = hash;

				key = _k;
				value = _v;
				hash = _h;

				dist = epd;
			}

			i = (i + 1) & mask;
			dist++;
		}
	}

	V get(K key) const {
		s32 i = index_of(key);
		if(i == -1) {
			V dummy;
			memset(&dummy, 0, sizeof(V)); // NOTE: not strictly necessary...
			return dummy;
		}
		return slots[i].value;
	}

	s32 index_of(K key) const {
		u32 hash = hash_key(key);
		s32 i = hash & mask;
		u32 dist = 0;
		for(;;) {
			if(slots[i].hash == 0) {
				return -1;
			}

            // NOTE TODO: Are we supposed to be using `hash`?
            // Or are we supposed to be using slots[i].hash?...
            s32 epd = probe_dist(i, hash);
			if(dist > epd) {
				return -1;
			}

			if(slots[i].hash == hash && eq_fn(slots[i].key, key)) {
				return i;
			}

			i = (i + 1) & mask;
			dist++;
		}
	}

	bool remove(K key) {
		s32 i = index_of(key);
		if(i == -1) return false;

		for(s32 j = 0; j < size - 1; j++) {
			s32 k = (i + 1) & mask;

			if(slots[k].hash == 0) break;

            s32 epd = probe_dist(k, slots[k].hash);
			if(epd == 0) break;

			memcpy(&slots[i], &slots[k], sizeof(Slot));

			i = k;
		}

		slots[i].hash = 0;
		count--;

		if(load_factor() < HASH_TABLE_LOAD_FACTOR_SHRINK_THRESHOLD) {
			resize(max(size / 2, u32(HASH_TABLE_DEFAULT_SIZE)));
		}

		return true;
	}

	f32 load_factor() const {
		return (f32) count / (f32) size;
	}

private:
    s32 probe_dist(u32 hash, u32 slot_index) const noexcept {
        return (slot_index + size - (hash & mask)) & mask;
    }

	static u32 hash_key(K key) {
		u32 h = hash_fn(key);
		// NOTE: a hash of 0 represents an empty slot
		if(h == 0) h |= 1;
		return h;
	}
};


u32 str_hash_fn(str const& s) {
	return murmur3((void const*) s, strsz(s), HASH_TABLE_DEFAULT_SEED);
}

bool str_eq_fn(str const& a, str const& b) {
	return streq(a, b);
}

u32 cstr_hash_fn(cstr const& s) {
    return murmur3((void const*) s, strlen(s), HASH_TABLE_DEFAULT_SEED);
}

bool cstr_eq_fn(cstr const& a, cstr const& b) {
    return strcmp(a, b) == 0;
}