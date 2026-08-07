// main.cpp
// Inventory Manager - C++17 command-line inventory tool
// Build: g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o inventory

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Item {
    int id = 0;
    std::string name;
    int quantity = 0;
    double price = 0.0;
};

static std::vector<Item> items;
static int next_id = 1;

const std::string DB_FILENAME = "inventory.csv";


// --------------------------------------------------
// Input helpers
// --------------------------------------------------

bool parse_int(const std::string& input, int& value) {
    if (input.empty()) {
        return false;
    }

    try {
        std::size_t position = 0;
        int parsed = std::stoi(input, &position);

        if (position != input.size()) {
            return false;
        }

        value = parsed;
        return true;
    }
    catch (...) {
        return false;
    }
}


bool parse_double(const std::string& input, double& value) {
    if (input.empty()) {
        return false;
    }

    try {
        std::size_t position = 0;
        double parsed = std::stod(input, &position);

        if (position != input.size()) {
            return false;
        }

        value = parsed;
        return true;
    }
    catch (...) {
        return false;
    }
}


std::string to_lower(std::string text) {
    std::transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    return text;
}


bool contains_comma(const std::string& text) {
    return text.find(',') != std::string::npos;
}


// --------------------------------------------------
// Persistence
// --------------------------------------------------

bool load_db(const std::string& filename) {
    std::ifstream input(filename);

    if (!input.is_open()) {
        // A missing database is valid on first launch.
        return true;
    }

    items.clear();
    next_id = 1;

    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::string token;
        Item item;

        // ID
        if (!std::getline(stream, token, ',')) {
            continue;
        }

        if (!parse_int(token, item.id) || item.id <= 0) {
            continue;
        }

        // Name
        if (!std::getline(stream, item.name, ',')) {
            continue;
        }

        if (item.name.empty()) {
            continue;
        }

        // Quantity
        if (!std::getline(stream, token, ',')) {
            continue;
        }

        if (!parse_int(token, item.quantity) || item.quantity < 0) {
            continue;
        }

        // Price
        if (!std::getline(stream, token, ',')) {
            continue;
        }

        if (!parse_double(token, item.price) || item.price < 0.0) {
            continue;
        }

        items.push_back(item);

        if (item.id >= next_id) {
            next_id = item.id + 1;
        }
    }

    if (input.bad()) {
        std::cerr << "ERROR: Failed while reading "
                  << filename << ".\n";
        return false;
    }

    return true;
}


bool save_db(const std::string& filename) {
    std::ofstream output(filename, std::ios::trunc);

    if (!output.is_open()) {
        std::cerr << "ERROR: Unable to open "
                  << filename
                  << " for writing.\n";
        return false;
    }

    output << std::fixed << std::setprecision(2);

    for (const auto& item : items) {
        output
            << item.id << ','
            << item.name << ','
            << item.quantity << ','
            << item.price << '\n';
    }

    if (!output) {
        std::cerr << "ERROR: Failed while writing "
                  << filename << ".\n";
        return false;
    }

    return true;
}


// --------------------------------------------------
// Inventory operations
// --------------------------------------------------

Item* find_item_by_id(int id) {
    for (auto& item : items) {
        if (item.id == id) {
            return &item;
        }
    }

    return nullptr;
}


void list_items() {
    if (items.empty()) {
        std::cout << "No items in inventory.\n";
        return;
    }

    std::cout
        << std::left
        << std::setw(6) << "ID"
        << std::setw(24) << "Name"
        << std::right
        << std::setw(10) << "Qty"
        << std::setw(12) << "Price"
        << '\n';

    std::cout << std::string(52, '-') << '\n';

    for (const auto& item : items) {
        std::cout
            << std::left
            << std::setw(6) << item.id
            << std::setw(24) << item.name
            << std::right
            << std::setw(10) << item.quantity
            << std::setw(11)
            << std::fixed
            << std::setprecision(2)
            << item.price
            << '\n';
    }
}


void add_item() {
    Item item;
    item.id = next_id;

    while (true) {
        std::cout << "Enter name: ";
        std::getline(std::cin, item.name);

        if (item.name.empty()) {
            std::cout << "Name cannot be empty.\n";
            continue;
        }

        if (contains_comma(item.name)) {
            std::cout
                << "Name cannot contain commas because "
                << "inventory is stored as CSV.\n";
            continue;
        }

        break;
    }

    std::string input;

    while (true) {
        std::cout << "Enter quantity: ";
        std::getline(std::cin, input);

        if (parse_int(input, item.quantity)
            && item.quantity >= 0) {
            break;
        }

        std::cout
            << "Quantity must be a non-negative integer.\n";
    }

    while (true) {
        std::cout << "Enter price: ";
        std::getline(std::cin, input);

        if (parse_double(input, item.price)
            && item.price >= 0.0) {
            break;
        }

        std::cout
            << "Price must be a non-negative number.\n";
    }

    items.push_back(item);
    ++next_id;

    std::cout << "Added item ID "
              << item.id << ".\n";
}


