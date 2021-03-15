#pragma once
#include "Element.h"
#include "Cyclic.h"
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

struct GeneralElement : public Element {

};

template <typename ... GeneralElement>
struct DirectProduct : public Element {
	std::tuple<GeneralElement...> vals;
	typedef std::tuple<GeneralElement...> TupleType;
	DirectProduct() : vals() {};
	DirectProduct inverse() const {
		DirectProduct inverse;
		DirectProduct thisCpy = *this;
		for_each2(inverse.vals, thisCpy.vals,
			[&](auto&& e1, auto&& e2) {
				e1 = e2.inverse();
			}
		);
		return inverse;
	};

	std::set<DirectProduct> generateAllElements() {
		std::set<DirectProduct> result;
		loopTuple<std::tuple_size<TupleType>::value - 1>::crossProd(vals, result);
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
		for_each2(vals, otherCpy.vals,
			[&](auto&& e1, auto&& e2) {
				e1 = e2;
			}
		);
	};

	friend bool operator==(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		int result = true;
		for_each2(lhs.vals, rhs.vals,
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
		for_each2(lhs.vals, rhs.vals,
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
private:
	template <int num>
	static void crossProduct(TupleType& vals, std::set<DirectProduct>& result) {
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
	}

	template<std::size_t remaining>
	struct loopTuple {
		static void crossProd(TupleType& vals, std::set<DirectProduct>& result) {
			crossProduct<remaining>(vals, result);
			loopTuple<remaining - 1>::crossProd(vals, result);
		}
	};
	template<>
	struct loopTuple<-1> {
		static void crossProd(TupleType& vals, std::set<DirectProduct>& result) {};
	};
};
