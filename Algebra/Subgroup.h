#pragma once
#include <iostream>
#include <map>
#include <omp.h>
#include "SetOperations.h"
#include "Group.h"
#include <thread>
#include <mutex>

/*
#ifndef NUM_PROCESSORS
#define NUM_PROCESSORS 1
#endif

template <typename Element>
class Subgroup : public FiniteGroup {
public:

	Subgroup() : FiniteGroup() { insert(identityElement); }
	Subgroup(const FiniteGroup& group) : FiniteGroup(group) {};
	void insertElement(const Element& element) {
		const size_t tmpSize = order();
		for (const Element& original : elements) {
			Element tmp = element;
			int count = 0;
			while (tmp != identityElement) {
				const Element result = original + tmp;
				insert(result);
				insert(tmp + original);
				tmp = tmp + element;
				insert(tmp);
			}
		}
		// If elements have been added the new element is a generator
		if (tmpSize != order()) {
			generators.push_back(element);
		}
	}
	bool valid(FiniteGroup group) const {
		if (elements.empty() || order() > group.order() / 2 || group.order() % order() != 0)
			return false;
		for (const Element& a : elements)
			for (const Element& b : elements) {
				if (elements.find(a + b.inverse()) == elements.end())
					return false;
			}
		return true;
	};
	//void operator=(const Subgroup& other) noexcept { elements = other.elements; };
	void insertGenerator(Element generator) { generators.push_back(generator); }
	std::vector<Element> getGenerators() {
		if (generators.size() > 1 && generators[0] == identityElement)
			generators.erase(generators.begin());
		return generators;
	};
private:
	std::vector<Element> generators;
};

template <typename Element>
std::set<Subgroup<Element>> generateCyclicSubgroups(FiniteGroup<Element>& group) {
	std::set<Subgroup<Element>> subgroups;
	for (const Element& factor : elements) {
		Element tmp = factor;
		Subgroup group;
		group.elements = { factor };
		do {
			tmp = tmp + factor;
			group.insert(tmp);
		} while (!isIdentity(tmp));
		subgroups.insert(group);
	}
	return subgroups;
};

template <typename Element>
std::set<Subgroup<Element>> getLeftCosets(FiniteGroup<Element>& group, const Subgroup<Element>& subgroup) {
	std::set<Subgroup<Element>> result;
	if (!subgroup.valid(group))
		return result;
	while (result.size() < group.elements.size() / subgroup.elements.size()) {
		for (const Element& element : elements) {
			Subgroup coset;
			for (const Element& subElement : subgroup.elements)
				coset.insert(element + subElement);
			coset.insertGenerator(element);
			result.insert(coset);

		}
	}
	return result;
};

template <typename Element>
std::set<Subgroup<Element>> getRightCosets(FiniteGroup<Element>& group, const Subgroup<Element>& subgroup) {
	std::set<Subgroup<Element>> result;
	if (!subgroup.valid(*this))
		return result;
	while (result.size() < elements.size() / subgroup.getElements().size()) {
		for (const Element& element : elements) {
			Subgroup coset;
			for (const Element& subElement : subgroup.getElements())
				coset.insert(subElement + element);
			coset.insertGenerator(element);
			result.insert(coset);
		}
	}
	return result;
};

template <typename Element>
struct allSubgroups {
	std::set<Subgroup<Element>> generateAllSubgroups(FiniteGroup<Element>& group) noexcept {
		if (group.elements.size() < 2)
			group.generateAllElements();

		// Threads concurrently find all possible combinations of elements
		std::thread threads[NUM_PROCESSORS];
		for (int i = 0; i < NUM_PROCESSORS; ++i) {
			threads[i] = std::thread([&]() {
				generateAllSubgroups(group, Subgroup(), order() / 2, elements.begin());
				});
		}

		for (int i = 0; i < NUM_PROCESSORS; ++i)
			threads[i].join();

		subgroups.insert(Subgroup(*this));
		return subgroups;
	};
private:
	std::mutex writelock;
	inline bool insert(const Subgroup& subgroup) noexcept {
		const std::lock_guard<std::mutex> lock(writelock);
		const bool inserted = subgroups.insert(subgroup).second;
		return inserted;
	};
	void generateAllSubgroups(const Subgroup& subgroup, size_t maxSize, typename std::set<Element>::iterator itr) {
		for (; itr != elements.end(); ++itr) {
			Subgroup tmp(subgroup);
			const Element element = *itr;
			tmp.insertElement(element);
			if (tmp.valid(*this) && insert(tmp))
				generateAllSubgroups(tmp, maxSize, itr);
		}
	}
};

*/