void update_item() {
    std::cout << "Enter item ID to update: ";

    std::string input;
    std::getline(std::cin, input);

    int id = 0;

    if (!parse_int(input, id) || id <= 0) {
        std::cout << "Invalid item ID.\n";
        return;
    }

    Item* item = find_item_by_id(id);

    if (item == nullptr) {
        std::cout << "Item not found.\n";
        return;
    }

    std::string value;

    std::cout
        << "Current name: " << item->name
        << "\nNew name (leave blank to keep): ";

    std::getline(std::cin, value);

    if (!value.empty()) {
        if (contains_comma(value)) {
            std::cout
                << "Name contains a comma; "
                << "name update skipped.\n";
        }
        else {
            item->name = value;
        }
    }

    std::cout
        << "Current quantity: " << item->quantity
        << "\nNew quantity (leave blank to keep): ";

    std::getline(std::cin, value);

    if (!value.empty()) {
        int quantity = 0;

        if (parse_int(value, quantity)
            && quantity >= 0) {
            item->quantity = quantity;
        }
        else {
            std::cout
                << "Invalid quantity; update skipped.\n";
        }
    }

    std::cout
        << "Current price: "
        << std::fixed
        << std::setprecision(2)
        << item->price
        << "\nNew price (leave blank to keep): ";

    std::getline(std::cin, value);

    if (!value.empty()) {
        double price = 0.0;

        if (parse_double(value, price)
            && price >= 0.0) {
            item->price = price;
        }
        else {
            std::cout
                << "Invalid price; update skipped.\n";
        }
    }

    std::cout << "Item updated.\n";
}


void remove_item() {
    std::cout << "Enter item ID to remove: ";

    std::string input;
    std::getline(std::cin, input);

    int id = 0;

    if (!parse_int(input, id) || id <= 0) {
        std::cout << "Invalid item ID.\n";
        return;
    }

    auto new_end = std::remove_if(
        items.begin(),
        items.end(),
        [id](const Item& item) {
            return item.id == id;
        }
    );

    if (new_end == items.end()) {
        std::cout << "Item not found.\n";
        return;
    }

    items.erase(new_end, items.end());

    std::cout << "Item removed.\n";
}


void search_items() {
    std::cout << "Enter search term: ";

    std::string term;
    std::getline(std::cin, term);

    if (term.empty()) {
        std::cout << "Search term cannot be empty.\n";
        return;
    }

    const std::string normalized_term = to_lower(term);

    bool found = false;

    for (const auto& item : items) {
        if (to_lower(item.name).find(normalized_term)
            != std::string::npos) {

            if (!found) {
                std::cout
                    << std::left
                    << std::setw(6) << "ID"
                    << std::setw(24) << "Name"
                    << std::right
                    << std::setw(10) << "Qty"
                    << std::setw(12) << "Price"
                    << '\n';

                std::cout
                    << std::string(52, '-')
                    << '\n';
            }

            found = true;

            std::cout
                << std::left
                << std::setw(6) << item.id
                << std::setw(24) << item.name
                << std::right
                << std::setw(10) << item.quantity
                << std::setw(11)
                << std::fixed
                << std::setprecision(2)
                << item.price
                << '\n';
        }
    }

    if (!found) {
        std::cout << "No matching items found.\n";
    }
}


// --------------------------------------------------
// CLI
// --------------------------------------------------

void print_help() {
    std::cout
        << "Commands:\n"
        << "  list    - show all items\n"
        << "  add     - add a new item\n"
        << "  update  - update an item by ID\n"
        << "  remove  - remove an item by ID\n"
        << "  search  - search items by name\n"
        << "  save    - save inventory to file\n"
        << "  help    - show this help\n"
        << "  quit    - save and exit\n";
}


int main() {
    if (!load_db(DB_FILENAME)) {
        std::cerr
            << "WARNING: Inventory database "
            << "could not be loaded completely.\n";
    }

    std::cout
        << "Inventory Manager\n"
        << "Type 'help' for commands.\n";

    std::string command;

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, command)) {
            std::cout << '\n';

            if (!save_db(DB_FILENAME)) {
                return 1;
            }

            break;
        }

        if (command == "list") {
            list_items();
        }
        else if (command == "add") {
            add_item();
        }
        else if (command == "update") {
            update_item();
        }
        else if (command == "remove") {
            remove_item();
        }
        else if (command == "search") {
            search_items();
        }
        else if (command == "save") {
            if (save_db(DB_FILENAME)) {
                std::cout << "Inventory saved.\n";
            }
        }
        else if (command == "help") {
            print_help();
        }
        else if (command == "quit") {
            if (!save_db(DB_FILENAME)) {
                return 1;
            }

            std::cout << "Goodbye.\n";
            break;
        }
        else if (command.empty()) {
            continue;
        }
        else {
            std::cout
                << "Unknown command. "
                << "Type 'help' for commands.\n";
        }
    }

    return 0;
}
