#pragma once
#include "Element.h"
#include "Cyclic.h"
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

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
		DirectProduct thisCpy = *this;
		for_each(inverse.vals, thisCpy.vals,
			[&](auto&& e1, auto&& e2) {
				e1 = e2.inverse();
			}
		);
		return inverse;
	};
	std::set<DirectProduct> generateAllElements() {
		std::set<DirectProduct> result;
		for_each(vals,
			[&](auto&& e1) {
				result = crossProduct(result, e1.generateAllElements());
			}
		);
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

	void operator=(const DirectProduct& other) noexcept { 
		DirectProduct otherCpy = other;
		for_each(vals, otherCpy.vals,
			[&](auto&& e1, auto&& e2) {
				e1 = e2;
			}
		);
	};

	friend bool operator==(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		int result = true;
		for_each(lhs.vals, rhs.vals,
			[&](auto&& e1, auto&& e2) {
				if (e1 != e2)
					result = false;
			}
		);
		return result;
	}

	friend bool operator!=(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		return !(lhs == rhs);
	}

	friend bool operator<(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		int result = -1;
		for_each(lhs.vals, rhs.vals,
			[&](auto&& e1, auto&& e2) {
				if (e1 != e2 && result == -1)
					result = e1 < e2;
			}
		);
		if (result == -1)
			result = false;
		return result;
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
