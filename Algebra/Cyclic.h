#pragma once
#include "Group.h"

#ifdef DYNAMIC_TYPE
struct Cyclic : public Element {
std::vector<int> val;
const size_t num;
Cyclic(size_t num) noexcept :num(num) {
	val.resize(num);
	for (int i = 0; i < num; ++i)
		val[i] = i + 1;
};
Cyclic operator&() const noexcept { return Cyclic(num); };
#else
template <size_t num>
struct Cyclic : public Element {
	int val[num];
	Cyclic() noexcept {
		for (int i = 0; i < num; ++i)
			val[i] = i + 1;
	};
	Cyclic operator&() const noexcept { return Cyclic(num); };
#endif
	/*Cyclic inverse() const noexcept {
		Cyclic result;
		for (int index = 0; index < num; ++index)
			result[index] = val[val[index] - 1];
		return result;
	};*/
	std::set<Cyclic> generateAllElements() {
		std::set<Cyclic> result = { Cyclic() };
		permute(result, Cyclic(), 0, num - 1);
		return result;
	};
	Cyclic operator+(const Cyclic& other) const noexcept {
		Cyclic result;
		for (int index = 0; index < num; ++index)
			result[index] = other[val[index] - 1];
		return result;
	};
	void operator=(const Cyclic& other) noexcept {
		for (int index = 0; index < num; ++index)
			val[index] = other.val[index];
	};
	friend bool operator==(const Cyclic& lhs, const Cyclic& rhs) noexcept {
		for (int index = 0; index < num; ++index)
			if (lhs[index] != rhs[index])
				return false;
		return true;
	};
	friend bool operator!=(const Cyclic& lhs, const Cyclic& rhs) noexcept {
		return !(lhs == rhs);
	};
	friend bool operator<(const Cyclic& lhs, const Cyclic& rhs) noexcept {
		for (int index = num - 1; index >= 0; --index)
			if (lhs[index] != rhs[index])
				return lhs[index] > rhs[index];
		return false;
	};
	inline int getResult(const int& num) const noexcept { return val[num - 1]; }
	friend std::ostream& operator<<(std::ostream& out, const Cyclic& element) {
		for (std::vector<int> val : getCyclicRep(element)) {
			out << "(";
			for (int v : val)
				out << v;
			out << ")";
		}

		/*for (int index = 0; index < num; ++index)
			out << element.val[index];*/

		return out;
	};

private:
	static void permute(std::set<Cyclic>& result, Cyclic element, int idxStart, int idxEnd) {
		if (idxStart == idxEnd)
			result.insert(element);
		else
			for (int index = idxStart; index < num; ++index) {
				Cyclic tmp = element;
				std::swap(tmp[idxStart], tmp[index]);
				permute(result, tmp, idxStart + 1, idxEnd);
				std::swap(tmp[idxStart], tmp[index]);
			}
	};
	static std::vector<std::vector<int>> getCyclicRep(const Cyclic& element) {
		std::vector<std::vector<int>> result;
		std::vector<int> tmpResult;
		std::set<int> numsProcessed;
		int tmp = 1;
		do {
			tmpResult.push_back(tmp);
			numsProcessed.insert(tmp);
			int prevNum = tmp;
			tmp = element.getResult(tmp);
			if (tmp == tmpResult[0]) {
				if (tmpResult.size() > 1)
					result.push_back(tmpResult);
				tmpResult.clear();
				tmp = 2;
				while (numsProcessed.find(tmp) != numsProcessed.end())
					++tmp;
			}
		} while (tmp <= num);

		if (tmpResult.size() > 1)
			result.push_back(tmpResult);
		else if (result.empty())
			result = { {1} };

		return result;
	};
	int operator[](const int& index) const noexcept { return val[index]; };
private:
	int& operator[](const int& index) noexcept { return val[index]; };
};