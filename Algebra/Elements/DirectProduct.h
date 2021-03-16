#pragma once
#include "Element.h"
#include "Cyclic.h"

//template <typename ... MockElement>


/// <summary>
/// Represents a direct product
/// </summary>
/// <typeparam name="...MockElement">Ordered list of all types of elements to create the product from</typeparam>
template <typename ... MockElement>
struct DirectProduct : public Element {
	std::tuple<MockElement...> vals;
	typedef std::tuple<MockElement...> MockTuple;

	DirectProduct() : vals() {};
	DirectProduct inverse() const {
		DirectProduct inverse;
		DirectProduct thisCpy = *this;
		for_both(inverse.vals, thisCpy.vals,
			[&](auto&& e1, auto&& e2) {
				e1 = e2.inverse();
			}
		);
		return inverse;
	};

	std::set<DirectProduct> generateAllElements() {
		std::set<DirectProduct> result;
		loop<std::tuple_size<MockTuple>::value - 1>::crossProd(vals, result);
		return result;
	};

	DirectProduct operator+(const DirectProduct& other) const {
		DirectProduct result;
		DirectProduct thisCpy = *this;
		DirectProduct otherCpy = other;
		for_all(result.vals, thisCpy.vals, otherCpy.vals,
			[&](auto&& e1, auto&& e2, auto&& e3) {
				e1 = e2 + e3;
			}
		);
		return result;
	};

	void operator=(const DirectProduct& other) noexcept {
		DirectProduct otherCpy = other;
		for_both(vals, otherCpy.vals,
			[&](auto&& e1, auto&& e2) {
				e1 = e2;
			}
		);
	};

	friend bool operator==(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		int result = true;
		for_both(lhs.vals, rhs.vals,
			[&](auto&& e1, auto&& e2) {
				if (e1 != e2)
					result = false;
			}
		);
		return result;
	};

	friend bool operator!=(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		return !(lhs == rhs);
	};

	friend bool operator<(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		int result = -1;
		for_both(lhs.vals, rhs.vals,
			[&](auto&& e1, auto&& e2) {
				if (e1 != e2 && result == -1)
					result = e1 < e2;
			}
		);
		if (result == -1)
			result = false;
		return result;
	};

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
	};

private:
	template <int num>
	static void crossProduct(MockTuple& vals, std::set<DirectProduct>& result) {
		if (result.empty()) {
			for (const auto& e2 : std::get<num>(vals).generateAllElements()) {
				DirectProduct tmp;
				std::get<num>(tmp.vals) = e2;
				result.insert(tmp);
			}
		}
		else for (const DirectProduct& e1 : result)
			for (const auto& e2 : std::get<num>(vals).generateAllElements()) {
				DirectProduct tmp = e1;
				std::get<num>(tmp.vals) = e2;
				result.insert(tmp);
			}
	};

	template<std::size_t remaining>
	struct loop {
		static void crossProd(MockTuple& vals, std::set<DirectProduct>& result) {
			crossProduct<remaining>(vals, result);
			loop<remaining - 1>::crossProd(vals, result);
		}
	};

	template<>
	struct loop<-1> {
		static void crossProd(MockTuple& vals, std::set<DirectProduct>& result) {};
	};
};
