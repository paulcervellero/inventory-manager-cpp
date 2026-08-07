# Inventory Manager

A lightweight command-line inventory management application written in C++17 with CSV persistence, validated CRUD operations, case-insensitive search, and robust file handling.

The project demonstrates core C++ and software development concepts through a practical application for creating, managing, searching, and persistently storing inventory records.

---

## Features

- Add new inventory items
- Automatically assign unique item IDs
- List all inventory records
- Update existing items by ID
- Remove items by ID
- Case-insensitive search by item name
- Validate quantities and prices
- Reject negative quantities and prices
- Protect against CSV-breaking item names
- Save inventory data to CSV
- Automatically load saved inventory at startup
- Save inventory when exiting
- Detect file read and write errors
- Display prices with consistent currency formatting

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
    int quantity = 0;
    double price = 0.0;
};
```

Inventory records are stored in memory using:

```cpp
std::vector<Item>
```

---

## Commands

| Command | Description |
| --- | --- |
| `list` | Display all inventory items |
| `add` | Add a new inventory item |
| `update` | Update an existing item by ID |
| `remove` | Remove an item by ID |
| `search` | Search items by name |
| `save` | Save the current inventory to disk |
| `help` | Display available commands |
| `quit` | Save the inventory and exit |

---

## Data Persistence

Inventory data is stored locally in:

```text
inventory.csv
```

Each record uses the following format:

```text
id,name,quantity,price
```

Example:

```text
1,Flight Computer,5,49.99
2,Sensor Module,10,12.50
```

When the application starts, the inventory database is loaded into memory.

When the inventory is saved, the current records are written back to the CSV file.

The application also checks for file read and write failures and reports errors through the command line.

---

## Program Structure

The application is intentionally implemented as a compact single-file C++ program.

```text
main()
│
├── Persistence
│   ├── load_db()
│   └── save_db()
│
├── Inventory Operations
│   ├── list_items()
│   ├── add_item()
│   ├── update_item()
│   ├── remove_item()
│   └── search_items()
│
├── Utility Functions
│   ├── find_item_by_id()
│   ├── parse_int()
│   ├── parse_double()
│   ├── to_lower()
│   └── contains_comma()
│
└── CLI
    └── print_help()
```

The main command loop receives user commands and dispatches them to the appropriate inventory operation.

---

## Input Validation

Inventory Manager validates user input before modifying inventory records.

### Quantity

Quantities must be non-negative integers.

Examples:

```text
5       Valid
0       Valid
2.5     Invalid
-5      Invalid
```

### Price

Prices must be non-negative numeric values.

Examples:

```text
49.99   Valid
0       Valid
-10     Invalid
```

### Item Names

Item names:

- Cannot be empty
- Cannot contain commas

Commas are rejected because the application uses a lightweight CSV storage format.

---

## Search

Inventory Manager supports case-insensitive substring searching.

For example, an inventory item named:

```text
Flight Computer
```

can be found using:

```text
flight
```

or:

```text
COMPUTER
```

---

## Technologies

- C++17
- C++ Standard Library
- Git
- GitHub

---

## Technical Concepts

This project demonstrates:

- C++ structures
- `std::vector`
- Standard Library algorithms
- Lambda expressions
- File streams
- CSV parsing
- String streams
- Exception handling
- Input validation
- Case-insensitive string processing
- Command-line interfaces
- Persistent data storage
- CRUD-style operations
- Error handling

---

## Building

### Requirements

A C++17-compatible compiler such as:

- GCC
- Clang

Compile with warnings enabled:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o inventory
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

## Example Usage

```text
Inventory Manager
Type 'help' for commands.

> add
Enter name: Flight Computer
Enter quantity: 3
Enter price: 49.99
Added item ID 1.

> list
ID    Name                           Qty       Price
----------------------------------------------------
1     Flight Computer                  3       49.99

> update
Enter item ID to update: 1
Current name: Flight Computer
New name (leave blank to keep):
Current quantity: 3
New quantity (leave blank to keep): 5
Current price: 49.99
New price (leave blank to keep):
Item updated.

> search
Enter search term: flight
ID    Name                           Qty       Price
----------------------------------------------------
1     Flight Computer                  5       49.99

> save
Inventory saved.

> quit
Goodbye.
```

---

## Verification

The application has been compiled using:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o inventory
```

with no compiler warnings or errors.

Core functionality has been manually verified for:

- Adding inventory records
- Listing records
- Updating records
- Removing records
- Case-insensitive searching
- Integer validation
- Negative-value rejection
- CSV name validation
- Saving data
- Loading persisted data after restart

---

## Project Structure

```text
inventory-manager-cpp/
├── .gitignore
├── LICENSE
├── main.cpp
└── README.md
```

The compiled executable and local `inventory.csv` database are excluded from version control.

---

## Future Improvements

Potential extensions include:

- Automated unit tests
- Full CSV escaping and quoted-field support
- Sorting and filtering
- Multiple inventory databases
- Import/export functionality
- Separation of inventory and persistence logic into dedicated classes
- More advanced command-line argument handling

---

## What I Learned

Building and improving this project strengthened my understanding of:

- Designing interactive C++ command-line applications
- Working with Standard Library containers and algorithms
- Parsing and validating user input
- Reading and writing persistent data
- Handling malformed input safely
- Implementing CRUD-style operations
- Designing case-insensitive search
- Handling file-system errors
- Compiling C++ with strict warning flags
- Testing application behavior across program restarts

---

## Author

Paul Cervellero

Computer Engineering  
University of South Carolina

Portfolio:  
https://paulcervellero.github.io
