#pragma once
#include "SetOperations.h"
#include <iostream>

struct Element {
	Element() {};
	Element operator&()  const noexcept { return Element(); };
	Element inverse() const noexcept { return {}; };
	Element identity() const noexcept { return {}; };
	std::set<Element> generateAllElements() { return {}; };
	Element operator+(const Element& other) const noexcept { return {}; };
	void operator=(const Element& other) noexcept { };
	friend bool operator==(const Element& lhs, const Element& rhs) noexcept { return false; }
	friend bool operator!=(const Element& lhs, const Element& rhs) noexcept { return false; }
	friend bool operator<(const Element& lhs, const Element& rhs) noexcept { return false; }
};