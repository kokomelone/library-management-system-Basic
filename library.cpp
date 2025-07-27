#include <iostream>
#include <list>
#include <stack>
using namespace std;

// 1st class: Book
class Book {
private:
    int id;
    string title, author;
    bool available;

public:
    Book(int id, string title, string author) {
        this->id = id;
        this->title = title;
        this->author = author;
        available = true;
    }

    int getId() const {
        return id;
    }

    string getTitle() const {
        return title;
    }

    bool isAvailable() const {
        return available;
    }

    void markIssued() {
        available = false;
    }

    void markReturned() {
        available = true;
    }
};

// 2nd class: Member
class Member {
public:
    struct IssuedBook {
        int bookId;
        int dueDay;
    };

    int memberId;
    string name;
    int issuedBooks;
    list<IssuedBook> issuedList;

    static const int MAX_ISSUE = 3;
    static const int DUE_PERIOD = 14;
    static const int FINE_PER_DAY = 1;

    Member(int id, string name) {
        memberId = id;
        this->name = name;
        issuedBooks = 0;
    }

    int getId() const {
        return memberId;
    }

    void issueBook(int bookId, int currentDay) {
        if (issuedBooks >= MAX_ISSUE)
            throw runtime_error("You can't issue more than 3 books");

        issuedList.push_back({ bookId, currentDay + DUE_PERIOD });
        issuedBooks++;
    }

    int returnBookfine(int bookId, int returnDay) {
        for (auto it = issuedList.begin(); it != issuedList.end(); ++it) {
            if (it->bookId == bookId) {
                int fine = 0;
                if (returnDay > it->dueDay)
                    fine = (returnDay - it->dueDay) * FINE_PER_DAY;

                issuedList.erase(it);
                issuedBooks--;
                return fine;
            }
        }
        throw runtime_error("This book was not issued to you.");
    }
};

// 3rd class: Librarian
class Librarian {
private:
    string staffID;
    string name;

public:
    Librarian(string id, string n) {
        staffID = id;
        name = n;
    }

    string getStaffID() const {
        return staffID;
    }

    string getName() const {
        return name;
    }
};

// 4th class: TransactionDetails
template <typename T>
class TransactionDetails : public Member {
public:
    T data;

    TransactionDetails(int id, string name, T data) : Member(id, name) {
        this->data = data;
    }

    void processReturn(int bookId, int returnDay) {
        try {
            int fine = returnBookfine(bookId, returnDay);
            cout << "Book returned by Member ID " << getId() << ".";
            if (fine > 0) {
                cout << " Fine: Rs." << fine;
            }
            cout << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

struct Transaction {
    int memberId;
    int bookId;
    string action;
    int fine;
};

// 5th class: Library
class Library {
private:
    list<Book> books;
    list<TransactionDetails<string>> members;
    stack<Transaction> transactions;
    int currentDay = 0;

public:
    void addBook(Book b) {
        books.push_back(b);
    }

    void addMember(TransactionDetails<string> m) {
        members.push_back(m);
    }

    void issueBook(int memberId, int bookId) {
        Member* member = nullptr;
        Book* book = nullptr;

        for (auto& m : members) {
            if (m.getId() == memberId) {
                member = &m;
                break;
            }
        }

        if (!member)
            throw runtime_error("Invalid member ID.");

        for (auto& b : books) {
            if (b.getId() == bookId) {
                book = &b;
                break;
            }
        }

        if (!book)
            throw runtime_error("Book not found.");
        if (!book->isAvailable())
            throw runtime_error("Book not available.");

        member->issueBook(bookId, currentDay);
        book->markIssued();
        transactions.push({ memberId, bookId, "Issue", 0 });

        cout << "Book issued successfully." << endl;
    }

    void returnBook(int memberId, int bookId) {
        bool found = false;
        int fine = 0;

        for (auto& m : members) {
            if (m.getId() == memberId) {
                try {
                    fine = m.returnBookfine(bookId, currentDay);
                    cout << "Book returned by Member ID " << m.getId() << ".";
                    if (fine > 0) {
                        cout << " Fine: Rs." << fine;
                    }
                    cout << endl;
                    found = true;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                    return;
                }
                break;
            }
        }

        if (!found)
            throw runtime_error("Invalid member ID.");

        for (auto& b : books) {
            if (b.getId() == bookId) {
                b.markReturned();
                break;
            }
        }

        transactions.push({ memberId, bookId, "Return", fine });
    }

    void advanceDay() {
        int x;
        cout << "Enter day advancement: ";
        cin >> x;
        currentDay += x;
        cout << "Advanced to day " << currentDay << endl;
    }

    void generateReport() {
        cout << "
----- Library Report -----
";
        cout << "Current Day: " << currentDay << endl;

        cout << "
Books in Library:
";
        for (const auto& book : books) {
            cout << "ID: " << book.getId()
                 << ", Title: " << book.getTitle()
                 << ", Available: " << (book.isAvailable() ? "Yes" : "No") << endl;
        }

        cout << "
Members:
";
        for (const auto& member : members) {
            cout << "ID: " << member.getId() << ", Name: " << member.name
                 << ", Books Issued: " << member.issuedBooks << endl;
        }

        cout << "
Transaction History (Most recent first):
";
        stack<Transaction> temp = transactions;
        while (!temp.empty()) {
            Transaction t = temp.top();
            temp.pop();
            cout << "Member ID: " << t.memberId
                 << ", Book ID: " << t.bookId
                 << ", Action: " << t.action;
            if (t.action == "Return" && t.fine > 0) {
                cout << ", Fine Collected: Rs." << t.fine;
            }
            cout << endl;
        }
    }
};

// main function
int main() {
    Library lib;
    int choice;

    while (true) {
        cout << "
===== Library Menu =====" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Add Member" << endl;
        cout << "3. Issue Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Advance Day" << endl;
        cout << "6. Generate Report" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int id;
            string title, author;
            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Title: ";
            getline(cin, title);
            cout << "Enter Author: ";
            getline(cin, author);
            lib.addBook(Book(id, title, author));
            break;
        }
        case 2: {
            int id;
            string name, info;
            cout << "Enter Member ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Additional Info: ";
            getline(cin, info);
            lib.addMember(TransactionDetails<string>(id, name, info));
            break;
        }
        case 3: {
            int memberId, bookId;
            cout << "Enter Member ID: ";
            cin >> memberId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            try {
                lib.issueBook(memberId, bookId);
            } catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 4: {
            int memberId, bookId;
            cout << "Enter Member ID: ";
            cin >> memberId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            try {
                lib.returnBook(memberId, bookId);
            } catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 5: {
            lib.advanceDay();
            break;
        }
        case 6: {
            lib.generateReport();
            break;
        }
        case 7: {
            cout << "Exiting program." << endl;
            return 0;
        }
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
