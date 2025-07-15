#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int year;

public:
    Book() : id(0), year(0) {}
    Book(int i, const string& t, const string& a, int y) : id(i), title(t), author(a), year(y) {}

    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    int getYear() const { return year; }

    void setId(int i) { id = i; }
    void setTitle(const string& t) { title = t; }
    void setAuthor(const string& a) { author = a; }
    void setYear(int y) { year = y; }

    void display() const {
        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\nYear: " << year << endl;
    }

    string serialize() const {
        return to_string(id) + ";" + title + ";" + author + ";" + to_string(year);
    }

    static Book deserialize(const string& line) {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, ';')) parts.push_back(token);
        return Book(stoi(parts[0]), parts[1], parts[2], stoi(parts[3]));
    }
};

vector<Book> loadBooks(const string& filename) {
    vector<Book> books;
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) books.push_back(Book::deserialize(line));
    }
    return books;
}

bool saveBooks(const vector<Book>& books, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) return false;
    for (const auto& b : books) outFile << b.serialize() << "\n";
    return true;
}

void addRecord(const string& filename) {
    int id, year;
    string title, author;
    cout << "Enter ID: "; cin >> id; cin.ignore();
    cout << "Enter Title: "; getline(cin, title);
    cout << "Enter Author: "; getline(cin, author);
    cout << "Enter Year: "; cin >> year; cin.ignore();

    vector<Book> books = loadBooks(filename);
    for (const auto& b : books) {
        if (b.getId() == id) {
            cout << "ID already exists!\n"; return;
        }
    }

    books.emplace_back(id, title, author, year);
    if (saveBooks(books, filename)) cout << "Added successfully.\n";
    else cout << "Error saving data.\n";
}

void displayAllRecords(const string& filename) {
    vector<Book> books = loadBooks(filename);
    if (books.empty()) {
        cout << "No books available.\n";
        return;
    }
    for (const auto& b : books) {
        b.display();
        cout << "-------------------\n";
    }
}

void searchRecordByID(const string& filename) {
    int id; cout << "Enter ID to search: "; cin >> id; cin.ignore();
    vector<Book> books = loadBooks(filename);
    for (const auto& b : books) {
        if (b.getId() == id) {
            b.display();
            return;
        }
    }
    cout << "Book not found.\n";
}

void updateRecord(const string& filename) {
    int id; cout << "Enter ID to update: "; cin >> id; cin.ignore();
    vector<Book> books = loadBooks(filename);
    bool found = false;
    for (auto& b : books) {
        if (b.getId() == id) {
            found = true;
            string title, author;
            int year;
            cout << "New Title: "; getline(cin, title);
            cout << "New Author: "; getline(cin, author);
            cout << "New Year: "; cin >> year; cin.ignore();
            b.setTitle(title);
            b.setAuthor(author);
            b.setYear(year);
            break;
        }
    }
    if (found && saveBooks(books, filename)) cout << "Updated successfully.\n";
    else cout << "Book not found or error saving data.\n";
}

void deleteRecord(const string& filename) {
    int id; cout << "Enter ID to delete: "; cin >> id; cin.ignore();
    vector<Book> books = loadBooks(filename);
    auto it = remove_if(books.begin(), books.end(), [id](const Book& b) { return b.getId() == id; });
    if (it == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    books.erase(it, books.end());
    if (saveBooks(books, filename)) cout << "Deleted successfully.\n";
    else cout << "Error saving data.\n";
}

void sortRecords(const string& filename) {
    vector<Book> books = loadBooks(filename);
    int choice;
    cout << "Sort by: 1-Title, 2-Year\nChoice: ";
    cin >> choice; cin.ignore();
    if (choice == 1) {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.getTitle() < b.getTitle();
            });
    }
    else if (choice == 2) {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.getYear() < b.getYear();
            });
    }
    else {
        cout << "Invalid option.\n";
        return;
    }
    for (const auto& b : books) {
        b.display();
        cout << "-------------------\n";
    }
}

void exportToCSV(const string& filename, const string& csvFilename) {
    vector<Book> books = loadBooks(filename);
    ofstream out(csvFilename);
    if (!out) {
        cout << "Error creating CSV file.\n";
        return;
    }
    out << "ID,Title,Author,Year\n";
    for (const auto& b : books) {
        out << b.getId() << ",\"" << b.getTitle() << "\",\"" << b.getAuthor() << "\"," << b.getYear() << "\n";
    }
    cout << "Export completed successfully to " << csvFilename << endl;
}

int main() {
    string filename = "books.txt";
    int option;
    do {
        cout << "\nMenu:\n1.Add book\n2.Display all\n3.Search by ID\n4.Update\n5.Delete\n6.Sort\n7.Export CSV\n0.Exit\nChoice: ";
        cin >> option; cin.ignore();
        switch (option) {
        case 1: addRecord(filename); break;
        case 2: displayAllRecords(filename); break;
        case 3: searchRecordByID(filename); break;
        case 4: updateRecord(filename); break;
        case 5: deleteRecord(filename); break;
        case 6: sortRecords(filename); break;
        case 7: exportToCSV(filename, "books.csv"); break;
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (option != 0);
    return 0;
}
