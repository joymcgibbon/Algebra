#pragma once
#include "Group.h"
#include <algorithm> 

template <size_t num>
struct Abelian : public Element {
	//using value_type = size_t;
	size_t val;
	Abelian() { val = 0; };
	Abelian(size_t val) : val(val% num) {};
	Abelian identity() { return Abelian(); };
	Abelian inverse() const { return Abelian(num - val); };
	std::set<Abelian> generateAllElements() {
		std::set<Abelian> result = { identity() };
		for (size_t i = 1; i < num; ++i)
			result.insert({ i });
		return result;
	};
	virtual Abelian operator+(const Abelian& other) const { return Abelian(val + other.val); };
	void operator=(const Abelian& other) noexcept { val = other.val; };
	friend bool operator==(const Abelian& lhs, const Abelian& rhs) noexcept { return lhs.val == rhs.val; }
	friend bool operator!=(const Abelian& lhs, const Abelian& rhs) noexcept { return lhs.val != rhs.val; }
	friend bool operator<(const Abelian& lhs, const Abelian& rhs) noexcept { return lhs.val < rhs.val; }
	friend std::ostream& operator<<(std::ostream& out, const Abelian& element) {
		out << '[' << element.val << ']';
		return out;
	};
};

template <size_t num>
struct AbelianMult : public Abelian<num> {
	//using value_type = size_t;
	size_t val;
	AbelianMult() { val = 1; };
	AbelianMult(size_t val) : val(val% num) {};
	AbelianMult identity() { return AbelianMult(); };
	AbelianMult inverse() const {
		size_t inverseVal;
		for (inverseVal = 1; (inverseVal * val) % num != 1; ++inverseVal);
		return { inverseVal };
	};
	std::set<AbelianMult> generateAllElements() {
		std::set<AbelianMult> result = { identity() };
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
	AbelianMult operator+(const AbelianMult& other) const { return AbelianMult(val * other.val); };
	friend bool operator==(const AbelianMult& lhs, const AbelianMult& rhs) noexcept { return lhs.val == rhs.val; }
	friend bool operator!=(const AbelianMult& lhs, const AbelianMult& rhs) noexcept { return lhs.val != rhs.val; }
	friend bool operator<(const AbelianMult& lhs, const AbelianMult& rhs) noexcept { return lhs.val < rhs.val; }
	friend std::ostream& operator<<(std::ostream& out, const AbelianMult& element) {
		out << '[' << element.val << ']';
		return out;
	};
};