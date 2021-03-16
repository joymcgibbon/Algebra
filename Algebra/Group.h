#pragma once
#include <iostream>
#include <map>
#include <omp.h>
#include "SetOperations.h"
#include "Element.h"
#include <thread>
#include <mutex>

/// <summary>
/// A representation of a finite group that can generate subgroups and cosets from any Element
/// </summary>
/// <typeparam name="Element">The type of element this group operates on</typeparam>
template <typename Element>
class FiniteGroup {
public:
	typedef Element Element;
	friend struct GroupOperations;

	const Element identity;
	FiniteGroup() : identity(Element()) { insert(identity); };
	FiniteGroup(const FiniteGroup& group) : identity(Element()) {
		elements = group.elements;
	};
	std::set<Element> getElements() const noexcept {
		return elements;
	};
	size_t order() const noexcept { return elements.size(); };

	void insertElement(const Element& element) {
		const size_t tmpSize = order();
		for (const Element& original : elements) {
			Element tmp = element;
			int count = 0;
			while (tmp != identity) {
				const Element result = original + tmp;
				insert(result);
				insert(tmp + original);
				tmp = tmp + element;
				insert(tmp);
			}
		}
		if (tmpSize != order())
			generators.push_back(element);
	}
	bool validSubgroup(FiniteGroup group) const {
		if (empty() || group.order() % order() != 0)
			return false;
		for (const Element& a : elements)
			for (const Element& b : elements) {
				if (elements.find(a + b.inverse()) == elements.end())
					return false;
			}
		return true;
	};
	bool empty() const noexcept { return elements.empty(); };
	void generateAllElements() {
		elements = identity.generateAllElements();
#ifdef _DEBUG
		if (empty())
			throw std::invalid_argument("Group is not valid: Elements generated must not be empty.");
		if (!validIdentity())
			throw std::invalid_argument("Group is not valid: Identity element added to any element does not give that element with the operation +.");
		if (!closed())
			throw std::invalid_argument("Group is not valid: Elements generated are not all possible with operation +.");
		if (!validInverses())
			throw std::invalid_argument("Group is not valid: Elements added to their inverse do not give the identity with the operation +.");
		if (!associative())
			throw std::invalid_argument("Group is not valid: Operation + is not associative.");
#endif
	};
	bool validIdentity() const {
		for (const Element& element : elements)
			if (element + identity != element || identity + element != element)
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
		for (const Element& element : elements)
			if (element + element.inverse() != identity)
				return false;
		return true;
	};
	bool associative() const {
		for (const Element& a : elements)
			for (const Element& b : elements)
				for (const Element& c : elements)
					if ((a + b) + c != a + (b + c))
						return false;
		return true;
	};
	friend bool operator<(const FiniteGroup& lhs, const FiniteGroup& rhs) noexcept {
		if (lhs.order() != rhs.order())
			return lhs.order() < rhs.order();
		auto itr2 = rhs.elements.begin();
		for (auto itr1 = lhs.elements.begin(); itr1 != lhs.elements.end(); ++itr1, ++itr2)
			if (*itr1 != *itr2)
				return (*itr1 < *itr2);
		return false;
	}
	friend bool operator==(const FiniteGroup& lhs, const FiniteGroup& rhs) noexcept {
		if (lhs.order() != rhs.order())
			return false;
		auto itr2 = rhs.elements.begin();
		for (auto itr1 = lhs.elements.begin(); itr1 != lhs.elements.end(); ++itr1, ++itr2)
			if (*itr1 != *itr2)
				return false;
		return true;
	}
	friend bool operator<=(const FiniteGroup& subgroup, const FiniteGroup& group) noexcept {
		return subgroup.validSubgroup(group);
	}
	friend std::ostream& operator<<(std::ostream& out, FiniteGroup& group) {
		out << "{";
		bool first = true;
		for (const Element& element : group.elements) {
			if (first)
				first = false;
			else
				out << ',';
			out << element;
		}
		out << "}";
		return out;
	}
	void insertGenerator(Element generator) { generators.push_back(generator); }
	std::vector<Element> getGenerators() {
		if (generators.size() > 1 && generators[0] == identity)
			generators.erase(generators.begin());
		return generators;
	};
private:
	std::set<Element> elements;
	std::vector<Element> generators;
	inline bool insert(const Element& g1) noexcept { return elements.insert(g1).second; };
};

