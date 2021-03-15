#pragma once
#include <iostream>
#include <map>
#include <omp.h>
#include "SetOperations.h"
#include "Element.h"
#include <thread>
#include <mutex>

#ifndef NUM_PROCESSORS
#define NUM_PROCESSORS 12
#endif

template <typename Element>
class FiniteGroup {
public:
	typedef Element Element;
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
			if (tmpSize != order())
				generators.push_back(element);
		}
		bool valid(FiniteGroup group) const {
			if (elements.empty() || elements.size() > group.order() / 2 || group.elements.size() % elements.size() != 0)
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
	FiniteGroup() : identityElement(Element()) { };
	FiniteGroup(const FiniteGroup& group) : identityElement(Element()) { elements = group.elements; };
	inline bool isIdentity(Element& e) const noexcept { return e == identityElement; };
	std::set<Element> getElements() const noexcept { return elements; };
	std::map<Element, Element> getInverses() noexcept {
		for (const Element& element : elements) {
			for (const Element& element2 : elements)
				if (element + element2 == identityElement)
					inverses.insert({ element, element2 });
		}
		return inverses;
	};
	size_t order() const noexcept { return elements.size(); };
	bool empty() const noexcept { return elements.empty(); };
	bool validIdentity() const {
		for (const Element& element : elements)
			if (element + identityElement != element || identityElement + element != element)
				return false;
		return true;
	};
	bool closed() const {
		for (const Element& a : elements)
			for (const Element& b : elements)
				if (elements.find(a + b) == elements.end())
					return false;
		return true;
	};
	bool validInverses() const {
		for (const Element& element : elements) {
			Element inverse = element.inverse();
			Element result = element + inverse;
			if (result != identityElement)
				return false;
		}
		return true;
	};
	void generateCyclicSubgroups() {
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
	}
	std::set<Subgroup> getLeftCosets(const Subgroup& subgroup) {
		std::set<Subgroup> result;
		if (!subgroup.valid(*this))
			return result;
		while (result.size() < elements.size() / subgroup.elements.size()) {
			for (const Element& element : elements) {
				Subgroup coset;
				for (const Element& subElement : subgroup.elements)
					coset.insert(element + subElement);
				coset.insertGenerator(element);
				result.insert(coset);

			}
		}
		return result;
	}
	std::set<Subgroup> getRightCosets(const Subgroup& subgroup) {
		std::set<Subgroup> result;
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
	}
	std::set<Subgroup> generateAllSubgroups() {
		if (elements.size() < 2)
			generateAllElements();

		// Threads concurrently find all possible combinations of elements
		std::thread threads[NUM_PROCESSORS];
		for (int i = 0; i < NUM_PROCESSORS; ++i) {
			threads[i] = std::thread([&]() {
				generateAllSubgroups(Subgroup(), order() / 2, elements.begin());
				});
		}

		for (int i = 0; i < NUM_PROCESSORS; ++i)
			threads[i].join();

		subgroups.insert(Subgroup(*this));
		return subgroups;
	}
	friend bool operator<(const FiniteGroup& lhs, const FiniteGroup& rhs) noexcept {
		if (lhs.elements.size() != rhs.elements.size())
			return lhs.elements.size() < rhs.elements.size();
		auto itr2 = rhs.elements.begin();
		for (auto itr1 = lhs.elements.begin(); itr1 != lhs.elements.end(); ++itr1, ++itr2)
			if (*itr1 != *itr2)
				return (*itr1 < *itr2);
		return false;
	}
	friend bool operator==(const FiniteGroup& lhs, const FiniteGroup& rhs) noexcept {
		if (lhs.elements.size() != rhs.elements.size())
			return false;
		auto itr2 = rhs.elements.begin();
		for (auto itr1 = lhs.elements.begin(); itr1 != lhs.elements.end(); ++itr1, ++itr2)
			if (*itr1 != *itr2)
				return false;
		return true;
	}
	friend std::ostream& operator<<(std::ostream& out, FiniteGroup& group) {
		out << "{";
		bool first = true;
		for (const Element& element : group.getElements()) {
			if (first)
				first = false;
			else
				out << ',';
			out << element;
		}
		out << "}";
		return out;
	}
	void generateAllElements() {
		elements = identityElement.generateAllElements();

		if (empty())
			throw std::invalid_argument("Group is not valid: Must not be empty.");
		if (!validIdentity())
			throw std::invalid_argument("Group is not valid: Identity element is not the identity.");
		if (!closed())
			throw std::invalid_argument("Group is not valid: Must be closed.");
		//	if (!validInverses())
		//		throw std::invalid_argument("Group is not valid: Inverses are not inverses.");
		getInverses();
	};
private:
	std::map<Element, Element> inverses;
	std::mutex writelock;
	Element identityElement;
	std::set<Element> elements;
	std::set<Subgroup> subgroups;
	inline bool insert(const Element& g1) noexcept { return elements.insert(g1).second; };
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

