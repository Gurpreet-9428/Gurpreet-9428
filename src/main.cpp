#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>

// Base class representing a liquor item
class Liquor {
public:
    Liquor(const std::string &name, double price, int quantity)
        : name(name), price(price), quantity(quantity) {}
    virtual ~Liquor() = default;

    const std::string &getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void addQuantity(int qty) { quantity += qty; }
    bool removeQuantity(int qty) {
        if (quantity >= qty) {
            quantity -= qty;
            return true;
        }
        return false;
    }

    virtual std::string type() const = 0;

protected:
    std::string name;
    double price;
    int quantity;
};

// Derived classes for common liquor categories
class Beer : public Liquor {
public:
    using Liquor::Liquor;
    std::string type() const override { return "Beer"; }
};

class Wine : public Liquor {
public:
    using Liquor::Liquor;
    std::string type() const override { return "Wine"; }
};

class Spirit : public Liquor {
public:
    using Liquor::Liquor;
    std::string type() const override { return "Spirit"; }
};

// Generic class for dynamically added varieties
class CustomLiquor : public Liquor {
public:
    CustomLiquor(const std::string &customType, const std::string &name,
                 double price, int quantity)
        : Liquor(name, price, quantity), customType(customType) {}

    std::string type() const override { return customType; }

private:
    std::string customType;
};

class Inventory {
public:
    void addItem(std::shared_ptr<Liquor> item) { items.push_back(std::move(item)); }

    bool restockItem(const std::string &name, int quantity) {
        for (auto &item : items) {
            if (item->getName() == name) {
                item->addQuantity(quantity);
                return true;
            }
        }
        return false;
    }

    bool purchaseItem(const std::string &name, int quantity) {
        for (auto &item : items) {
            if (item->getName() == name) {
                return item->removeQuantity(quantity);
            }
        }
        return false;
    }

    void listItems() const {
        std::cout << std::left << std::setw(12) << "Type"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price"
                  << std::setw(10) << "Quantity" << '\n';
        std::cout << std::string(52, '-') << '\n';
        for (const auto &item : items) {
            std::cout << std::left << std::setw(12) << item->type()
                      << std::setw(20) << item->getName()
                      << std::setw(10) << item->getPrice()
                      << std::setw(10) << item->getQuantity() << '\n';
        }
    }

private:
    std::vector<std::shared_ptr<Liquor>> items;
};

void printMenu() {
    std::cout << "\nLiquor Store Inventory System" << '\n';
    std::cout << "1. Add Item (Beer/Wine/Spirit)" << '\n';
    std::cout << "2. Restock Item" << '\n';
    std::cout << "3. Purchase Item" << '\n';
    std::cout << "4. List Inventory" << '\n';
    std::cout << "5. Add New Variety" << '\n';
    std::cout << "6. Exit" << '\n';
    std::cout << "Select an option: ";
}

int main() {
    Inventory inventory;
    int choice;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            break;
        }
        std::cin.ignore();

        if (choice == 1) {
            int typeChoice;
            std::cout << "Select type (1=Beer, 2=Wine, 3=Spirit): ";
            std::cin >> typeChoice;
            std::cin.ignore();

            std::string name;
            double price;
            int quantity;
            std::cout << "Item name: ";
            std::getline(std::cin, name);
            std::cout << "Price: ";
            std::cin >> price;
            std::cout << "Quantity: ";
            std::cin >> quantity;
            std::cin.ignore();

            std::shared_ptr<Liquor> item;
            if (typeChoice == 1) {
                item = std::make_shared<Beer>(name, price, quantity);
            } else if (typeChoice == 2) {
                item = std::make_shared<Wine>(name, price, quantity);
            } else {
                item = std::make_shared<Spirit>(name, price, quantity);
            }
            inventory.addItem(std::move(item));
        } else if (choice == 2) {
            std::string name;
            int quantity;
            std::cout << "Item name: ";
            std::getline(std::cin, name);
            std::cout << "Quantity to add: ";
            std::cin >> quantity;
            std::cin.ignore();
            if (!inventory.restockItem(name, quantity)) {
                std::cout << "Item not found.\n";
            }
        } else if (choice == 3) {
            std::string name;
            int quantity;
            std::cout << "Item name: ";
            std::getline(std::cin, name);
            std::cout << "Quantity to purchase: ";
            std::cin >> quantity;
            std::cin.ignore();
            if (!inventory.purchaseItem(name, quantity)) {
                std::cout << "Purchase failed. Check item name or quantity.\n";
            }
        } else if (choice == 4) {
            inventory.listItems();
        } else if (choice == 5) {
            std::string type;
            std::string name;
            double price;
            int quantity;
            std::cout << "New variety type: ";
            std::getline(std::cin, type);
            std::cout << "Item name: ";
            std::getline(std::cin, name);
            std::cout << "Price: ";
            std::cin >> price;
            std::cout << "Quantity: ";
            std::cin >> quantity;
            std::cin.ignore();

            inventory.addItem(std::make_shared<CustomLiquor>(type, name, price, quantity));
        } else if (choice == 6) {
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }

    return 0;
}

