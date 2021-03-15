/*
#pragma once
#include <iostream>
#include <map>
#include <omp.h>
#include "SetOperations.h"
#include "Element.h"

template <typename Element>
class FiniteGroup {
public:
	typedef Element Element;
	class Subgroup : public FiniteGroup {
	public:
		Subgroup() : FiniteGroup() { insert(identityElement); }
		void insertElement(const Element& element) {
			const size_t tmpSize = order();
			for (const Element& original : elements) {
				Element tmp = element;
				while (!isIdentity(tmp)) {
					insert(original + tmp);
					insert(tmp + original);
					tmp = tmp + element;
					insert(tmp);
				}
			}
			if (tmpSize != elements.size()) {
				if (generators.size() == 1 && generators[0] == identityElement)
					generators.clear();
				generators.push_back(element);
			}
		}
		bool valid(FiniteGroup group) const {
			if (elements.empty() || group.elements.size() % elements.size() != 0)
				return false;
			for (const Element& a : elements)
				for (const Element& b : elements) {
					if (elements.find(a + b.inverse()) == elements.end())
						return false;
				}
			return true;
		};
		void setElements(std::set<Element> set) { elements = set; };
		void insertGenerator(Element generator) { generators.push_back(generator); }
		std::vector<Element> getGenerators() const { return generators; };
	private:
		std::vector< Element> generators;

	};
	FiniteGroup() : identityElement(Element()) { };
	void initialize() { generateAllElements(); buildInverseList(); }
	inline bool isIdentity(Element& e) const noexcept { return e == identityElement; };
	std::set<Element> getElements() const noexcept { return elements; };
	std::map<Element, Element> getInverses() const noexcept { return inverses; };
	size_t order() const noexcept { return elements.size(); };
	bool empty() const noexcept { return elements.empty(); };
	void buildInverseList() {
		for (Element element : elements)
			inverses.insert({ element, element.inverse() });
	}
	std::set<Subgroup> generateAllSubgroups() noexcept {
		if (inverses.empty())
			initialize();

		Subgroup subgroup;
		generateAllSubgroups(subgroup, order() / 2, elements.begin());

		Subgroup all;
		all.setElements(elements);
		subgroups.insert(all);
		return subgroups;
	}
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
	std::set<Subgroup> getLeftCosets(Subgroup subgroup) {
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
	std::set<Subgroup> getRightCosets(Subgroup subgroup) {
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
public:
	class Multithreaded : public FiniteGroup {
	public:
		Multithreaded() : FiniteGroup() { omp_init_lock(&writelock); };
		std::set<Subgroup> generateAllSubgroups() noexcept {
			if (inverses.empty())
				initialize();

			Subgroup subgroup;
#pragma omp parallel
			generateAllSubgroups(subgroup, order() / 2, elements.begin());

			Subgroup all;
			all.setElements(elements);
			subgroups.insert(all);
			return subgroups;
		}
		std::set<Subgroup> generateAllSubgroupsM() {
			if (inverses.empty())
				initialize();

			std::set<Element> tmp(elements.begin(), elements.end());
			std::set<std::set<Element>> set = operation::combinations(tmp, 2);
			const int  num_threads = (int)ceil(sqrt(set.size()) / 4);
			std::vector<std::set<std::set<Element>>>  vector = operation::partitionsEvenDist(set, num_threads);

#pragma omp parallel num_threads(num_threads)
			{
				if (num_threads != omp_get_num_threads())
					throw 1;
				generateAllSubgroups(order() / 2, vector[omp_get_thread_num()]);
			}

			generateCyclicSubgroups();
			Subgroup all;
			all.setElements(elements);
			subgroups.insert(all);
			subgroups.insert(Subgroup());
			return subgroups;
		}
	private:
		omp_lock_t writelock;
		void generateAllSubgroups(size_t maxSize, typename std::set<std::set<Element>> set) noexcept {
			std::cout << omp_get_thread_num() << '\n';
			FiniteGroup::Subgroup subgroup;
			for (const std::set<Element>& set2 : set)
				for (const Element& element : set2) {
					Subgroup tmp = subgroup;
					tmp.insertElement(element);
					if (tmp.elements.size() <= maxSize && tmp.valid(*this)) {
						if (insertSubgroup(tmp))
							generateAllSubgroups(tmp, maxSize, ++elements.find(element));
					}
				}
		}
		inline bool insertSubgroup(const Subgroup& subgroup) noexcept {
			omp_set_lock(&writelock);
			const bool inserted = subgroups.insert(subgroup).second;
			omp_unset_lock(&writelock);
			return inserted;
		};
		void generateAllSubgroups(Subgroup& subgroup, size_t maxSize, typename std::set<Element>::iterator itr) {
			//#pragma omp task
			{
				std::cout << omp_get_thread_num() << '\n';
				for (; itr != elements.end(); ++itr) {
					Subgroup tmp = subgroup;
					const Element element = *itr;
					tmp.insertElement(element);
					if (tmp.elements.size() <= maxSize && tmp.valid(*this) && insertSubgroup(tmp))
						generateAllSubgroups(tmp, maxSize, itr);
				}
			}
		}
	};
protected:
	Element identityElement;
	std::set<Element> elements;
	std::map<Element, Element> inverses;
	std::set<Subgroup> subgroups;
	void generateAllElements() noexcept { elements = identityElement.generateAllElements(); };
	bool insert(const Element& g1) noexcept { return elements.insert(g1).second; };
	inline bool insertSubgroup(const Subgroup& subgroup) noexcept {
		return subgroups.insert(subgroup).second;
	};

	void generateAllSubgroups(Subgroup& subgroup, size_t maxSize, typename std::set<Element>::iterator itr) {
		#pragma omp task
		{
			std::cout << omp_get_thread_num() << '\n';
			for (; itr != elements.end(); ++itr) {
				Subgroup tmp = subgroup;
				const Element element = *itr;
				tmp.insertElement(element);
				if (tmp.elements.size() <= maxSize && tmp.valid(*this) && insertSubgroup(tmp))
					generateAllSubgroups(tmp, maxSize, itr);
			}
		}
	}
};

*/