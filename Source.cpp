#include <iostream>

using namespace std;
// forward iterator  для односв списка; reverse iterator,or bidir;tie,concat

template<typename...>
class mytuple;
template<>
class mytuple<> {};
template<typename Head, typename... Tail>
class mytuple<Head, Tail...> : private mytuple<Tail...> {
public:
    using base_type = mytuple<Tail...>;
    using value_type = Head;
    mytuple(Head h, Tail... tail)
        : base_type(tail...), head_(h) {}


    template<typename... Other>
    mytuple& operator=(const mytuple<Other...>& other) {
        assign_impl<0>(*this, other);
        return *this;
    }

    base_type& base() { return *this; }
    const base_type& base() const { return *this; }

    Head& head() { return head_; }
    const Head& head() const { return head_; }

private:
    Head head_;
};

template<int I, typename Tuple>
struct getter;
template<int I, typename Head, typename... Tail>
struct getter<I, mytuple<Head, Tail...>> {

    static decltype(auto) get(mytuple<Head, Tail...>& t) {
        return getter<I - 1, mytuple<Tail...>>::get(t.base());
    }

    static decltype(auto) get(const mytuple<Head, Tail...>& t) {
        return getter<I - 1, mytuple<Tail...>>::get(t.base());
    }
};
template<typename Head, typename... Tail>
struct getter<0, mytuple<Head, Tail...>> {
    static Head& get(mytuple<Head, Tail...>& t) {
        return t.head();
    }

    static const Head& get(const mytuple<Head, Tail...>& t) {
        return t.head();
    }
};

template<int I, typename... Args>
decltype(auto) get(mytuple<Args...>& t) {
    return getter<I, mytuple<Args...>>::get(t);
}

template<int I, typename... Args>
decltype(auto) get(const mytuple<Args...>& t) {
    return getter<I, mytuple<Args...>>::get(t);
}

template<int I, typename Tuple1, typename Tuple2>
void assign_impl(Tuple1&, const Tuple2&) {}

template<int I, typename... T1, typename... T2>
void assign_impl(mytuple<T1...>& t1, const mytuple<T2...>& t2) {
    if constexpr (I < sizeof...(T1)) {
        get<I>(t1) = get<I>(t2);
        assign_impl<I + 1>(t1, t2);
    }
}




template<typename... Args>
mytuple<Args&...> tie_(Args&... args) {
    return mytuple<Args&...>(args...);

}











template<int... I>
struct idx_seq {};
template<int N, int... I>
struct make_idx_seq : make_idx_seq<N - 1, N - 1, I...> {};
template<int... I>
struct make_idx_seq<0, I...> {
    using type = idx_seq<I...>;
};
template<typename... A, typename... B, int... I, int... J>
auto concat_impl(const mytuple<A...>& t1, const mytuple<B...>& t2,
    idx_seq<I...>, idx_seq<J...>) {
    return mytuple<A..., B...>(
        get<I>(t1)...,
        get<J>(t2)...
    );
}
template<typename... A, typename... B>
auto concat(const mytuple<A...> t1, const mytuple<B...> t2) {
    return concat_impl(
        t1, t2,
        typename make_idx_seq<sizeof...(A)>::type{},
        typename make_idx_seq<sizeof...(B)>::type{}
    );
}
int main() {
    mytuple<int, int, int> t(12, 2, 89);
    mytuple<int, double, int>t2(12, 2.1, 5);
    int A = 1;
    double B = 2.3;
    int C = 3;

    //tie
    auto bib = tie_(A, B, C);
    tie_(A, B, C)=t2;
    A += 20;
    cout << get<0>(t) << " "
        << get<1>(t) << " "
        << get<2>(t) << endl;
    cout << "TIE:   " << get<0>(bib) << " " << get<1>(bib) << " " << get<2>(bib);
    cout << endl;
    //tie
    mytuple<mytuple<int,int,int>, double, int> b(t, 2.34, 89);
    cout << get<0>(get<0>(b));
    auto CONC = concat(t2, t);
    cout << endl;
    cout << get<5>(CONC);
    

    return 0;
}