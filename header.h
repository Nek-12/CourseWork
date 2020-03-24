#define NDEBUG
#ifndef LAB5_HEADER_H
#define LAB5_HEADER_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "sha256.h"
#include <iomanip>
#include "table_printer.h"
#include <thread>
#include <regex>
#include <unordered_set>

//TODO: Update comments

using tprinter::TablePrinter; //allows to use the tableprinter namespace
typedef unsigned long nat;

extern std::string path; //Path to the program folder, see main.cpp -> int main()

void sleep(const nat& ); // const nat - milliseconds to sleep, uses std::this_thread::sleep_for
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode); //allows for reading a line from the iostream object with input check (foolproofing)
// 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for passwords


class Genre;
class Author;

class Book //contains data about books entries
{
    friend class Author;
    friend class Genre;
public:
    Book() = delete;
    explicit Book(const nat& n, std::string  t, std::string d): id(n), title(std::move(t)), date(std::move(d)) {};
    Book& operator=(const Book& rhs);
    ~Book();

    void addAuthor(Author& );
    void addGenre(Genre& );
    void remAuthor(Author& );
    void remGenre(Genre& );
    void print();

private:
    void addToGenres(const Book& );
    void addToAuthors(const Book& );
    void remFromGenres();
    void remFromAuthors();

    nat id; //unique book number
    std::string title;
    std::string date; //The date the book was CREATED
    std::unordered_set<Author*> authors;
    std::unordered_set<Genre*> genres;
};

class Author
{
    friend class Book;
    friend class Genre;
public:
    Author() = delete;
    explicit Author(const nat& id, std::string n, std::string c, std::string d): id(id), name(std::move(n)),country(std::move(c)),date(std::move(d)) {};
    Author& operator=(const Author& rhs);
    ~Author();

    void addGenre(Genre& );
    void addBook(Book& );
    void remGenre(Genre& );
    void remBook(Book& );
    void print();

private:
    void addToBooks(const Author& );
    void addToGenres(const Author& );
    void remFromBooks();
    void remFromGenres();

    nat id;
    std::string name;
    std::string country;
    std::string date;
    std::unordered_set<Book*> books;
    std::unordered_set<Genre*> genres;

};

class Genre
{
    friend class Author;
    friend class Book;
public:
    Genre() = delete;
    explicit Genre(const nat& id, std::string n): id(id), name(std::move(n)) {};
    Genre& operator=(const Genre& rhs);
    ~Genre();

    void addAuthor(Author& );
    void addBook(Book& );
    void remAuthor(Author& );
    void remBook(Book& );
    void print();

private:
    void addToBooks(const Genre& );
    void addToAuthors(const Genre& );
    void remFromBooks();
    void remFromAuthors();

    nat id;
    std::string name;
    std::unordered_set<Book*> books;
    std::unordered_set<Author*> authors;
};

class Data // SINGLETON for storing all the nested structures
{
private:
    Data();

    //Actual Data
    std::vector<Book> vbooks; //Contains all the Books in the database
    std::map<std::string, std::string> mapuser; // holds <login, password> (hashed)
    std::map<std::string, std::string> mapadm; //same

public:
    friend class Book; //to use Books in functions
    Data(Data const&) = delete; //Deleted because it's a singleton. We use & instead
    void operator=(Data const&) = delete; //No copying!

    static Data& getInstance() //Returns a reference to the single static instance of Data.
    {
        static Data Instance;
#ifdef DEBUG
        std::cout << "Got instance of data" << std::endl; //for debugging
#endif
        return Instance;
    }

    void save(); //Writes the data to the files (books.txt etc.)
    bool loginCheck(std::string& s, bool isadmin); //Checks if the login of the string s matches its (hashed) password
    bool passCheck(const std::string& l, const std::string& p, bool isadmin); //same as loginCheck, but for password
    //1 for admin, 0 for user - bool isAdmin
    bool uinit(); //Reads the data from "user.txt" and puts it into the Data::mapuser
    bool bookinit(); //Reads the data from "books.txt" and puts it into the Data::vBooks
    bool adminit(); //Reads the data from "admin.txt" and puts it into the Data::mapadm
    void printbooks();
    void printCredentials(bool isAdmin); //Just prints all the USERNAMES in the mapuser or mapadm

    std::map<std::string, std::string>& muser() //We should not allow access to our data members - they are private,
                                                //but these functions are public. Data abstraction, that is.
    { return mapuser; }

    std::map<std::string, std::string>& madm()
    { return mapadm; }

    std::vector<Book>& vBooks()
    { return vbooks; }
};


#endif //LAB5_HEADER_H