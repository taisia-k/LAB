#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>

class Category {
private:
	int id;
	std::string name;
public:

	Category (const std::string& n);

	void addCategory(pqxx::connection& conn);

	static void allCategories(pqxx::connection& conn);

	static void totalRevenue(pqxx::connection& conn);

	int getId();

	std::string getName();
};

#endif
