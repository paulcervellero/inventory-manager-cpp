# Inventory Manager

A lightweight command-line inventory management application written in C++17.

The application provides persistent inventory storage using CSV files and supports adding, updating, removing, searching, listing, and saving inventory records through an interactive command-line interface.

---

## Overview

Inventory Manager was built to practice core C++ programming concepts through a practical data-management application.

Inventory records are stored in memory using the C++ Standard Library and persisted between sessions using a CSV file. The program automatically loads existing inventory data at startup and can save changes during execution or when exiting.

---

## Features

- Add new inventory items
- Automatically assign unique item IDs
- List all inventory records
- Update existing items by ID
- Remove items by ID
- Search items by name
- Validate numeric user input
- Save inventory data to CSV
- Load saved inventory automatically at startup
- Save automatically when exiting with `quit`

---

## Inventory Model

Each inventory item contains four fields:

```text
Item
├── ID
├── Name
├── Quantity
└── Price
```

The corresponding C++ structure is:

```cpp
struct Item {
    int id = 0;
    std::string name;
    int qty = 0;
    double price = 0.0;
};
```

Inventory records are stored in memory using a `std::vector<Item>`.

---

## Commands

| Command | Function |
| --- | --- |
| `list` | Display all inventory items |
| `add` | Add a new inventory item |
| `update` | Update an existing item by ID |
| `remove` | Remove an item by ID |
| `search` | Search items using a name substring |
| `save` | Save the current inventory to disk |
| `help` | Display available commands |
| `quit` | Save inventory and exit |

---

## Data Persistence

Inventory data is stored locally in:

```text
inventory.csv
```

Each record is stored in the following format:

```text
id,name,quantity,price
```

Example:

```text
1,Sensor,5,12.5
2,Cable,10,4.99
3,Controller,2,39.95
```

When the application starts, `load_db()` reads the CSV file and reconstructs the inventory in memory.

The `save_db()` function writes the current inventory back to the file.

---

## Program Structure

The application is intentionally implemented as a compact single-file C++ program.

Major components include:

```text
main()
│
├── load_db()
├── save_db()
│
├── list_items()
├── add_item()
├── update_item()
├── remove_item()
├── search_items()
│
├── find_item_by_id()
├── is_number()
└── print_help()
```

The main command loop receives user commands and dispatches them to the appropriate inventory operation.

---

## Technical Concepts

This project demonstrates:

- C++17
- Structures
- `std::vector`
- File streams
- CSV parsing
- String streams
- Standard Library algorithms
- Lambda expressions
- Exception handling
- Input validation
- Dynamic record management
- Command-line interface design

---

## Building

### Requirements

A C++17-compatible compiler such as:

- GCC
- Clang

Compile using:

```bash
g++ -std=c++17 main.cpp -o inventory
```

Run on macOS or Linux:

```bash
./inventory
```

On Windows:

```bash
inventory.exe
```

---

## Example

```text
Inventory Manager
Type 'help' for commands.

> add
Enter name: Sensor
Enter quantity: 5
Enter price: 12.50
Added item id 1.

> list
ID      Name            Qty     Price
----------------------------------------
1       Sensor          5       12.5

> search
Enter search term (name substring): Sensor
ID      Name    Qty     Price
--------------------------------
1       Sensor  5       12.5

> save
Saved.

> quit
Goodbye.
```

---

## Limitations and Future Improvements

The current implementation intentionally keeps the application lightweight. Potential improvements include:

- Case-insensitive searching
- Improved CSV parsing and escaping
- Validation preventing negative quantities and prices
- Sorting and filtering
- Unit tests
- Improved table formatting
- Separation of inventory and persistence logic into classes
- Multiple inventory files
- More robust error handling

---

## What I Learned

This project strengthened my understanding of:

- Building interactive C++ command-line programs
- Managing collections with the C++ Standard Library
- Reading and writing persistent data
- Parsing structured text files
- Validating user input
- Implementing create, read, update, delete, and search operations
- Using Standard Library algorithms and containers
- Structuring a small application into focused functions

---

## Author

Paul Cervellero

Computer Engineering  
University of South Carolina

Portfolio:  
https://paulcervellero.github.io
