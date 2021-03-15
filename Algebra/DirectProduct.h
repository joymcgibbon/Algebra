#pragma once
#include "Element.h"
#include "Cyclic.h"
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

/*struct GroupTypes {
	template<class Ts> struct overload : Ts { using Ts::operator(); };
	template<class Ts> overload(Ts)->overload<Ts>;

	typedef std::variant<Zn::FullGroup, Sn> Groups;
	typedef std::vector<allTypesVar> DirectProductType;
	typedef std::variant<Zn::FullGroup::Element, Sn::Element> Elements;

	static_assert(std::is_same_v <Zn::FullGroup, std::variant_alternative_t<Zn::FullGroup::GROUP_TYPE, Groups>>);
	static_assert(std::is_same_v <Zn::FullGroup::Element, std::variant_alternative_t<Zn::FullGroup::GROUP_TYPE, Elements>>);

	static_assert(std::is_same_v <Sn, std::variant_alternative_t<Sn::GROUP_TYPE, Groups>>);
	static_assert(std::is_same_v <Sn::Element, std::variant_alternative_t<Sn::GROUP_TYPE, Elements>>);

	//static_assert(std::is_same_v <DirectProduct, std::variant_alternative_t<2, Groups>>);
	//static_assert(std::is_same_v <allTypesVar, std::variant_alternative_t<2, Elements>>);
};*/


enum Groups {

};

struct GeneralElement : public Element {

};
template <typename ... GeneralElement>
struct DirectProduct : public Element {
	std::tuple<GeneralElement...> vals;

	DirectProduct() : vals() {};

	template <typename Element>
	std::set<DirectProduct> crossProduct(std::set<DirectProduct> g1, std::set<Element> g2) {
		std::set<DirectProduct> result;
		if (g1.empty()) {
			for (const Element& e2 : g2) {
				DirectProduct tmp;
				std::get<Element>(tmp.vals) = e2;
				result.insert(tmp);
			}
			return result;
		}
		if (g2.empty())
			return g1;
		for (const DirectProduct& e1 : g1)
			for (const Element& e2 : g2) {
				DirectProduct tmp = e1;
				std::get<Element>(tmp.vals) = std::get<Element>(tmp.vals) + e2;
				result.insert(tmp);
			}
		return result;
	}

	DirectProduct identity() {
		return DirectProduct();
	};
	DirectProduct inverse() const {
		DirectProduct inverse;
		std::apply([&](auto element, auto...ops) {
			inverse = element.inverse();
			}, vals);
		return inverse;
	};
	std::set<DirectProduct> generateAllElements() {
		std::set<DirectProduct> result;
		//result.insert(identity());
		std::apply([&](auto element, auto...ops) {  result = crossProduct(result, element.generateAllElements()); }, vals);
		return result;
	};

	DirectProduct operator+(const DirectProduct& other) const {
		DirectProduct result;
		std::apply([&](auto element, auto...ops) { result += element; }, vals);
		return result;
	};

	template <typename Element>
	void operator+=(const Element& other) {
		std::get<Element>(vals) = std::get<Element>(vals) + other;
	};

	template <typename Element>
	void operator=(const Element& other) {
		std::get<Element>(vals) = other;
	};

	void operator=(const DirectProduct& other) noexcept { };
	//void operator=(GeneralElement& other) noexcept { };
	template <typename Element>
	friend bool operator==(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		bool result = true;
		std::apply([&](auto element, auto...ops) {
			result &= (element == std::get<Element>(vals));
		}, lhs.vals);
		return result;
	}

	friend bool operator!=(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		return !(lhs == rhs);
	}

	template <typename Element>AAA
	friend bool operator<(const DirectProduct& lhs, const DirectProduct& rhs) noexcept {
		bool result = false;
		std::apply([&](auto element, auto...ops) {
			result |= element < std::get<Element>(rhs.vals);
		}, lhs.vals);
		return result;
	}
	friend std::ostream& operator<<(std::ostream& out, const DirectProduct& element) {
		/*out << "{";
		bool first = true;
		for (int i = 0; i < element.val.size(); ++i) {
			if (first)
				first = false;
			else
				out << ',';
			out << element.val[i];
		}
		out << "}";*/
		return out;
	}

	//GeneralElement operator[](const int index) noexcept { return val[index]; };
	//const GeneralElement operator[](const int index) const noexcept { return val[index]; };
	//size_t size() const noexcept { return val.size(); }
	//GeneralElement operator[](const int& index) const noexcept { return val[index]; };
private:
	//GeneralElement& operator[](const int& index) noexcept { return val[index]; };
};



/*class DirectProduct : public FiniteGroup<std::vector<Element*>> {
public:
	DirectProduct() : {};
	template <class T>
	std::set<Element> crossProduct(std::set<Element> g1, std::set<T> g2);
	template <class T>
	void crossProduct(Group<T>& other) { this->elements = crossProduct(this->elements, other.getElements());}
	std::set<Element> generateAllElements() override;
	Element identity() const override;
	Element inverseHelper(const Element element) const;
	size_t getOrder() const override;
	Element add(const Element& g1, const Element& g2) const override;
	void print(std::ostream& out, const Element& element) const override;
	friend std::ostream& operator<<(std::ostream& out, DirectProduct& group) {
		out << "\\{";
		bool first = true;
		for (Element element : group.getElements()) {
			if (first)
				first = false;
			else
				out << ',';
			group.print(out, element);
		}
		out << "\\}";
		return out;
	}
private:
	std::vector<GroupTypes::Groups> groups;
};*/

