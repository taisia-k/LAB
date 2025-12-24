#include "../include/Category.hpp"

using namespace std;


Category::Category (const string& n) : name(n) {};

void Category::addCategory(pqxx::connection& conn) {
	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec(
			"INSERT INTO categories (category_name) VALUES (" +
			txn.quote(name) +
			") RETURNING id"
		);

		id = res[0]["id"].as<int>();

		txn.commit();

		cout << "Категория добавлена. ID = " << id << endl;
	}
	catch (const std::exception& e) {
		cerr << "Ошибка при добавлении категории: " << e.what() << endl;
	}
}

void Category::allCategories(pqxx::connection& conn) {

	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec("SELECT * FROM categories ORDER BY id");

		cout << "Доступные категории:" << endl;

		for (const auto& row : res) {
			cout << "ID: " << row["id"].as<int>()
				 << ", category_name: " << row["category_name"].c_str()
				 << endl;
		}
	}
	catch (const std::exception& e) {
		cerr << "Ошибка при чтении категорий: " << e.what() << endl;
	}
}

void Category::totalRevenue(pqxx::connection& conn) {

	try {
		pqxx::work txn(conn);

		pqxx::result res = txn.exec(
				"SELECT\n"
				"c.id,\n"
				"c.category_name,\n"
				"COALESCE(SUM(d.price * o.quantity), 0) AS total_price\n"
				"FROM categories c\n"
				"LEFT JOIN dishes d ON c.id = d.category_id\n"
				"LEFT JOIN orders o ON d.id = o.dish_id\n"
				"GROUP BY c.id, c.category_name\n"
				"ORDER BY c.category_name;"
		);

		cout << "Общая выручка по категориям:" << endl;

		for (const auto& row : res) {
			cout << "ID: " << row["id"].as<int>()
				 << ", category_name: " << row["category_name"].c_str()
				 << ", total_price: " << row["total_price"].as<int>()
				 << endl;
		}
	}
	catch (const std::exception& e) {
		cerr << "Ошибка при чтении заказов: " << e.what() << endl;
	}
}

int Category::getId() {
	return id;
}

string Category::getName() {
	return name;
}
