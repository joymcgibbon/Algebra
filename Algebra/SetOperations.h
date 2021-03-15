#pragma once
#include <set>
#include <vector>
#include <omp.h>

namespace operation {
	template <typename T>
	void combinations(const std::set<T>& original, size_t maxSize, typename std::set<T>::iterator itr, std::set<std::set<T>>& set, const std::set<T>& tmp = {}) {
		if (tmp.size() >= maxSize) {
			return;
		}
		while (itr != original.end()) {
			std::set<T> tmp(tmp);
			tmp.insert({ *itr++ });
			if (set.insert(tmp).second && itr != original.end())
				combinations(original, maxSize, ++itr, set, tmp);
		}
	};

	template <typename T>
	std::set<std::set<T>> combinations(const std::set<T>& original, int maxSize) {
		std::set<std::set<T>> set;
		combinations(original, maxSize, original.begin(), set);
		return set;
	};

	template <typename T>
	std::vector<std::set<T>> partitions(const std::set<T>& set, int partitions) {
		std::vector<std::set<T>>  vector;
		if (partitions < 1)
			return vector;
		vector.resize(partitions);
		auto itr = set.begin();
		size_t count = 0;
		for (int i = 0; i < partitions; ++i)
			for (int j = 0; j < set.size() / partitions; ++j) {
				vector[i].insert(*itr++);
				++count;
			}

		while (count++ < set.size())
			vector.back().insert(*itr++);
		return vector;
	}

	template <typename T>
	std::vector<std::set<T>> partitionsEvenDist(std::set<T> set, int partitions) {
		std::vector<std::set<T>>  vector;
		if (partitions < 1)
			return vector;
		vector.resize(partitions);
		auto itr = set.begin();
		size_t count = 0;
		for (size_t j = 0; j < set.size() / partitions; ++j) {
			for (int i = 0; i < partitions; ++i) {
				vector[i].insert(*itr++);
				++count;
			}
		}

		while (count++ < set.size())
			vector.back().insert(*itr++);
		return vector;
	}

	template <typename T>
	std::set<std::vector<T>> crossProduct(std::set<std::vector<T>> lhs, std::set<T> rhs) {
		std::set<std::vector<T>> result;
		if (lhs.empty()) {
			for (const T element : rhs) {
				std::vector<T> tmp;
				tmp.val.push_back(element);
				result.insert(tmp);
			}
			return result;
		}
		if (rhs.empty())
			return lhs;
		for (std::vector<T> lhsElement : lhs)
			for (T rhsElement : rhs) {
				std::vector<T> tmp = lhsElement;
				tmp.val.push_back(rhsElement);
				result.insert(tmp);
			}
		return result;
	}
}

template <class T>
struct threadsafeSet {
	threadsafeSet() { omp_init_lock(&writelock); };
	inline bool insert(const T& item) noexcept {
		omp_set_lock(&writelock);
		const bool inserted = set.insert(item).second;
		omp_unset_lock(&writelock);
		return inserted;
	};
	std::set<T> set;
	omp_lock_t writelock;
};

template <typename T>
struct mallocator {
	using value_type = T;

	mallocator() = default;
	template <class U>
	mallocator(const mallocator<U>&) {}

	T* allocate(std::size_t n) {
		if (n <= std::numeric_limits<std::size_t>::max() / sizeof(T)) {
			if (auto ptr = std::malloc(n * sizeof(T))) {
				return static_cast<T*>(ptr);
			}
		}
		throw std::bad_alloc();
	}
	void deallocate(T* ptr, std::size_t n) {
		std::free(ptr);
	}
};