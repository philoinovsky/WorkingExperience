#include <utility>
using namespace std;

template<typename T>
T f(T param) {
    return param;
}

void main() {
    auto b = forward<int&>(int(1)); // error, must be lvalue
    const int * const p = new int(1);
    auto a = f(p);
}