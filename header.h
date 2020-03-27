//#define NDEBUG
#pragma once
#include <map>
#include "table_printer.h"
#include <unordered_set>
#include <vector>

enum
{
    WAIT_TIME_SMALL = 500,
    WAIT_TIME_MID = 750,
    WAIT_TIME_NORMAL = 1000,
    WAIT_TIME_LONG = 2000
};

//TODO: Update comments

using tprinter::TablePrinter; //allows to use the tableprinter namespace

extern std::string path; //Path to the program folder, see main.cpp -> int main()

class Genre;
class Author;
class Data;
class Book;
void cls();
std::string lowercase(const std::string& );
void sleep(const unsigned& ); // const unsigned - milliseconds to sleep, uses std::this_thread::sleep_for
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode);
//allows for reading a line from the iostream object with input check (foolproofing)
// 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for passwords

template<typename T>
typename std::vector<T>::iterator findName(std::vector<T>& vec, const std::string& title);



class Book //contains data about book entries
{
    friend class Author;
    friend class Genre;
    friend class Data;
    friend typename std::vector<Book>::iterator findName<Book>(std::vector<Book>&, const std::string&);
    friend void manageBook();
    friend std::ostream &operator<<(std::ostream &, const Book&);
public:
    Book() = delete;
    Book(const Book& b): id(b.id), name(b.name),year(b.year) {addToGenres(b); addToAuthors(b);} ;
    explicit Book(std::string n, std::string  t, unsigned y = 0): id(std::move(n)), name(std::move(t)), year(y) {};
    Book& operator=(const Book& rhs);
    ~Book();

    void addAuthor(Author& );
    void addGenre(Genre& );
    void remAuthor(Author& );
    void remGenre(Genre& );

    bool check(const std::string& s);

private:
    void addToGenres(const Book& );
    void addToAuthors(const Book& );
    void remFromGenres();
    void remFromAuthors();

    std::string id; //unique book number
    std::string name;
    unsigned year = 0;
    std::unordered_set<Author*> authors;
    std::unordered_set<Genre*> genres;

};

class Author
{
    friend class Book;
    friend class Genre;
    friend class Data;
    friend std::vector<Author>::iterator findName<Author>(std::vector<Author>&, const std::string&);
    friend std::ostream &operator<<(std::ostream &, const Book& );
    friend std::ostream& operator<<(std::ostream &os, const Author& a);
    friend void editBookAuthor(Book* );
public:
    Author() = delete;
    Author(const Author& a): id(a.id), country(a.country), date(a.date) { addToGenres(a), addToBooks(a);};
    explicit Author(std::string no, std::string n, std::string d = "Unknown", std::string c = "Unknown"):
    id(std::move(no)), name(std::move(n)),country(std::move(c)),date(std::move(d)) {};
    Author& operator=(const Author& rhs);
    ~Author();

    void addGenre(Genre& );
    void addBook(Book& );
    void remGenre(Genre& );
    void remBook(Book& );
    bool check(const std::string& s);

private:
    void addToBooks(const Author& );
    void addToGenres(const Author& );
    void remFromBooks();
    void remFromGenres();

    std::string id;
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
    friend class Data;
    friend std::vector<Genre>::iterator findName<Genre>(std::vector<Genre>&, const std::string&);
    friend std::ostream &operator<<(std::ostream&, const Book& );
    friend std::ostream &operator<<(std::ostream&, const Genre& );
public:
    Genre() = delete;
    Genre(const Genre& g): id(g.id), name(g.name) {addToBooks(g), addToAuthors(g);};
    explicit Genre(std::string  no, std::string n): id(std::move(no)), name(std::move(n)) {};
    Genre& operator=(const Genre& rhs);
    ~Genre();

    void addAuthor(Author& );
    void addBook(Book& );
    void remAuthor(Author& );
    void remBook(Book& );
    bool check(const std::string& s);

private:
    void addToBooks(const Genre& );
    void addToAuthors(const Genre& );
    void remFromBooks();
    void remFromAuthors();

    std::string id;
    std::string name;
    std::unordered_set<Book*> books;
    std::unordered_set<Author*> authors;

};

class Data // SINGLETON for storing all the nested structures
{
    friend class Book;
    friend class Genre;
    friend class Author;
    friend Book* newBook();
    friend void manageBook();
    friend void editBookAuthor(Book* );
public:
    Data(Data const&) = delete; //Deleted because it's a singleton. We use & instead
    void operator=(Data const&) = delete; //No copying!

    static Data& getInstance() //Returns a reference to the single static instance of Data.
    {
        static Data instance;
        return instance;
    }

    void save(); //Writes the data to the files (books.txt etc.)
    void printbooks();
    void printCredentials(bool isadmin); //Just prints all the USERNAMES in the muser or madm
    static std::string genId();
    bool delAccount(const std::string& l, const bool& isadmin);
    void uinit(); //Reads the data from "user.txt" and puts it into the Data::muser
    void bookinit(); //Reads the data from "books.txt" and puts it into the Data::vBooks
    void adminit(); //Reads the data from "admin.txt" and puts it into the Data::madm
    bool passCheck(const std::string& l, const std::string& p, const bool& isadmin);
    bool loginCheck(std::string& s, bool isadmin);
    void createAccount(const std::string& l, const std::string& p, const bool& isadmin);
    size_t enumAccounts(bool isadmin);
    void changePass(const std::string& l, const std::string& p, const bool& isadmin);
    std::vector<Book*> searchBook(const std::string& s);

    const std::string loginprompt = "\n Enter the login or \"exit\" to exit:";
    const std::string passprompt = "Enter the password or \"exit\" to exit:";
    const std::string passconfirm = "Confirm the password or enter \"exit\" to exit: ";

private:
    Data() = default;
    static void ensureFileExists(const std::string& f);

    std::vector<Genre> vgenres;
    std::vector<Author> vauthors;
    std::vector<Book> vbooks; //Contains all the Books in the database
    std::map<std::string, std::string> muser; // holds <login, password> (hashed)
    std::map<std::string, std::string> madm; //same

};

template<typename T>
typename std::vector<T>::iterator findName(std::vector<T>& vec, const std::string& title)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        if (it->name == title ) return it;
    }
    return vec.end();
}

template std::vector<Author>::iterator findName(std::vector<Author>&, const std::string&);
template std::vector<Genre>::iterator findName(std::vector<Genre>&, const std::string&);
template std::vector<Book>::iterator findName(std::vector<Book>&, const std::string&);