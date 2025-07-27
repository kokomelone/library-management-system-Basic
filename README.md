# library-management-system-Basic (C++)
A console based Library Management System in C++ using OOP and STL. Supports book issuing, late return fines, transaction logs with stacks, and complete library reports.

## Features

- Add new books with title, author, and ID
- Add members and track their issued books
- Issue books (max 3 per member)
- Calculate late return fines (₹1/day after 14 days)
- Return books and show fine if any
- Maintain a transaction history using `stack`
- Generate a report showing:
  - All books and their availability
  - Member details and their issued books
  - Recent transactions (issue/return)

## Technologies Used

- C++
- STL: `list`, `stack`
- Object-Oriented Programming (OOP)
  
## Classes Overview

- **Book**  
  Represents a book with ID, title, author, and availability status.

- **Member**  
  Manages member information and the books they have issued.

- **Librarian**  
  Stores librarian details (name and staff ID).

- **TransactionDetails (Template)**  
  Inherits from Member; manages return and fine display.

- **Library**  
  Core class that manages:
  - Books and members
  - Issue/return functions
  - Transaction history using stack
  - Day advancement and report generation

## How to Run

1. **Save your code** in a file called `library.cpp`

2. **Compile the code** using any C++ compiler.
