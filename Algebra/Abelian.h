#pragma once
#include "Group.h"
#include <algorithm> 
namespace Abelian {
	template <size_t num>
	struct Additive : public Element {
		int val;
		Additive() { val = 0; };
		Additive(size_t val) : val(val% num) {};
		Additive inverse() const { return Additive(num - val); };
		static std::set<Additive> generateAllElements() {
			std::set<Additive> result = { Additive() };
			for (size_t i = 1; i < num; ++i)
				result.insert({ i });
			return result;
		};
		virtual Additive operator+(const Additive& other) const { return Additive(val + other.val); };
		void operator=(const Additive& other) noexcept { val = other.val; };
		friend bool operator==(const Additive& lhs, const Additive& rhs) noexcept { return lhs.val == rhs.val; }
		friend bool operator!=(const Additive& lhs, const Additive& rhs) noexcept { return lhs.val != rhs.val; }
		friend bool operator<(const Additive& lhs, const Additive& rhs) noexcept { return lhs.val < rhs.val; }
		friend std::ostream& operator<<(std::ostream& out, const Additive& element) {
			out << '[' << element.val << ']';
			return out;
		};
	};

	template <size_t num>
	struct Multiplicitive : public Element {
		int val;
		Multiplicitive() { val = 1; };
		Multiplicitive(size_t val) : val(val% num) {};
		Multiplicitive inverse() const {
			unsigned int inverseVal;
			for (inverseVal = 1; (inverseVal * val) % num != 1; ++inverseVal);
			return { inverseVal };
		};
		static std::set<Multiplicitive> generateAllElements() {
			std::set<Multiplicitive> result = { Multiplicitive() };
			std::set<int> factors = operation::getFactors(num);
			for (size_t i = 2; i < num; ++i) {
				std::set<int> factorsOfIndex = operation::getFactors(i);
				std::vector<int> commonFactors(10);
				std::vector<int>::iterator it = std::set_intersection(factors.begin(), factors.end(), factorsOfIndex.begin(), factorsOfIndex.end(), commonFactors.begin());
				commonFactors.resize(it - commonFactors.begin());
				if (commonFactors.size() == 1)
					result.insert({ i });
			}
			return result;
		};
		Multiplicitive operator+(const Multiplicitive& other) const { return Multiplicitive(val * other.val); };
		friend bool operator==(const Multiplicitive& lhs, const Multiplicitive& rhs) noexcept { return lhs.val == rhs.val; }
		friend bool operator!=(const Multiplicitive& lhs, const Multiplicitive& rhs) noexcept { return lhs.val != rhs.val; }
		friend bool operator<(const Multiplicitive& lhs, const Multiplicitive& rhs) noexcept { return lhs.val < rhs.val; }
		friend std::ostream& operator<<(std::ostream& out, const Multiplicitive& element) {
			out << '[' << element.val << ']';
			return out;
		};
	};
}