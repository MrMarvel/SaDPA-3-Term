template<typename T, typename K>
struct Pair {
    T first;
    K second;
    Pair(T f, K s) {
        first = f;
        second = s;
    }
};