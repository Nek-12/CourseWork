//#define NDEBUG
#pragma once
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include "fort.hpp"
#include <utility>
//#define WINDOWS

using ull = unsigned long;

enum
{
    WAIT_TIME_SMALL = 500,
    WAIT_TIME_MID = 750,
    WAIT_TIME_NORMAL = 1000,
    WAIT_TIME_LONG = 2000
};

//TODO: Update comments
//TODO: Add move-constuctors for huge perf gain
//TODO: Write a log class and put everything into it;

extern std::string path; //Path to the program folder, see main.cpp -> int main()

class Genre;
class Author;
class Data;
class Book;
ull genID();
void cls();
unsigned getCurYear();
bool checkString(const std::string&, char);
std::string lowercase(const std::string&);
void sleep(const unsigned&); // const unsigned - milliseconds to sleep, uses std::this_thread::sleep_for
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode);
//allows for reading a line from the iostream object with input check (foolproofing)
// 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for passwords

class Book //contains data about book entries
{
    friend class Author;
    friend class Genre;
    friend class Data;
    friend std::ostream& operator<<(std::ostream&, const Book&);
public:
    Book() = delete;
    Book(const Book& b) : id(b.id), name(b.name), year(b.year)
    {
        addToGenres(b);
        addToAuthors(b);
        std::cout << name << " was copy-constructed\n";
    };
    explicit Book(ull n, std::string t, unsigned y = 0) : id(n), name(std::move(t)), year(y)
    {
        std::cout << name << " was created \n";
    };
    Book& operator=(const Book& rhs) = delete;
    ~Book();

    void addAuthor(Author&);
    void addGenre(Genre&);
    void remAuthor(Author&);
    void remGenre(Genre&);
    [[nodiscard]] const std::string& getName() const
    { return name; }
    void rename(const std::string& s)
    { name = s; }
    std::map<ull, Author*>& getAuthors()
    { return authors; } //TODO: Very bad
    std::map<ull, Genre*>& getGenres()
    { return genres; }
    [[nodiscard]] unsigned int getYear() const
    { return year; }
    void setYear(unsigned int y)
    { year = y; }
private:
    void addToGenres(const Book&);
    void addToAuthors(const Book&);
    void remFromGenres();
    void remFromAuthors();
    bool check(const std::string& s);

    ull id; //unique book number
    std::string name;//TODO: Refactor this to work with vectors, maps with *s are bs
    unsigned year = 0;
    std::map<ull, Author*> authors;
    std::map<ull, Genre*> genres;

};

class Author
{
    friend class Book;
    friend class Genre;
    friend class Data;

    friend std::ostream& operator<<(std::ostream&, const Book&);
    friend std::ostream& operator<<(std::ostream& os, const Author& a);
public:
    Author() = delete;
    Author(const Author& a) : id(a.id), name(a.name), country(a.country), date(a.date)
    {
        addToGenres(a);
        addToBooks(a);
        std::cout << name << " was copy-constructed\n";
    };
    explicit Author(ull no, std::string n, std::string d = "Unknown", std::string c = "Unknown") : //TODO: Bad
            id(no), name(std::move(n)), country(std::move(c)), date(std::move(d))
    {
        std::cout << name << " was created\n";
    };
    Author& operator=(const Author& rhs) = delete;
    ~Author();

    void addGenre(Genre&);
    void addBook(Book&);
    void remGenre(Genre&);
    void remBook(Book&);
    bool check(const std::string& s);
    [[nodiscard]] const std::string& getName() const
    { return name; }
    void rename(const std::string& s)
    { name = s; }
    std::map<ull, Book*>& getBooks()
    { return books; } //TODO: Very bad
    std::map<ull, Genre*>& getGenres()
    { return genres; }
private:
    void addToBooks(const Author&);
    void addToGenres(const Author&);
    void remFromBooks();
    void remFromGenres();

    ull id;
    std::string name;
    std::string country;
    std::string date;
    std::map<ull, Book*> books;
    std::map<ull, Genre*> genres;
};

