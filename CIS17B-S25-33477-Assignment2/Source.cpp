#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>

using namespace std;

//Library Management System
//Lets you add Books
//Lets you add Users
// Lets you view all users, view all books, check in and check out books

// Book Class
class Book {
private:
    string title;
    string author;
    string ISBN;
    bool available;

public:
    Book(const string& t, const string& a, const string& isbn)
        : title(t), author(a), ISBN(isbn), available(true) {}

    void setAvailability(bool status) {
        available = status;
    }

    bool isAvailable() const {
        return available;
    }

    void displayBook() const {
        cout << "Title: " << title << "\nAuthor: " << author << "\nISBN: " << ISBN
            << "\nAvailable: " << (available ? "Yes" : "No") << "\n";
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    string getISBN() const {
        return ISBN;
    }
};

// 2. User and other derived classes students faculty
class User {
protected:
    string name;
    int userID;
    vector<Book*> borrowedBooks;

public:
    static int idCounter; //generates user ids

    User(const string& n) : name(n), userID(idCounter++) {}

    virtual string getClassType() const = 0;

    void borrowBook(Book* book) {
        if (book->isAvailable()) {
            borrowedBooks.push_back(book);
            book->setAvailability(false); // marks book as unavailable
        }
        else {
            cout << "Book is not available for borrowing.\n";
        }
    }

    void returnBook(Book* book) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), book);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            book->setAvailability(true); // marks book as available
        }
        else {
            cout << "You have not borrowed this book.\n";
        }
    }

    void listBorrowedBooks() const {
        cout << "Books Borrowed by " << name << ":\n";
        for (const auto& book : borrowedBooks) {
            book->displayBook();
        }
    }

    int getUserID() const {
        return userID;
    }

    const vector<Book*>& getBorrowedBooks() const {
        return borrowedBooks;
    }

    string getName() const {
        return name;
    }
};

int User::idCounter = 0;

class Student : public User {
public:
    Student(const string& n) : User(n) {}

    string getClassType() const override {
        return "Student";
    }
};

class Faculty : public User {
public:
    Faculty(const string& n) : User(n) {}

    string getClassType() const override {
        return "Faculty";
    }
};

// 3. Library  Uses singleton 
class Library {
private:
    static Library* instance;
    map<string, Book*> books;
    map<int, User*> users;

    Library() {}

public:
    //one instance of librar
    static Library* getInstance() {
        if (!instance) {
            instance = new Library();
        }
        return instance;
    }

    void addBook(Book* book) {
        books[book->getISBN()] = book;
    }

    void addUser(User* user) {
        users[user->getUserID()] = user;
    }

    Book* findBookByTitle(const string& title) {
        for (auto& entry : books) {
            if (entry.second->getTitle() == title) {
                return entry.second;
            }
        }
        return nullptr;
    }

    User* findUserByID(int userID) {
        if (users.find(userID) != users.end()) {
            return users[userID];
        }
        return nullptr;
    }

    void listBooks() const {
        for (auto& entry : books) {
            entry.second->displayBook();
        }
    }

    // Updates and shows the users details and book details etc
    void listUsers() const {
        cout << ">> List All Users\n>> -------\n";
        for (auto& entry : users) {
            User* user = entry.second;
            cout << ">> User " << entry.first << ":\n";
            cout << ">> -- Name: " << user->getName() << "\n";
            cout << ">> -- Class: " << user->getClassType() << "\n";
            cout << ">> -- Books Checked Out:\n";

            if (user->getBorrowedBooks().empty()) {
                cout << ">> -- -- No books checked out.\n";
            }
            else {
                int bookIndex = 0;
                for (const auto& book : user->getBorrowedBooks()) {
                    cout << ">> --  Book " << bookIndex++ << ":\n";
                    cout << ">> --  Title: " << book->getTitle() << "\n";
                    cout << ">> --  Author: " << book->getAuthor() << "\n";
                    cout << ">> --  ISBN: " << book->getISBN() << "\n";
                }
            }
        }
    }
};

Library* Library::instance = nullptr;

// 4. Factory class for Users and Books factory method
class UserFactory {
public:
    static User* createUser(int userType, const string& name) {
        if (userType == 1) {
            return new Student(name); //student object
        }
        else if (userType == 2) {
            return new Faculty(name); //faculty objwxr
        }
        else {
            throw invalid_argument("Invalid user type."); //throw invalid
        }
    }
};

class BookFactory {
public:
    static Book* createBook(const string& title, const string& author, const string& isbn) {
        return new Book(title, author, isbn); // create Book object
    }
};

