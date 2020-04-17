//#define NDEBUG
#pragma once

#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "fort.hpp"
//#define WINDOWS
#ifndef WINDOWS
int getch();
#endif

using ull = unsigned long;

enum
{
    WAIT_TIME_SMALL = 750,
    WAIT_TIME_NORMAL = 1000,
    WAIT_TIME_LONG = 2000
};

//TODO: Update comments
//TODO: Add move-constuctors for huge perf gain

extern std::string path; //Path to the program folder, see main.cpp -> int main()

class Data;
class Entry;
class Book;
class Author;
class Genre;

ull genID();
void cls();
unsigned getCurYear();
bool checkString(const std::string&, char);
std::string lowercase(const std::string&);
void sleep(const unsigned&); // milliseconds to sleep
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode);
//allows for reading a line from the iostream object with input check (foolproofing)
// 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for passwords

class Entry
{
    friend std::ostream& operator<<(std::ostream& os, const Entry& e)
    {
        os << e.to_string();
        return os;
    }//Uses to_string that is virtual, applicable for any entry
    friend bool operator==(const Entry& lhs, const Entry& rhs)
    { return lhs.id() == rhs.id(); } //applicable to any entry because of dynamic binding
public:
    Entry() = delete; //No blank entries
    Entry& operator=(const Entry&) = delete; //No copying the same entry
    virtual ~Entry() = default;
    explicit operator ull() const
    { return no; } //for convenience

    [[nodiscard]] const ull& id() const
    { return no; }
    [[nodiscard]] std::string getName() const
    { return name; }
    void rename(const std::string& s)
    { name = s; }
    [[nodiscard]] virtual std::string to_string() const = 0;
    //virtual void print() = 0;
    [[nodiscard]] virtual bool check(const std::string& s) const = 0;
protected:
    Entry(const Entry& e) = default;//TODO: Remove copying functionality?
    explicit Entry(std::string n, const ull& id) : no(id), name(std::move(n))
    {}
private:
    ull no; //unique id
    std::string name;
};

class Book : public Entry
{
    friend class Genre;
    friend class Author;
    friend class Data;
public:
    ~Book() override;
    Book(const Book& b) : Entry(b.getName(), b.id()), year(b.year)
    {
        addToGenres(b);
        addToAuthors(b);
        std::cout << getName() << " was copy-constructed\n";
    };
    explicit Book(const std::string& t, const unsigned& y = 0, const ull& n = genID()) : Entry(t, n), year(y)
    {
        std::cout << getName() << " was created \n";
    };

    void addAuthor(Author&);
    void addGenre(Genre&);
    void remAuthor(Author&);
    void remGenre(Genre&);
    //void print() override;
    [[nodiscard]] bool check(const std::string& s) const override;
    [[nodiscard]] std::string to_string() const override;

//    std::map<ull, Author*>& getAuthors()
//    { return authors; } //TODO: Very bad
//    std::map<ull, Genre*>& getGenres()
//    { return genres; }
//    [[nodiscard]] unsigned int getYear() const
//    { return year; }
    void setYear(unsigned int y)
    { year = y; }
private:
    void addToGenres(const Book&);
    void addToAuthors(const Book&);
    void remFromGenres();
    void remFromAuthors();

    unsigned year = 0;
    std::set<Author*> authors;
    std::set<Genre*> genres;
};

class Author : public Entry
{
    friend class Book;
    friend class Data;
public:
    ~Author() override;
    Author(const Author& a) : Entry(a.getName(), a.id()), country(a.country), date(a.date)
    {
        addToBooks(a);
        std::cout << getName() << " was copy-constructed\n";
    };
    explicit Author(const std::string& n, std::string d, std::string c, const ull& id = genID()) : //TODO: Bad
            Entry(n, id), country(std::move(c)), date(std::move(d))
    {
        std::cout << getName() << " was created\n";
    };

    void addBook(Book&);
    void remBook(Book&);
    //void print();
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] bool check(const std::string& s) const override;
//    std::map<ull, Book*>& getBooks()
//    { return books; } //TODO: Very bad
private:
    void addToBooks(const Author&);
    void remFromBooks();

    std::string country;
    std::string date;
    std::set<Book*> books;
};

class Genre : public Entry
{
    friend class Book;
    friend class Data;
public:
    explicit Genre(std::string n, ull id = genID()) : Entry(std::move(n), id)
    {}
    ~Genre() override;
    Genre(const Genre& g) : Entry(g.getName(), g.id())
    {
        std::cout << getName() << " was copy-constructed\n";
        addToBooks(g);
    };

