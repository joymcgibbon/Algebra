#pragma warning( disable : 4005 )
#pragma once
#include "DirectProduct.h"
#include "Group.h"
#include "Cyclic.h"
#include "Abelian.h"
#include <iostream>

typedef FiniteGroup<DirectProduct<Cyclic<2>, Abelian<2>>> TYPE;
void printCosets(TYPE group, bool printElements = false, bool printCosets = true);
void getAndPrintCosets(FiniteGroup<TYPE::Element>* group, TYPE::Subgroup subgroup, bool printElements = false, bool printCosets = true, bool printIfNotNormal = true);
void printInverses(TYPE group);

#include <omp.h>

int FibHelp(int n);
int SerialFib(int n);

// Creates parallel section and calls FibHelp to calculate Fibonnaci number
int OpenMPFib(int n) {
	int res;

#pragma omp parallel 
#pragma omp single  
	res = FibHelp(n);

	return res;
}

// Helper function to create tasks to calculate fibonacci numbers
int FibHelp(int n) {
	if (n < 18)
		return SerialFib(n);

	int n1, n2;

#pragma omp task shared(n1) 
	{
		n1 = FibHelp(n - 1);
		std::cout << omp_get_thread_num() << '\n';
	}

	n2 = FibHelp(n - 2);

//#pragma omp taskwait
	return n1 + n2;
}

// Serial version of Fib to handle small n efficiently
int SerialFib(int n) {
	if (n < 2) {
		return n;
	}
	else {
		return SerialFib(n - 1) + SerialFib(n - 2);
	}
}
int main() {
	//FibHelp(100);
	printCosets(TYPE(), false, false);
}

void printCosets(TYPE group, bool printElements, bool printCosets) {
	time_t start;
	time_t end;
	time(&start);
	auto subgroups = group.generateAllSubgroups();
	time(&end);

	time_t time = end - start;
	time_t minutes = (int)(time / 60);
	const time_t seconds = time - (minutes * 60);
	std::cout << "\nTime: " << minutes << ':' << (seconds > 9? "" :"0") << seconds;

	std::cout << "----------------------------------" << subgroups.size() << "----------------------------------\n";
	/*
	for (TYPE::Subgroup subgroup : subgroups) {
		getAndPrintCosets(&group, subgroup, printElements, printCosets);
	}
	std::cout << "----------------------------------" << subgroups.size() << "----------------------------------\n";

	if (group.getElements().size() != group.order())
		std::cout << "---------------------------------------FAIL---------------------------------------";

	std::cout << "------------------------------------------\n";
	std::cout << group << '\n';*/
}

void getAndPrintCosets(TYPE* group, TYPE::Subgroup subgroup, bool printElements, bool printCosets, bool printIfNotNormal) {
	if (!subgroup.valid(*group))
		std::cout << "Invalid subgroup \n";
	auto cosets = group->getLeftCosets(subgroup);
	auto cosetElements = cosets;
	auto rightCosets = group->getRightCosets(subgroup);
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
	for (const TYPE::Element& gen : subgroup.getGenerators()) {
		if (first)
			first = false;
		else
			std::cout << ',';
		std::cout << gen;
	}
	std::cout << ">= \t";
	if (!printElements)
		std::cout << subgroup.getElements().size();
	else
		std::cout << subgroup;
	std::cout << "\n";

	if (printCosets) {
		//for (int i = 0; i < 24; ++i)
		for (TYPE::Subgroup coset : cosets) {
			//if (coset.getGenerators()[0] == i) {
			std::cout << "<";
			std::cout << coset.getGenerators()[0];
			std::cout << ">=" << coset << "\n";
			std::cout << "\n";
		}
		std::cout << cosets.size() << "\n";
	}
}

void printInverses(TYPE group) {
	for (const auto& element : group.getInverses()) {
		std::cout << element.first;
		std::cout << "^-1 = ";
		std::cout << element.second;
		std::cout << "\n";

	}
}