void manageBooks(Library* library) {
    int choice;
    string title, author, isbn;
    while (true) {
        cout << "\n> Manage Books:\n> 1. Add a Book\n> 2. Go Back\n";
        cout << "\n> Enter your choice: ";
        cin >> choice;
        cin.ignore(); 
        //choice 1 for managing books adds books
        if (choice == 1) { //title
            cout << "Enter the Title (0 to cancel): ";
            getline(cin, title);
            if (title == "0") break; 

            cout << "Enter the Author (0 to cancel): "; //author
            getline(cin, author);
            if (author == "0") break;

            cout << "Enter the ISBN (0 to cancel): "; //isbn
            getline(cin, isbn);
            if (isbn == "0") break;

            Book* book = BookFactory::createBook(title, author, isbn);  //
            library->addBook(book);
            cout << "Book Added\n";
        }
        else if (choice == 2) {
            break;  // Go back to the previous menu
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

// For managing users
void manageUsers(Library* library) {
    int choice, userType;
    string name;

    while (true) {
        cout << "\n> Manage Users:\n> 1. Add a User\n> 2. Go Back\n";
        cout << "\n> Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            // For adding users
            while (true) {
                cout << "Enter 1 for student or 2 for faculty (0 to cancel): ";
                string userTypeStr;
                getline(cin, userTypeStr); // Read input as a string

                if (userTypeStr == "0") break;  // User chose to cancel

                try {
                    userType = stoi(userTypeStr); // Try converting the string to an integer
                    if (userType == 1 || userType == 2) {
                        break;  // Valid input, break loop
                    }
                    else {
                        cout << "!! ERROR: Only valid options are 1 or 2\n";  // Error message
                    }
                }
                catch (const invalid_argument&) {
                    cout << "!! ERROR: Invalid input. Please enter a number (1 or 2) or 0 to cancel.\n";
                }
            }

            if (userType == 0) break;  // Exit if cancels

            cout << "Enter name (0 to cancel): ";
            getline(cin, name);
            if (name == "0") break;  // Exit if cancels

            // Try and catch statement
            try {
                User* user = UserFactory::createUser(userType, name);
                library->addUser(user);
                cout << "User Added with ID " << user->getUserID() << "\n";
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 2) {
            break;  // Go back
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}




void manageTransactions(Library* library) {
    int choice;
    while (true) {
        cout << "\n> Manage Transactions:\n> 1. Check Out A Book\n> 2. Check In A Book\n> 3. List All Books\n> 4. List All Users\n> 5. Go Back\n";
        cout << "\n> Enter your choice: ";
        cin >> choice;
        cin.ignore(); //  transaction menu

        if (choice == 1) {
            string title;
            int userID;

            while (true) {
                cout << "\nEnter Book Title (or 0 to cancel): ";
                getline(cin, title);

                if (title == "0") break;  // Exit if cancels

                Book* book = library->findBookByTitle(title);
                if (!book) {
                    cout << "!! Error: No book with that title exists !!\n";
                    continue;  // Ask again 
                }

                while (true) {
                    cout << "Enter User ID (or x to cancel): ";
                    string userIDStr;
                    getline(cin, userIDStr);

                    if (userIDStr == "x") break;  // Exit if cancels

                    try {
                        userID = stoi(userIDStr);  // Convert id string to integer
                    }
                    catch (const invalid_argument&) {
                        cout << "!! Error: Invalid User ID. Please enter a valid number.\n";
                        continue;  // Ask again 
                    }

                    User* user = library->findUserByID(userID);
                    if (!user) {
                        cout << "!! Error: No User with that ID Exists !!\n";
                        continue;  // Ask again 
                    }

                    // If everything is valid, perform the transaction
                    if (book->isAvailable()) {
                        user->borrowBook(book);
                        cout << title << " checked out by User " << userID << "\n";
                    }
                    else {
                        cout << "Book is not available for borrowing.\n";
                    }

                    break;  // Exit the loop after transcation
                }

                if (title == "0") break;  // Exit if cancels
                break;  // Exit the loop after
            }
        }
        else if (choice == 2) {
            // Check in book
            string title;
            int userID;

            while (true) {
                cout << "\n Enter Book Title (or 0 to cancel): ";
                getline(cin, title);

                if (title == "0") break;  // Exit if  cancels

                Book* book = library->findBookByTitle(title);
                if (!book) {
                    cout << "!! Error: No book with that title exists !!\n";
                    continue;  // Ask again 
                }

                while (true) {
                    cout << " Enter User ID (or x to cancel): ";
                    string userIDStr;
                    getline(cin, userIDStr);

                    if (userIDStr == "x") break;  // Exit  cancels

                    try {
                        userID = stoi(userIDStr);  // Convert id string to integer
                    }
                    catch (const invalid_argument&) {
                        cout << "!! Error: Invalid User ID. Please enter a valid number.\n";
                        continue;  // Ask again 
                    }

                    User* user = library->findUserByID(userID);
                    if (!user) {
                        cout << "!! Error: No User with that ID Exists !!\n";
                        continue;  // Ask again 
                    }

                    // Check if the user has borrowed the book
                    bool hasBorrowedBook = false;
                    for (Book* borrowedBook : user->getBorrowedBooks()) {
                        if (borrowedBook->getTitle() == title) {
                            hasBorrowedBook = true;
                            break;
                        }
                    }

                    if (!hasBorrowedBook) {
                        cout << "!! Error: User has not borrowed this book !!\n";
                        continue;  // Ask again 
                    }

                    // If the book was borrowed, return it
                    user->returnBook(book);
                    cout << title << " has been checked in by User " << userID << "\n";
                    break;  // Exit the loop 
                }

                if (title == "0") break;  // Exit if user cancels the book title
                break;  // Exit the loop
            }
        }
        else if (choice == 3) {
            library->listBooks();
        }
        else if (choice == 4) {
            library->listUsers();  // Lists users with books
        }
        else if (choice == 5) {
            break;  // Go back to menu
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}


int main() {
    Library* library = Library::getInstance();
    int mainChoice;

    while (true) {
        cout << "\nWelcome to the Library:\n"
            << "1. Manage Books\n"
            << "2. Manage Users\n"
            << "3. Manage Transactions\n"
            << "4. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> mainChoice;

        if (mainChoice == 1) {
            manageBooks(library);
        }
        else if (mainChoice == 2) {
            manageUsers(library);
        }
        else if (mainChoice == 3) {
            manageTransactions(library);
        }
        else if (mainChoice == 4) {
            cout << "\nThank you for using the Library System!\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
