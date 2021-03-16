#pragma warning( disable : 4005 )
#pragma once
#include "DirectProduct.h"
#include "Group.h"
#include "Cyclic.h"
#include "Abelian.h"
#include "GroupOperations.h"
#include <iostream>

template <typename FiniteGroup>
void printCosets(FiniteGroup group, bool printElements = false, bool printCosets = true);
template <typename FiniteGroup>
void getAndPrintCosets(FiniteGroup group, FiniteGroup subgroup, bool printElements = false, bool printCosets = true, bool printIfNotNormal = true);
template <typename FiniteGroup>
void printInverses(FiniteGroup group);

int main() {
	try {
		{
			FiniteGroup<Abelian::Additive<4>> group1;
			group1.generateAllElements();
			//if (group.order() <= 120)
			std::cout << group1 << '\n';
			std::cout << "\n\n";
			for (const auto& element : GroupOperations::generators(group1)) {
				std::cout << element << "\n";
			}
			printCosets(group1, false, false);
			printTable(std::cout, GroupOperations::cayleyTable(group1));
		}
		{
			FiniteGroup<Abelian::Multiplicitive<5>> group1;
			group1.generateAllElements();
			//if (group.order() <= 120)
			std::cout << group1 << '\n';
			std::cout << "\n\n";
			for (const auto& element : GroupOperations::generators(group1)) {
				std::cout << element << "\n";
			}
			printCosets(group1, false, false);
			printTable(std::cout, GroupOperations::cayleyTable(group1));
		}
		//FiniteGroup<Abelian::Multiplicitive<42>> group;
		//group.generateAllElements();
		//if (group.order() <= 120)
		//std::cout << group << '\n';

		//FiniteGroup<Abelian::Multiplicitive<45>> group2;
		//group2.generateAllElements();
		//if (group.order() <= 120)
		//std::cout << group2 << '\n';
		//printCosets(TYPE(), false, false);


	}
	catch (std::invalid_argument e) {
		std::cout << e.what();
	}
}

template <typename FiniteGroup>
void printCosets(FiniteGroup group, bool printElements, bool printCosets) {
	/*time_t start;
	time_t end;
	time(&start);
	auto subgroups = group.generateAllSubgroups();
	time(&end);

	time_t time = end - start;
	time_t minutes = (int)(time / 60);
	const time_t seconds = time - (minutes * 60);

	std::cout << group << '\n';
	for (auto subgroup : subgroups) {
		std::cout << '<';
		for (auto generator : subgroup.getGenerators()) {
			std::cout << generator << ",";
		}
			std::cout << ">\t" << subgroup.order() << "\n";
	}

	std::cout << "\nTime: " << minutes << ':' << (seconds > 9 ? "" : "0") << seconds;
	std::cout << "----------------------------------" << subgroups.size() << "----------------------------------\n";*/
	
	/*for (FiniteGroup::Subgroup subgroup : subgroups) {
		getAndPrintCosets(&group, subgroup, printElements, printCosets);
	}
	std::cout << "----------------------------------" << subgroups.size() << "----------------------------------\n";

	if (group.getElements().size() != group.order())
		std::cout << "---------------------------------------FAIL---------------------------------------";

	std::cout << "------------------------------------------\n";
	std::cout << group << '\n';*/
}

template <typename FiniteGroup>
void getAndPrintCosets(FiniteGroup group, FiniteGroup subgroup, bool printElements, bool printCosets, bool printIfNotNormal) {
	if (!subgroup.validSubgroup(group))
		std::cout << "Invalid subgroup \n";
	auto cosets = GroupOperations::leftCosets(group, subgroup);
	auto cosetElements = cosets;
	auto rightCosets = GroupOperations::rightCosets(group, subgroup);
	bool equal = true;
	for (auto rightCoset : rightCosets)
		if (std::find(cosetElements.begin(), cosetElements.end(), rightCoset) == cosetElements.end())
			equal = false;
	if (!equal && !printIfNotNormal) {
		std::cout << "Not normal \n";
		return;
	}
	std::cout << "<";
	bool first = true;
	for (const FiniteGroup::Element& gen : subgroup.getGenerators()) {
		if (first)
			first = false;
		else
			std::cout << ',';
		std::cout << gen;
	}
	std::cout << ">= \t";
	if (!printElements)
		std::cout << subgroup.order();
	else
		std::cout << subgroup;
	std::cout << "\n";

	if (printCosets) {
		//for (int i = 0; i < 24; ++i)
		for (FiniteGroup coset : cosets) {
			//if (coset.getGenerators()[0] == i) {
			std::cout << "<";
			std::cout << coset.getGenerators()[0];
			std::cout << ">=" << coset << "\n";
			std::cout << "\n";
		}
		std::cout << cosets.size() << "\n";
	}
}

template <typename FiniteGroup>
void printInverses(FiniteGroup group) {
	for (const auto& element : GroupOperations::getInverses(group)) {
		std::cout << element.first;
		std::cout << "^-1=\t";
		std::cout << element.second << "\t" << element.first.inverse();
		std::cout << "\n";
	}
}
