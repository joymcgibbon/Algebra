#pragma once
#include "Group.h"
#include <vector> 
#include <algorithm>
#include <random>

#ifndef NUM_PROCESSORS
#define NUM_PROCESSORS 12
#endif

struct GroupOperations {
public:
	template <typename Element>
	struct Multithreaded {
		std::set<FiniteGroup<Element>> allSubgroups(const FiniteGroup<Element>& group) {
			// Threads concurrently find all possible combinations of elements that form subgroups recursively
			std::thread threads[NUM_PROCESSORS];
			for (std::thread& thread : threads)
				thread = std::thread([&]() {
				generateAllSubgroups(group, FiniteGroup<Element>(), group.order() / 2, group.elements.begin());
					});
			for (std::thread& thread : threads)
				thread.join();

			subgroups.insert(FiniteGroup<Element>(group));
			return subgroups;
		}

	private:
		void generateAllSubgroups(const FiniteGroup<Element>& group, const FiniteGroup<Element>& subgroup, size_t maxSize, typename std::set<Element>::iterator itr) {
			for (; itr != group.elements.end(); ++itr) {
				FiniteGroup<Element> tmp(subgroup);
				const Element element = *itr;
				tmp.insertElement(element);
				if (tmp.order() <= group.order() / 2 && tmp.valid(group) && insert(tmp))
					generateAllSubgroups(tmp, maxSize, itr);
			}
		}

		inline bool insert(const FiniteGroup<Element>& subgroup) noexcept {
			const std::lock_guard<std::mutex> lock(writelock);
			return subgroups.insert(subgroup).second;
		};

		std::set<FiniteGroup<Element>> subgroups;
		std::mutex writelock;
	};

	template <typename Element>
	static std::map<Element, Element> getInverses(const FiniteGroup<Element>& group) noexcept {
		std::map<Element, Element> inverses;
		for (const Element& a : group.elements) {
			for (const Element& b : group.elements)
				if (a + b == group.identity)
					inverses.insert({ a, b });
		}
		return inverses;
	};

	template <typename Element>
	static std::set<FiniteGroup<Element>> leftCosets(const FiniteGroup<Element>& group, const FiniteGroup<Element>& subgroup) {
		std::set<FiniteGroup<Element>> result;
		if (!subgroup.validSubgroup(group))
			return result;

		while (result.size() < group.order() / subgroup.order()) {
			for (const Element& element : group.elements) {
				FiniteGroup<Element> coset;
				for (const Element& subElement : subgroup.elements)
					coset.insert(element + subElement);
				coset.insertGenerator(element);
				result.insert(coset);

			}
		}
		return result;
	};

	template <typename Element>
	static std::set<FiniteGroup<Element>> rightCosets(const FiniteGroup<Element>& group, const FiniteGroup<Element>& subgroup) {
		std::set<FiniteGroup<Element>> result;
		if (!(subgroup <= group))
			return result;

		while (result.size() < group.order() / subgroup.order()) {
			for (const Element& element : group.elements) {
				FiniteGroup<Element> coset;
				for (const Element& subElement : subgroup.elements)
					coset.insert(subElement + element);
				coset.insertGenerator(element);
				result.insert(coset);
			}
		}
		return result;
	};

	template <typename Element>
	static std::set<FiniteGroup<Element>> cyclicSubgroups(const FiniteGroup<Element>& group) {
		std::set<FiniteGroup<Element>> subgroups;
		for (const Element& factor : group.elements) {
			Element tmp = factor;
			FiniteGroup<Element> group;
			group.elements = { factor };
			do {
				tmp = tmp + factor;
				group.insert(tmp);
			} while (!group.isIdentity(tmp));
			subgroups.insert(group);
		}
		return subgroups;
	};

	template <typename Element>
	static std::vector<Element> generators(const FiniteGroup<Element>& group) {
		std::vector<Element> tmpElements(++group.elements.begin(), group.elements.end());
		std::mt19937 rand(1);
		//std::shuffle(tmpElements.begin(), tmpElements.end(), rand);
		FiniteGroup<Element> subgroup;

		do {
			const auto tmp = tmpElements.begin();
			subgroup.insertElement(*tmp);
			tmpElements.erase(tmp);
		} while (subgroup.order() < group.order());
		return subgroup.getGenerators();
	};

	template <typename Element>
	static std::vector<std::vector<Element>> cayleyTable(const FiniteGroup<Element>& group) {
		return cayleyTable(group.elements);
	};

	template <typename Element>
	static std::vector<std::vector<Element>> cayleyTable(const std::set<Element> elements) {
		std::vector<std::vector<Element>> table;
		table.resize(elements.size());

		for (std::vector<Element>& vec : table)
			vec.resize(elements.size());

		auto itr = elements.begin();
		for (int i = 0; i < elements.size(); ++i, ++itr) {
			auto itr2 = elements.begin();
			for (int j = 0; j < elements.size(); ++j, ++itr2) {
				table[i][j] = *itr + *itr2;
			}
		}
		return table;
	};

	template <typename Element, typename MockElement>
	std::map<Element, MockElement> isomorphism(const FiniteGroup<Element>& G, const FiniteGroup<MockElement>& H) {
		std::map<Element, MockElement> isomorphism;
		isomorphism[Element()] = MockElement();

		std::vector<std::vector<Element>> cayleyTableG = cayleyTable(G.elements);
		std::vector<std::vector<MockElement>> cayleyTableH = cayleyTable(G.elements);

	};

};

template <typename T>
void printTable(std::ostream& out, std::vector<std::vector<T>> table) {
	out << '\n';
	for (int i = 0; i < table.size(); ++i) {
		for (int j = 0; j < table[i].size(); ++j)
			out << table[i][j] << '\t';
		out << '\n';
	}
}