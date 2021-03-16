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

	std::set<int> getFactors(const int val) {
		if (val == 0)
			return { 0 };

		std::set<int> factors = { 1 };
		int tmp = val;
		for (; tmp % 2 == 0; tmp = tmp / 2) {
			factors.insert(2);
			factors.insert(tmp);
		}
		for (int i = 3; i <= sqrt(tmp); i += 2)
			for (; tmp % i == 0; tmp = tmp / i) {
				factors.insert(i);
				factors.insert(val);
			}
		if (tmp > 2)
			factors.insert(tmp);

		return factors;
	}
}