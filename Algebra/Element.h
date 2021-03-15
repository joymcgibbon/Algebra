#pragma once
#include "SetOperations.h"
#include <iostream>
/// <summary>
/// Base class for all elements that can be stored in groups. Creates a valid group containing only the empty set
/// </summary>
struct Element {
	/// <summary>
	/// Produces the identity element of the group
	/// </summary>
	Element() {};
	/// <summary>
	/// Produces an element that when added to this element gives the identity. Not used or checked by FiniteGroup
	/// </summary>
	/// <returns>The element that will give the identity added to this element</returns>
	Element inverse() const noexcept { return {}; };
	/// <summary>
	/// Creates all possible elements of the group
	/// </summary>
	/// <returns>A set of elements in the full group</returns>
	std::set<Element> generateAllElements() { return { {} }; };
	/// <summary>
	/// Adds one element to another
	/// </summary>
	/// <param name="other"></param>
	/// <returns>Result of adding elements</returns>
	virtual Element operator+(const Element& other) const noexcept { return {}; };
	/// <summary>
	/// Sets one element equal to another
	/// </summary>
	/// <param name="other"></param>
	void operator=(const Element& other) noexcept { };
	/// <summary>
	/// Determines if elements are equal
	/// </summary>
	/// <param name="lhs">First element to compare</param>
	/// <param name="rhs">Second element to compare</param>
	/// <returns>Result of equality</returns>
	friend bool operator==(const Element& lhs, const Element& rhs) noexcept { return false; }
	/// <summary>
	/// Determines if elements are not equal
	/// </summary>
	/// <param name="lhs">First element to compare</param>
	/// <param name="rhs">Second element to compare</param>
	/// <returns>Result of inequality</returns>
	friend bool operator!=(const Element& lhs, const Element& rhs) noexcept { return false; }
	/// <summary>
	/// Produces a unique ordering for elements. 
	/// The identity element must be the smallest element.
	/// </summary>
	/// <param name="lhs">First element to compare</param>
	/// <param name="rhs">Second element to compare</param>
	/// <returns>Result of comparison</returns>
	friend bool operator<(const Element& lhs, const Element& rhs) noexcept { return false; }
	/// <summary>
	/// Display the element
	/// </summary>
	friend std::ostream& operator<<(std::ostream& out, const Element& element) {
		out << "{}";
		return out;
	};
};