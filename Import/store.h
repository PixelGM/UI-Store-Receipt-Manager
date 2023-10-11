#ifndef STORE_H
#define STORE_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "nlohmann/json.hpp"
#include <fstream>

#include <QString>
#include <QTextStream>

class Item {
private:
    int id;
    std::string name;
    double price;

public:
    // Default constructor
    Item() : id(30669), name("Bananas"), price(1.99) {}

    Item(int id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }

    void applyDiscount(double percentage) {
        if (percentage >= 0 && percentage <= 1) {
            price -= price * percentage;
        }
        else {
            std::cerr << "Invalid discount percentage. It should be between 0 and 1." << std::endl;
        }
    }
};

class Receipt {
private:
    std::string title;
    std::string branch;
    std::string street;
    std::string city;
    int member;
    std::vector<Item> items;

public:
    // Receipt constructor
    Receipt()
        : title(""), branch(""), street(""),
        city(""), member(000000000000) {
        items.push_back(Item()); // Default item (Bananas)
        items.push_back(Item(55506, "Chicken", 34.21));
    }

    // Getters and Setters Receipt
    void setTitle(const std::string& title) { this->title = title; }
    void setBranch(const std::string& branchName) { branch = branchName; }
    void setStreet(const std::string& street) { this->street = street; }
    void setCity(const std::string& details) { city = details; }
    void setMember(int number) { member = number; }
    void setItems(const std::vector<Item>& newItems) { items = newItems; }

    std::string getTitle() const { return title; }
    std::string getBranch() const { return branch; }
    std::string getStreet() const { return street; }
    std::string getCity() const { return city; }
    int getMember() const { return member; }
    const std::vector<Item>& getItems() const { return items; }

    void addItem(const std::string& idStr, const std::string& name, const std::string& priceStr) {
        // Convert id string to int
        int id = std::stoi(idStr);

        // Convert price string to double
        double price = std::stod(priceStr);

        Item item(id, name, price);
        items.push_back(item);
    }

    QString printReceipt() const {
        QString receipt;
        QTextStream out(&receipt);

        out << "\n";
        out << "---------------------------\n";
        out << QString::fromStdString(title) << "\n";
        out << QString::fromStdString(branch) << "\n";
        out << QString::fromStdString(street) << "\n";
        out << QString::fromStdString(city) << "\n\n";
        out << "Member " << member << "\n";
        out << "\n";

        double subtotal = 0.0;
        for (const auto& item : items) {
            out << QString::number(item.getId()) << " " << QString::fromStdString(item.getName()) << " " << QString::number(item.getPrice(), 'f', 2) << "\n";
            subtotal += item.getPrice();
        }

        double gst = subtotal * 0.05; // Assuming 5% GST
        double pst = subtotal * 0.07; // Assuming 7% PST
        double tax = gst + pst;
        double total = subtotal + tax;

        out << "\n";
        out << "Subtotal " << QString::number(subtotal, 'f', 2) << "\n";
        out << "Tax " << QString::number(tax, 'f', 2) << " (GST " << QString::number(gst, 'f', 2) << " PST " << QString::number(pst, 'f', 2) << ")\n";
        out << "TOTAL (" << QString::number(total, 'f', 2) << ")\n";
        out << "---------------------------\n";
        out << "\n";

        return receipt;
    }

    QString printSettings() const {
        QString receipt;
        QTextStream out(&receipt);

        out << "Title: " << QString::fromStdString(title) << "\n";
        out << "Branch: " << QString::fromStdString(branch) << "\n";
        out << "Street: " << QString::fromStdString(street) << "\n";
        out << "City: " << QString::fromStdString(city) << "\n";

        return receipt;
    }

    void applyDiscountToItem(int itemId, double percentage) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.applyDiscount(percentage);
                return;
            }
        }
        std::cerr << "Item with ID: " << itemId << " not found." << std::endl;
    }

};

nlohmann::json to_json(const Item& item) {
    nlohmann::json j;
    j["id"] = item.getId();
    j["name"] = item.getName();
    j["price"] = item.getPrice();
    return j;
}

nlohmann::json to_json(const Receipt& receipt) {
    nlohmann::json j;
    j["title"] = receipt.getTitle();
    j["branch"] = receipt.getBranch();
    j["street"] = receipt.getStreet();
    j["city"] = receipt.getCity();
    j["member"] = receipt.getMember();

    // For the items vector:
    for (const auto& item : receipt.getItems()) {
        j["items"].push_back(to_json(item));
    }
    return j;
}

