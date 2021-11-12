template<typename T, typename K>
struct Pair {
public:
    T first;
    K second;
public:
    Pair(T f, K s) {
        first = f;
        second = s;
    }
};