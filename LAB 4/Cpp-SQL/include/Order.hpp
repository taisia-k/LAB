#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>

class Order {
private:

	int order_id;
	int dish_id;
	std::string order_date;
	int quantity;
public:

	Order (int id, const std::string& date, int q);

	void addOrder(pqxx::connection& conn);

    static void showOrders(pqxx::connection& conn);

    static void averageCost(pqxx::connection& conn);

    static void countOrders(pqxx::connection& conn);

	int getId();
};

#endif
