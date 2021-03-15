#pragma once
#include "SetOperations.h"
#include <iostream>

struct Element {
	Element() {};
	 Element operator&()  const noexcept { return Element(); };
	 Element inverse() const noexcept { return {}; };
	 Element identity() const noexcept { return {}; };
	 std::set<Element> generateAllElements() { return {}; };
	 std::set<Element*> generateAllElementPtrs() { return {}; };
	 Element operator+(const Element& other) const noexcept { return {}; };
	void operator=(const Element& other) noexcept { };
};