class Genre
{
    friend class Author;
    friend class Book;
    friend class Data;

    friend std::ostream& operator<<(std::ostream&, const Book&);
    friend std::ostream& operator<<(std::ostream&, const Genre&);
public:
    Genre() = delete;
    Genre(const Genre& g) : id(g.id), name(g.name)
    {
        std::cout << name << " was copy-constructed\n";
        addToBooks(g);
        addToAuthors(g);
    };
    explicit Genre(ull no, std::string n) : id(no), name(std::move(n))
    {
        std::cout << name << " was created\n";
    };
    Genre& operator=(const Genre& rhs) = delete;
    ~Genre();

    void addAuthor(Author&);
    void addBook(Book&);
    void remAuthor(Author&);
    void remBook(Book&);
    bool check(const std::string& s);
    [[nodiscard]] const std::string& getName() const
    { return name; }
    void rename(const std::string& s)
    { name = s; }
    [[nodiscard]] const std::map<ull, Book*>& getBooks() const
    { return books; } //TODO: Very bad
    [[nodiscard]] const std::map<ull, Author*>& getAuthors() const
    { return authors; }
private:
    void addToBooks(const Genre&);
    void addToAuthors(const Genre&);
    void remFromBooks();
    void remFromAuthors();

    ull id;
    std::string name;
    std::map<ull, Book*> books;
    std::map<ull, Author*> authors;
};

class Data // SINGLETON for storing all the nested structures
{
    friend class Book; //TODO: Move all the definitions to the cpp to cleanup
    friend class Genre;
    friend class Author;
public:
    Data(Data const&) = delete; //Deleted because it's a singleton. We use & instead
    void operator=(Data const&) = delete; //No copying!
    static Data& getInstance() //Returns a reference to the single static instance of Data.
    {
        static Data instance;
        return instance;
    }
    void load();
    void save(); //Writes the data to the files (books.txt etc.)
    void printbooks();
    Book* add(const Book& o)
    {
        auto it = sb.emplace(o.id,o);
        return (it.second ? &it.first->second: nullptr);
    }
    Genre* add(const Genre& o)
    {
        auto it = sg.emplace(o.id,o);
        return (it.second ? &it.first->second: nullptr);
    }
    Author* add(const Author& o)
    {
        auto it = sa.emplace(o.id,o);
        return (it.second ? &it.first->second: nullptr);
    }
    size_t erase(const Book& o) {return sb.erase(o.id);}
    size_t erase(const Genre& o) {return sg.erase(o.id);}
    size_t erase(const Author& o) {return sa.erase(o.id);}

    void printCredentials(bool isadmin); //Just prints all the USERNAMES in the mu or ma
    bool delAccount(const std::string& l, const bool& isadmin);
    bool passCheck(const std::string& l, const std::string& p, const bool& isadmin);
    bool loginCheck(std::string& s, bool isadmin);
    void createAccount(const std::string& l, const std::string& p, const bool& isadmin);
    size_t enumAccounts(bool isadmin);
    void changePass(const std::string& l, const std::string& p, const bool& isadmin);
    std::vector<Book*> searchBook(const std::string& s);

    const std::string loginprompt = "Enter the login or \"exit\" to exit:";
    const std::string passprompt = "Enter the password or \"exit\" to exit:";
    const std::string passconfirm = "Confirm the password or enter \"exit\" to exit: ";
    std::vector<Genre*> searchGenre(const std::string& s);
    void printAuthors();
    void printGenres(unsigned);
    std::vector<Author*> searchAuthor(const std::string& s);
private:
    Data() = default;
    static void ensureFileExists(const std::string& f);

    std::map<ull, Genre> sg;
    std::map<ull, Author> sa;
    std::map<ull, Book> sb; //Contains all the Books in the database
    std::map<std::string, std::string> mu; // holds <login, password> (hashed)
    std::map<std::string, std::string> ma; //same
};