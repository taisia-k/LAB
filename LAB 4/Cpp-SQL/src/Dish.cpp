#include "../include/Dish.hpp"

using namespace std;

Dish::Dish (const string& n, int id, double p) : name(n), category_id(id), price(p){};

void Dish::addDish(pqxx::connection& conn) {
	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec(
				"INSERT INTO dishes (name, category_id, price) VALUES ("
				+ txn.quote(name) + ", "
				+ to_string(category_id) + ", "
				+ to_string(price) +
				") RETURNING id"
		);
		id = res[0]["id"].as<int>();

		txn.commit();

		cout << "Блюдо добавлено. ID = " << id << endl;

	}
	catch (const std::exception& e) {
		cerr << "Ошибка при добавлении блюда: " << e.what() << endl;
	}
}

void Dish::showDishes(pqxx::connection& conn) {
	try {

		pqxx::work txn(conn);

		pqxx::result res = txn.exec("SELECT * FROM dishes ORDER BY id");

		cout << "Все блюда:" << endl;

		for (const auto& row : res) {
			cout << "ID: " << row["id"].as<int>()
				 << ", Name: " << row["name"].c_str()
				 << ", Category ID: " << row["category_id"].as<int>()
				 << ", Price: " << row["price"].as<double>()
				 << endl;
		}

		txn.commit();
	}
	catch (const std::exception& e) {
		cerr << "Ошибка при выводе блюд: " << e.what() << endl;
	}
}

void Dish::allDish(pqxx::connection& conn) {

	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec("SELECT * FROM dishes ORDER BY id");

		cout << "Доступные блюда:" << endl;

		for (const auto& row : res) {
			cout << "ID: " << row["id"].as<int>()
				 << ", name: " << row["name"].c_str()
				 << endl;
		}
	}
	catch (const std::exception& e) {
		cerr << "Ошибка при чтении блюд: " << e.what() << endl;
	}

}

void Dish::top3Dish(pqxx::connection& conn) {
	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec(
				"SELECT\n"
				"d.id,\n"
				"d.name,\n"
				"COALESCE(SUM(o.quantity), 0) AS quantity\n"
				"FROM dishes d\n"
				"LEFT JOIN orders o ON d.id = o.dish_id\n"
				"GROUP BY d.id, d.name\n"
				"ORDER BY quantity DESC\n"
				"LIMIT 3;"
		);

		for (const auto& row : res) {
			cout << "Name: " << row["name"].c_str() << ", Quantity: " << row["quantity"].as<int>() << endl;
		}

		txn.commit();
	}
	catch (const std::exception& e) {
		cerr << "Ошибка при чтении заказов: " << e.what() << endl;
	}
}

int Dish::getId() {
	return id;
}
