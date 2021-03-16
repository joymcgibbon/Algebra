#include <set>

// copied from https://blog.tartanllama.xyz/exploding-tuples-fold-expressions/ 
template <size_t... Idx>
auto make_index_dispatcher(std::index_sequence<Idx...>) {
	return [](auto&& f) { (f(std::integral_constant<size_t, Idx>{}), ...); };
}

template <size_t N>
auto make_index_dispatcher() {
	return make_index_dispatcher(std::make_index_sequence<N>{});
}

template <typename Tuple, typename Func>
void for_each(Tuple&& t, Func&& f) {
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = make_index_dispatcher<n>();
	dispatcher([&f, &t](auto idx) { f(std::get<idx>(std::forward<Tuple>(t))); });
}

// modified for 2 tuples
template <typename Tuple, typename Func>
void for_both(Tuple&& t1, Tuple&& t2, Func&& f) {
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = make_index_dispatcher<n>();
	dispatcher([&f, &t1, &t2](auto idx) { f(std::get<idx>(std::forward<Tuple>(t1)), std::get<idx>(std::forward<Tuple>(t2))); });
}

// modified for 3 tuples
template <typename Tuple, typename Func>
void for_all(Tuple&& t1, Tuple&& t2, Tuple&& t3, Func&& f) {
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = make_index_dispatcher<n>();
	dispatcher([&f, &t1, &t2, &t3](auto idx) { f(
		std::get<idx>(std::forward<Tuple>(t1)),
		std::get<idx>(std::forward<Tuple>(t2)),
		std::get<idx>(std::forward<Tuple>(t3))
	); });
}