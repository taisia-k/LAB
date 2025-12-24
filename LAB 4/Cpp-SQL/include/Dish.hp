#ifndef DISH_H
#define DISH_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>

class Dish {
private:
	int id;
	std::string name;
	int category_id;
	double price;

public:

	Dish (const std::string& n, int id, double p);

	void addDish(pqxx::connection& conn);

    static void showDishes(pqxx::connection& conn);

	static void allDish(pqxx::connection& conn);

	static void top3Dish(pqxx::connection& conn);

	int getId();
};

#endif
