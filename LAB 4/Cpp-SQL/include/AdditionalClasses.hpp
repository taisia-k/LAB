#ifndef ADDITIONALCLASSES_H
#define ADDITIONALCLASSES_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include "../include/Category.hpp"

class MainCourse : public Category {
public:
	std::string getType();
};

class Dessert : public Category {
public:
	std::string getType();
};

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair(T1 f, T2 s);
};

#endif
