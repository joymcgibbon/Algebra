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

// copied from https://blog.tartanllama.xyz/exploding-tuples-fold-expressions/ 
template <size_t... Idx>
auto make_index_dispatcher(std::index_sequence<Idx...>) {
	return [](auto&& f) { (f(std::integral_constant<size_t, Idx>{}), ...); };
}

template <size_t N>
auto make_index_dispatcher() {
	return make_index_dispatcher(std::make_index_sequence<N>{});
}

template <typename Tuple, typename Func>
void for_each(Tuple&& t, Func&& f) {
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = make_index_dispatcher<n>();
	dispatcher([&f, &t](auto idx) { f(std::get<idx>(std::forward<Tuple>(t))); });
}

// modified for 2 tuples
template <typename Tuple, typename Func>
void for_each(Tuple&& t1, Tuple&& t2, Func&& f) {
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = make_index_dispatcher<n>();
	dispatcher([&f, &t1, &t2](auto idx) { f(std::get<idx>(std::forward<Tuple>(t1)), std::get<idx>(std::forward<Tuple>(t2))); });
}

// modified for 3 tuples
template <typename Tuple, typename Func>
void for_each(Tuple&& t1, Tuple&& t2, Tuple&& t3, Func&& f) {
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = make_index_dispatcher<n>();
	dispatcher([&f, &t1, &t2, &t3](auto idx) { f(
		std::get<idx>(std::forward<Tuple>(t1)), 
		std::get<idx>(std::forward<Tuple>(t2)),
		std::get<idx>(std::forward<Tuple>(t3))
	); });
}

