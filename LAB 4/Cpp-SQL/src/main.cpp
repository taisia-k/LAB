#include <iostream>
#include <pqxx/pqxx>
#include <istream>
#include <regex>
#include <fstream>
#include <chrono>
#include <sstream>
#include "../include/Category.hpp"
#include "../include/Dish.hpp"
#include "../include/Order.hpp"

using namespace std;

bool isId(pqxx::connection& conn, int id, const string& table) {

	bool f;

    try {

        pqxx::work txn(conn);

        pqxx::result res = txn.exec( "SELECT 1 FROM " + table + " WHERE id = " + to_string(id));

        f = !res.empty();
    } catch (const std::exception& e) {
    	cerr << "Ошибка при проверки ID: " << e.what() << endl;
    }

	return f;
}

bool isValidDate(const std::string& date) {

    regex date_regex(R"(\d{4}-\d{2}-\d{2})");
    if (!regex_match(date, date_regex)) return false;

    int year = stoi(date.substr(0,4));
    int month = stoi(date.substr(5,2));
    int day = stoi(date.substr(8,2));

    if (month < 1 || month > 12) return false;

    int days_in_month[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        days_in_month[2] = 29;

    if (day < 1 || day > days_in_month[month]) return false;

    return true;
}

void logAction(ofstream& file, const string& text) {
	auto now = chrono::system_clock::now();
	time_t now_c = chrono::system_clock::to_time_t(now);
	tm now_tm = *localtime(&now_c);
    file << text << " " << put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << endl;
}

void Menu() {
	cout << "===ВЫБЕРИТЕ ДЕЙСТВИЕ===" << endl;
	cout << "1.Добавить категорию" << endl;
	cout << "2.Добавить блюдо" << endl;
	cout << "3.Добавить заказ" << endl;
	cout << "4.Просмотр всех блюд" << endl;
	cout << "5.Просмотр всех заказов" << endl;
	cout << "6.Вывести общую выручку по категориям" << endl;
	cout << "7.Вывести топ-3 самых продаваемых блюда" << endl;
	cout << "8.Вывести среднюю стоимость заказа" << endl;
	cout << "9.Вывести общее количество заказов для каждого блюда" << endl;
	cout << "10.Выход из меню" << endl;
	cout << "Ваш выбор: ";
}

int main() {
	int choice = 0;

	ofstream outFile("log.txt", ios::app);
	if(!outFile.is_open()) {
		cerr << "Ошибка! Не удалось открыть файл для логирования" << endl;
	} else {

		try {
			string conninfo = "host=localhost port=5432 dbname=restaurant_db user=postgres password=***";

			pqxx::connection conn(conninfo);

			if (conn.is_open()) {
				cout << "Connected to database: " << conn.dbname() << endl;
			} else {
				cerr << "Failed to connect to the database." << endl;
				return 1;
			}

			do {
				cout << endl;
				Menu();
				cin >> choice;
				cin.ignore();

				switch (choice) {
					case (1) : {
						string name;

						Category::allCategories(conn);
						cout << "Введите название категории: ";
						getline(cin, name);

						Category newCategory(name);
						newCategory.addCategory(conn);
						ostringstream ss;
					    ss << "Добавлена новая категория с ID = " << newCategory.getId() << " ";
					    logAction(outFile, ss.str());
						break;
					}
					case (2) : {
						string name;
						int id;
						double price;

						cout << "Введите название блюда: ";
						getline(cin, name);
						Category::allCategories(conn);
						cout << "Введите id категории: ";
						cin >> id;
						while(!isId(conn, id, "categories")) {
							cout << "Ошибка! Такой категории нет" << endl;
							cout << endl;
							Category::allCategories(conn);
							cout << "Введите id категории: ";
							cin >> id;
						}
						cout << "Введите стоимость: ";
						cin >> price;
						while(price < 0) {
							cout << "Ошибка! Стоимость не может быть отрицательной" << endl;
							cout << "Введите стоимость: ";
							cin >> price;
						}

						Dish newDish(name, id, price);
						newDish.addDish(conn);
						ostringstream ss;
						ss <<"Добавлено новое блюдо с ID = " << newDish.getId() << " ";
						logAction(outFile, ss.str());
						break;
					}
					case (3) : {
						int dish_id;
						string order_date;
						int quantity;

						Dish::allDish(conn);
						cout << "Введите id блюда: ";
						cin >> dish_id;
						while(!isId(conn, dish_id, "dishes")) {
							cout << "Ошибка! Такого блюда нет" << endl;
							cout << endl;
							Dish::allDish(conn);
							cout << "Введите id блюда: ";
							cin >> dish_id;
						}
						cin.ignore();
						cout << "Введите дату заказа в формате 'YYYY-MM-DD': ";
						getline(cin, order_date);
						while(!isValidDate(order_date)) {
							cout << "Некорректная дата" << endl;
							cout << "Введите дату заказа в формате 'YYYY-MM-DD': ";
							getline(cin, order_date);
						}
						cout << "Введите количество заказанных порций: ";
						cin >> quantity;
						while (quantity < 0) {
							cout << "Ошибка! Стоимость не может быть отрицательной" << endl;
							cout << "Введите количество заказанных порций: ";
							cin >> quantity;
						}

						Order newOrder(dish_id, order_date, quantity);
						newOrder.addOrder(conn);
						ostringstream ss;
						ss << "Добавлен новый заказ с ID = " << newOrder.getId() << " ";
						logAction(outFile, ss.str());
						break;
					}
					case (4) : {
						Dish::showDishes(conn);
						logAction(outFile, "Аналитический запрос");
						break;
					}
					case (5) : {
						Order::showOrders(conn);
						logAction(outFile, "Аналитический запрос");
						break;
					}
					case (6) : {
						Category::totalRevenue(conn);
						logAction(outFile, "Аналитический запрос");
						break;
					}

					case (7) : {
						Dish::top3Dish(conn);
						logAction(outFile, "Аналитический запрос");
						break;
					}

					case (8) : {
						Order::averageCost(conn);
						logAction(outFile, "Аналитический запрос");
						break;
					}

					case (9) : {
						Order::countOrders(conn);
						logAction(outFile, "Аналитический запрос");
						break;
					}

					case (10) : {
						cout << "Выход из программы...";
						conn.close();
						break;
					}
					default : {
						cout << "Ошибка! Неверный выбор" << endl;
					}
				}

			} while (choice != 10);

		} catch (const std::exception& e) {
			cerr << "Error: " << e.what() << endl;
			return 1;
		}
		outFile.close();
	}

    return 0;
}
