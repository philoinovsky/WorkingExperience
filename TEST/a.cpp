template<typename T>
T f(T param) {
    return param;
}


void main() {
    const int * const p = new int(1);
    auto a = f(p);
}