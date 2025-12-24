#include "../include/Order.hpp"

using namespace std;

Order::Order (int id, const string& date, int q) : dish_id(id), order_date(date), quantity(q){};

void Order::addOrder(pqxx::connection& conn) {

	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec("INSERT INTO orders (dish_id, order_date, quantity) VALUES ("
				+ to_string(dish_id) + ", "
				+ txn.quote(order_date) + ", "
				+ to_string(quantity) +
				") RETURNING order_id"
		);

		order_id = res[0]["order_id"].as<int>();

		cout << "Заказ добавлен. ID = " << order_id << endl;

		txn.commit();

	}
	catch (const std::exception& e) {
		cerr << "Ошибка при добавлении заказа: " << e.what() << endl;
	}
}

void Order::showOrders(pqxx::connection& conn) {
    try {

        pqxx::work txn(conn);

        pqxx::result res = txn.exec(
        		"SELECT\n"
				"	o.*,\n"
				"	(o.quantity * d.price) AS total_price\n"
				"FROM orders o\n"
				"JOIN dishes d ON o.dish_id = d.id\n"
        		"ORDER BY order_date;"
		);

		txn.commit();

        cout << "Все заказы:" << endl;

        for (const auto& row : res) {
            cout << "ID: " << row["order_id"].as<int>()
                 << ", Dish_ID: " << row["dish_id"].as<int>()
                 << ", Order_date: " << row["order_date"].c_str()
                 << ", Quantity: " << row["quantity"].as<int>()
                 << ", Total_price: " << row["total_price"].as<double>()
				 << endl;
        }



    }
    catch (const std::exception& e) {
        cerr << "Ошибка при выводе блюд: " << e.what() << endl;
    }
}

void Order::averageCost(pqxx::connection& conn) {

    try {
        pqxx::work txn(conn);

        pqxx::result res = txn.exec(
        		"SELECT\n"
        		"AVG(o.quantity * d.price) AS average_cost\n"
        		"FROM orders o\n"
        		"JOIN dishes d ON d.id = o.dish_id;"
        );

        cout << "Средняя стоимость заказа:" << endl;

        cout << res[0]["average_cost"].as<double>() << endl;
    }
    catch (const std::exception& e) {
        cerr << "Ошибка при чтении заказов: " << e.what() << endl;
    }
}

void Order::countOrders(pqxx::connection& conn) {

    try {
        pqxx::work txn(conn);

        pqxx::result res = txn.exec(
        		"SELECT\n"
				"d.id,\n"
				"d.name,\n"
				"COUNT(o.order_id) AS total_count\n"
        		"FROM dishes d\n"
        		"LEFT JOIN orders o ON o.dish_id = d.id\n"
        		"GROUP BY d.id, d.name\n"
        		"ORDER BY d.id ASC;\n"
        );

        cout << "Общее количество заказов для каждого блюда:" << endl;

        for (const auto& row : res) {
            cout << "ID: " << row["id"].as<int>()
            	 << ", name: " << row["name"].c_str()
				 << ", total_count: " << row["total_count"].as<int>()
				 << endl;
        }
    }
    catch (const std::exception& e) {
        cerr << "Ошибка при чтении заказов: " << e.what() << endl;
    }
}

int Order::getId() {
	return order_id;
}