// Save the Receipt object to a JSON file
void save_to_file(const Receipt& receipt, const std::string& filename) {
    nlohmann::json j = to_json(receipt);

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return;
    }

    file << j.dump(4); // 4 spaces for indentation
}

// Load the Receipt object from a JSON file
void load_from_file(Receipt& receipt, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;

    // Check for the existence of keys and handle potential null values
    std::string title = j.contains("title") && !j["title"].is_null() ? j["title"].get<std::string>() : "Default Store Title";
    std::string branch = j.contains("branch") && !j["branch"].is_null() ? j["branch"].get<std::string>() : "Default Branch";
    std::string street = j.contains("street") && !j["street"].is_null() ? j["street"].get<std::string>() : "Default Street Name";
    std::string city = j.contains("city") && !j["city"].is_null() ? j["city"].get<std::string>() : "Default City Details";
    int member = j.contains("member") && !j["member"].is_null() ? j["member"].get<int>() : 0; // Default member number as 0

    std::vector<Item> items;
    if (j.contains("items") && j["items"].is_array()) {
        for (const auto& item_json : j["items"]) {
            int id = item_json.contains("id") && !item_json["id"].is_null() ? item_json["id"].get<int>() : 0;
            std::string name = item_json.contains("name") && !item_json["name"].is_null() ? item_json["name"].get<std::string>() : "Default Item Name";
            double price = item_json.contains("price") && !item_json["price"].is_null() ? item_json["price"].get<double>() : 0.0;
            items.push_back(Item(id, name, price));
        }
    }

    // Set the values to the receipt object
    receipt.setTitle(title);
    receipt.setBranch(branch);
    receipt.setStreet(street);
    receipt.setCity(city);
    receipt.setMember(member);
    receipt.setItems(items);
}

void processCommand(Receipt& receipt) {
    std::string command;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "/exit") {
            break;
        }

        std::istringstream iss(command);
        std::string cmd, arg1, arg2, arg3, arg4;

        iss >> cmd;

        if (cmd == "/set") {
            iss >> arg1;
            std::getline(iss, arg2); // Read the rest of the line after the first argument

            if (!arg2.empty()) {
                arg2 = arg2.substr(1);   // Remove the leading space
            }


            if (arg1 == "title") {
                receipt.setTitle(arg2);
                std::cout << "Title Has Been Set!" << std::endl;
            }
            else if (arg1 == "branch") {
                receipt.setBranch(arg2);
                std::cout << "Branch Has Been Set!" << std::endl;
            }
            else if (arg1 == "street") {
                receipt.setStreet(arg2);
                std::cout << "Street Has Been Set!" << std::endl;
            }
            else if (arg1 == "city") {
                receipt.setCity(arg2);
                std::cout << "City Has Been Set!" << std::endl;
            }
            else if (arg1 == "member") {
                try {
                    receipt.setMember(std::stoi(arg2));
                    std::cout << "Title Has Been Set!" << std::endl;
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid member number provided." << std::endl;
                }
            }
            else {
                std::cout << "Invalid argument for /set command." << std::endl;
            }
        }
        else if (cmd == "/additem") {
            iss >> arg1 >> arg2 >> arg3;
            if (!arg1.empty() && !arg2.empty() && !arg3.empty()) {
                if (arg1.size() == 5) {
                    receipt.addItem(arg1, arg2, arg3);
                    std::cout << "An Item Has Been Added!" << std::endl;
                } 
                else {
                    std::cout << "Item ID has to be 5 Digits!" << std::endl;
                    continue;
                }
                
            }
            else {
                std::cout << "Invalid arguments for /additem command." << std::endl;
            }
        }
        else if (cmd == "/print") {
            receipt.printReceipt();
        }
        else if (cmd == "/save") {
            save_to_file(receipt, "receipt.json");
        }
        else if (cmd == "/load") {
            try {
                load_from_file(receipt, "receipt.json");
                std::cout << "JSON has been Loaded!" << std::endl;
            }
            catch (const nlohmann::json::exception& e) {
                std::cerr << "JSON error: " << e.what() << std::endl;
            }
        }
        else if (cmd == "/discount") {
            double discountPercentage;
            int itemId;

            iss >> itemId >> discountPercentage;

            receipt.applyDiscountToItem(itemId, discountPercentage);
            std::cout << itemId << " has been Discounted by " << discountPercentage * 100 << "%!" << std::endl;
        }
        else {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }
}


#endif // STORE_H
