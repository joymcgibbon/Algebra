#pragma once
#include "Group.h"

template <size_t num>
struct Abelian : public Element {
	//using value_type = size_t;
	size_t val;
	Abelian() { val = 0; };
	Abelian(size_t val) : val(val% num) {};
	Abelian operator&() const noexcept { return Abelian(); };
	Abelian identity() { return 0; };
	Abelian inverse() const { return Abelian(num - val); };
	std::set<Abelian> generateAllElements() {
		std::set<Abelian> result = { identity() };
		for (size_t i = 0; i < num; ++i)
			result.insert({ i });
		return result;
	};
	static std::set<Element*> generateAllElementPtrs() {
		std::set<Element*> result = { &identity() };
		for (size_t i = 0; i < num; ++i)
			result.insert(&Abelian{ i });
		return result;
	};
	Abelian operator+(const Abelian& other) const { return Abelian(val + other.val); };
	void operator=(const Abelian& other) noexcept { val = other.val; };
	friend bool operator==(const Abelian& lhs, const Abelian& rhs) noexcept { return lhs.val == rhs.val; }
	friend bool operator!=(const Abelian& lhs, const Abelian& rhs) noexcept { return lhs.val != rhs.val; }
	friend bool operator<(const Abelian& lhs, const Abelian& rhs) noexcept { return lhs.val < rhs.val; }
	friend std::ostream& operator<<(std::ostream& out, const Abelian& element) {
		out << '[' << element.val << ']';
		return out;
	};
};