    void addBook(Book&);
    void remBook(Book&);
    [[nodiscard]] std::string to_string() const override;
    //void print();
    [[nodiscard]] bool check(const std::string& s) const override;
//    [[nodiscard]] const std::map<ull, Book*>& getBooks() const
//    { return books; } //TODO: Very bad
private:
    void addToBooks(const Genre&);
    void remFromBooks();

    std::set<Book*> books;
};


class Data // SINGLETON for storing all the nested structures
{
    friend class Book; //TODO: Move all the definitions to the cpp to cleanup
    friend class Genre;
    friend class Author;
public:
    Data(Data const&) = delete; //No copying, no moving!
    void operator=(Data const&) = delete; //No copying!
    static Data& getInstance() //Returns a reference to the single static instance of Data.
    {
        static Data instance;
        return instance;
    }
    void load(); //Loads all the data from several files
    void save(); //Writes the data to the files (books.txt etc.)
    void printBooks();
    void printAuthors();
    void printGenres(unsigned);
    Book* add(const Book& o)
    {
        auto it = mbooks.emplace(o.id(), o);
        return (it.second ? &it.first->second : nullptr);
    }
    Genre* add(const Genre& o)
    {
        auto it = mgenres.emplace(o.id(), o);
        return (it.second ? &it.first->second : nullptr);
    }
    Author* add(const Author& o)
    {
        auto it = mauthors.emplace(o.id(), o);
        return (it.second ? &it.first->second : nullptr);
    }
    size_t erase(const Book& o)
    { return mbooks.erase(o.id()); }
    size_t erase(const Genre& o)
    { return mgenres.erase(o.id()); }
    size_t erase(const Author& o)
    { return mauthors.erase(o.id()); }
    void printCredentials(bool isadmin); //Just prints all the usernames
    bool delAccount(const std::string& l, const bool& isadmin);
    bool passCheck(const std::string& l, const std::string& p, const bool& isadmin);
    bool loginCheck(const std::string& s, const bool& isadmin);
    void createAccount(const std::string& l, const std::string& p, const bool& isadmin);
    size_t enumAccounts(const bool& isadmin);
    void changePass(const std::string& l, const std::string& p, const bool& isadmin);
    std::vector<Book*> searchBook(const std::string& s);
    std::vector<Genre*> searchGenre(const std::string& s);
    std::vector<Author*> searchAuthor(const std::string& s);
private:
    Data() = default;
    static void ensureFileExists(const std::string& f);

    std::map<ull, Genre> mgenres;
    std::map<ull, Author> mauthors;
    std::map<ull, Book> mbooks; //Contains all the Books in the database
    std::map<std::string, std::string> users; // holds <login, password> (hashed)
    std::map<std::string, std::string> admins; //same
};

#define LOGINPROMPT  "Enter the login or \"exit\" to exit: "
#define PASSPROMPT  "Enter the password or \"exit\" to exit: "
#define PASSCONFIRM  "Confirm the password or enter \"exit\" to exit: "
#define ADMIN_CONSOLE_ENTRIES  "      :ADMIN:\n"\
                                                 "Select an option: "\
                                                 "\n1 -> Manage book data "\
                                                 "\n2 -> Change your password"\
                                                 "\n3 -> Register an administrator"\
                                                 "\n4 -> Delete users "\
                                                 "\n0 -> Delete your account (careful!)"\
                                                 "\nq -> Sign off"
#define USER_CONSOLE_ENTRIES  "      :USER:\n"\
                                                "\nSelect an option: "\
                                                "\n1 -> Manage book data "\
                                                "\n2 -> Change your password"\
                                                "\n0 -> Delete your account"\
                                                "\nq -> Sign off"
#define ADMIN_MANAGEMENT_ENTRIES  ":ADMIN:\n"\
                                                    "Select an option: "\
                                                    "\n1 -> Search anything "\
                                                    "\n2 -> Show data "\
                                                    "\n3 -> Add a new book"\
                                                    "\n4 -> Manage a book"\
                                                    "\n5 -> Add a new author"\
                                                    "\n6 -> Manage an author "\
                                                    "\n7 -> Add a new genre "\
                                                    "\n8 -> Manage a genre "\
                                                    "\nq -> Go back"
#define USER_MANAGEMENT_ENTRIES  ":USER:\n"\
                                                   "Select an option: "\
                                                   "\n1 -> Search anything"\
                                                   "\n2 -> Show data "\
                                                   "\nq -> Go back"
#define SHOW_DATA_MENU_ENTRIES  "Select an option: "\
                                                  "\n1 -> Show all books "\
                                                  "\n2 -> Show all authors "\
                                                  "\n3 -> Show all genres for a given year "\
                                                  "\nq -> Go back"
#define ANY_KEY  "Press any key to continue..."