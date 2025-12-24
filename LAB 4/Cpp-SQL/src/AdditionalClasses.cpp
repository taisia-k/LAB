#include "../include/AdditionalClasses.hpp"

using namespace std;

string MainCourse::getType() {
	return getName();
}

string Dessert::getType() {
	return getName();
}

template <typename T1, typename T2>
Pair<T1, T2>::Pair (T1 f, T2 s) : first(f), second(s) {}
