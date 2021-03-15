#pragma once
#include "Element.h"
#include "Cyclic.h"
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

/*struct GroupTypes {
	template<class Ts> struct overload : Ts { using Ts::operator(); };
	template<class Ts> overload(Ts)->overload<Ts>;

	typedef std::variant<Zn::FullGroup, Sn> Groups;
	typedef std::vector<allTypesVar> DirectProductType;
	typedef std::variant<Zn::FullGroup::Element, Sn::Element> Elements;

	static_assert(std::is_same_v <Zn::FullGroup, std::variant_alternative_t<Zn::FullGroup::GROUP_TYPE, Groups>>);
	static_assert(std::is_same_v <Zn::FullGroup::Element, std::variant_alternative_t<Zn::FullGroup::GROUP_TYPE, Elements>>);

	static_assert(std::is_same_v <Sn, std::variant_alternative_t<Sn::GROUP_TYPE, Groups>>);
	static_assert(std::is_same_v <Sn::Element, std::variant_alternative_t<Sn::GROUP_TYPE, Elements>>);

	//static_assert(std::is_same_v <DirectProduct, std::variant_alternative_t<2, Groups>>);
	//static_assert(std::is_same_v <allTypesVar, std::variant_alternative_t<2, Elements>>);
};*/


enum Groups {

};

struct GeneralElement : public Element {

};

template <typename ... GeneralElement>
struct DirectProduct : public Element {
	std::tuple<GeneralElement...> vals;

	DirectProduct() : vals() {};

	template <typename Element>
	std::set<DirectProduct> crossProduct(std::set<DirectProduct> g1, std::set<Element> g2) {
		std::set<DirectProduct> result;
		if (g1.empty()) {
			for (const Element& e2 : g2) {
				DirectProduct tmp;
				std::get<Element>(tmp.vals) = e2;
				result.insert(tmp);
			}
			return result;
		}
		if (g2.empty())
			return g1;
		for (const DirectProduct& e1 : g1)
			for (const Element& e2 : g2) {
				DirectProduct tmp = e1;
				std::get<Element>(tmp.vals) = std::get<Element>(tmp.vals) + e2;
				result.insert(tmp);
			}
		return result;
	}

	DirectProduct identity() {
		return DirectProduct();
	};
	DirectProduct inverse() const {
		DirectProduct inverse;
		std::apply([&](auto element, auto...ops) {
			std::get<0>(inverse.vals) = element;
			}, vals);
		return inverse;
	};
	std::set<DirectProduct> generateAllElements() {
		std::set<DirectProduct> result;
		std::apply([&](auto&& element, auto...ops) {  result = crossProduct(result, element.generateAllElements()); }, vals);
		return result;
	};

	DirectProduct operator+(const DirectProduct& other) const {
		DirectProduct result;
		DirectProduct thisCpy = *this;
		DirectProduct otherCpy = other;
		for_each(result.vals, thisCpy.vals, otherCpy.vals,
			[&](auto&& e1, auto&& e2, auto&& e3) {
				e1 = e2 + e3;
			}
		);
		return result;
	};

	void operator=(const DirectProduct& other) noexcept { };
	//void operator=(GeneralElement& other) noexcept { };
	template <typename Element>
	friend bool operator==(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		bool result = true;
		for_each(lhs.vals, rhs.vals,
			[](auto&& e1, auto&& e2) {
				if (e1 != e2)
					return false;
			}
		);

		return true;
	}

	friend bool operator!=(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		return !(lhs == rhs);
	}

	friend bool operator<(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		for_each(lhs.vals, rhs.vals,
			[&](auto&& e1, auto&& e2) {
				if (e1 != e2)
					return e1 < e2;
			}
		);
		return false;
	}

	friend std::ostream& operator<<(std::ostream& out, const DirectProduct& element) {
		out << "(";
		bool first = true;
		for_each(element.vals,
			[&](auto&& e) { 
				if (first)
					first = false;
				else
					out << ',';
				out << e;
			}
		);

		std::cout << ')';
		return out;
	}
};
