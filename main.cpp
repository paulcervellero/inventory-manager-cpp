// main.cpp
// Inventory Manager - simple CLI inventory tool
// Build: g++ -std=c++17 main.cpp -o inventory

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Item {
    int id = 0;
    std::string name;
    int qty = 0;
    double price = 0.0;
};

static std::vector<Item> items;
static int next_id = 1;
const std::string DB_FILENAME = "inventory.csv";

bool is_number(const std::string &s) {
    if (s.empty()) return false;
    char *end = nullptr;
    std::strtod(s.c_str(), &end);
    return (*end == '\0');
}

void load_db(const std::string &fname) {
    std::ifstream in(fname);
    if (!in.is_open()) return;
    items.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        Item it;

        // id
        if (!std::getline(ss, token, ',')) continue;
        try { it.id = std::stoi(token); } catch (...) { continue; }

        // name
        if (!std::getline(ss, token, ',')) token = "";
        it.name = token;

        // qty
        if (!std::getline(ss, token, ',')) token = "0";
        try { it.qty = std::stoi(token); } catch (...) { it.qty = 0; }

        // price
        if (!std::getline(ss, token, ',')) token = "0";
        try { it.price = std::stod(token); } catch (...) { it.price = 0.0; }

        items.push_back(it);
        if (it.id >= next_id) next_id = it.id + 1;
    }
    in.close();
}

void save_db(const std::string &fname) {
    std::ofstream out(fname, std::ios::trunc);
    for (const auto &it : items) {
        out << it.id << ',' << it.name << ',' << it.qty << ',' << it.price << '\n';
    }
    out.close();
}

void list_items() {
    if (items.empty()) {
        std::cout << "No items in inventory.\n";
        return;
    }
    std::cout << "ID\tName\t\tQty\tPrice\n";
    std::cout << "----------------------------------------\n";
    for (const auto &it : items) {
        std::cout << it.id << '\t'
                  << it.name << '\t';
        if (it.name.size() < 8) std::cout << '\t';
        std::cout << it.qty << '\t' << it.price << '\n';
    }
}

void add_item() {
    Item it;
    it.id = next_id++;
    std::cout << "Enter name: ";
    std::getline(std::cin, it.name);
    while(it.name.empty()) {
        std::cout << "Name cannot be empty. Enter name: ";
        std::getline(std::cin, it.name);
    }

    std::string tmp;
    std::cout << "Enter quantity: ";
    std::getline(std::cin, tmp);
    while (!is_number(tmp)) {
        std::cout << "Invalid number. Enter quantity: ";
        std::getline(std::cin, tmp);
    }
    it.qty = std::stoi(tmp);

    std::cout << "Enter price: ";
    std::getline(std::cin, tmp);
    while (!is_number(tmp)) {
        std::cout << "Invalid number. Enter price: ";
        std::getline(std::cin, tmp);
    }
    it.price = std::stod(tmp);

    items.push_back(it);
    std::cout << "Added item id " << it.id << ".\n";
}

Item* find_item_by_id(int id) {
    for (auto &it : items) {
        if (it.id == id) return &it;
    }
    return nullptr;
}

void update_item() {
    std::string tmp;
    std::cout << "Enter item id to update: ";
    std::getline(std::cin, tmp);
    if (!is_number(tmp)) { std::cout << "Invalid id.\n"; return; }
    int id = std::stoi(tmp);
    Item *it = find_item_by_id(id);
    if (!it) { std::cout << "Item not found.\n"; return; }

    std::cout << "Current name: " << it->name << "\nNew name (leave blank to keep): ";
    std::string s; std::getline(std::cin, s);
    if (!s.empty()) it->name = s;

    std::cout << "Current qty: " << it->qty << "\nNew qty (leave blank to keep): ";
    std::getline(std::cin, s);
    if (!s.empty()) {
        if (!is_number(s)) { std::cout << "Invalid qty; update skipped.\n"; }
        else it->qty = std::stoi(s);
    }

    std::cout << "Current price: " << it->price << "\nNew price (leave blank to keep): ";
    std::getline(std::cin, s);
    if (!s.empty()) {
        if (!is_number(s)) { std::cout << "Invalid price; update skipped.\n"; }
        else it->price = std::stod(s);
    }
    std::cout << "Item updated.\n";
}

void remove_item() {
    std::string tmp;
    std::cout << "Enter item id to remove: ";
    std::getline(std::cin, tmp);
    if (!is_number(tmp)) { std::cout << "Invalid id.\n"; return; }
    int id = std::stoi(tmp);
    auto it = std::remove_if(items.begin(), items.end(), [&](const Item& x){ return x.id == id; });
    if (it != items.end()) {
        items.erase(it, items.end());
        std::cout << "Item removed.\n";
    } else {
        std::cout << "Item not found.\n";
    }
}

void search_items() {
    std::cout << "Enter search term (name substring): ";
    std::string term; std::getline(std::cin, term);
    if (term.empty()) { std::cout << "Empty search.\n"; return; }
    bool found = false;
    for (const auto &it : items) {
        if (it.name.find(term) != std::string::npos) {
            if (!found) {
                std::cout << "ID\tName\tQty\tPrice\n";
                std::cout << "--------------------------------\n";
            }
            found = true;
            std::cout << it.id << '\t' << it.name << '\t' << it.qty << '\t' << it.price << '\n';
        }
    }
    if (!found) std::cout << "No matches.\n";
}

void print_help() {
    std::cout << "Commands:\n"
              << "  list    - show all items\n"
              << "  add     - add a new item\n"
              << "  update  - update an existing item by id\n"
              << "  remove  - remove item by id\n"
              << "  search  - search items by name\n"
              << "  save    - save inventory to file\n"
              << "  help    - show this help\n"
              << "  quit    - save & exit\n";
}

int main() {
    load_db(DB_FILENAME);
    std::cout << "Inventory Manager\nType 'help' for commands.\n";
    std::string cmd;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, cmd)) break;
        if (cmd == "list") list_items();
        else if (cmd == "add") add_item();
        else if (cmd == "update") update_item();
        else if (cmd == "remove") remove_item();
        else if (cmd == "search") search_items();
        else if (cmd == "save") { save_db(DB_FILENAME); std::cout << "Saved.\n"; }
        else if (cmd == "help") print_help();
        else if (cmd == "quit") { save_db(DB_FILENAME); std::cout << "Goodbye.\n"; break; }
        else if (cmd.empty()) continue;
        else std::cout << "Unknown command. Type 'help' for commands.\n";
    }
    return 0;
}
