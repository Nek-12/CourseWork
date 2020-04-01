//#define NDEBUG
#pragma once
#include <map>
#include "table_printer.h"
#include <set>
#include <iostream>
#include <utility>
#include <memory>
using ull = unsigned long long;

enum
{
    WAIT_TIME_SMALL = 500,
    WAIT_TIME_MID = 750,
    WAIT_TIME_NORMAL = 1000,
    WAIT_TIME_LONG = 2000
};

//TODO: Update comments

using tprinter::TablePrinter; //allows to use the table printer namespace

extern std::string path; //Path to the program folder, see main.cpp -> int main()

class Entry;
class Genre;
class Author;
class Data;
class Book;
class Account;
class User;
class Admin;

ull genID();
void cls();
bool checkString(const std::string&, char);
std::string lowercase(const std::string& );
void sleep(const unsigned& ms);
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode);
//allows for reading a line from the iostream object with input check (foolproofing)
// 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for passwords

class Entry
{
    friend class Data;
    friend std::ostream& operator<<(std::ostream&, const Entry&); //Uses to_string that is virtual, applicable for any entry
    friend bool operator== (const Entry& lhs, const Entry& rhs) { return lhs.no == rhs.no; }
public:
    Entry() = delete; //No blank entries
    Entry(const Entry& e) = delete; //No copies!
    Entry(Entry&& e) noexcept: no(e.no), title(std::move(e.title)) {} //Moving is allowed
    Entry& operator=(const Entry& ) = delete; //No copying the same entry
    explicit Entry(std::string n, const ull& id = genID() ): no(id), title(std::move(n)) {}
    virtual ~Entry() = default;
    explicit operator ull() const { return no; } //for convenience
    explicit operator std::string() const { return title; } //for convenience

    [[nodiscard]] const ull& id() const {return no;}
    [[nodiscard]] std::string name() const { return title; }
    [[nodiscard]] virtual std::string to_string() const = 0; //for operator<<
    bool check(const std::string& s) {return (this->to_string().find(s) == std::string::npos);}; //checks this with anything unlike operator==
    void rename(const std::string& s) { title = s; }
protected:

private:
    ull no; //unique id
    std::string title;
};

class Book: public Entry
{
    friend class Genre;
    friend class Author;
    friend class Data;
public:
    explicit Book(const std::string& n, unsigned y, const ull& no = genID() ): Entry(n, no), year(y) {}
    Book(Book&& b) noexcept: Entry( std::move(b)), year(b.year), authors(std::move(b.authors)), genres(std::move(b.genres)) {}
    ~Book() override;


    void addAuthor(Author&); //TODO: Use overloaded link and unlink functions
    void addGenre(Genre&); //TODO: Move to pointers. At any cost.
    void remAuthor(Author&);
    void remGenre(Genre&);
    [[nodiscard]] std::string to_string() const override;
private:
    unsigned year = 0;
    std::set<ull> authors;
    std::set<ull> genres;
};

class Author: public Entry
{
    friend class Book;
    friend class Genre;
    friend class Data;
public:
    explicit Author(const std::string& n, std::string d, std::string c, const ull& no = genID()):
    Entry(n,no), date(std::move(d)), country(std::move(c)) {}

    Author(Author&& a) noexcept:
    Entry(std::move(a)), date(std::move(a.date)), country(std::move(a.country)), books(std::move(a.books)), genres(std::move(a.genres)) {};

    ~Author() override;

    void addGenre(Genre&);
    void addBook(Book&);
    void remGenre(Genre&);
    void remBook(Book&);
    [[nodiscard]] std::string to_string() const override;
private:
    std::string date;
    std::string country;
    std::set<ull> books;
    std::set<ull> genres;
};

class Genre: public Entry
{
    friend class Book;
    friend class Author;
    friend class Data;
public:
    using Entry::Entry;
    Genre(Genre&& g) noexcept: Entry(std::move(g)), books(std::move(g.books)), authors(std::move(g.authors)) {}
    ~Genre() override;

    void addAuthor(Author&);
    void addBook(Book&);
    void remAuthor(Author&);
    void remBook(Book& );
    [[nodiscard]] std::string to_string() const override;
private:
    std::set<ull> books;
    std::set<ull> authors;
};

class Account : public Entry
{
public:
    explicit Account(std::string n, std::string p, const ull& id = genID() ): Entry(std::move(n),id), pass(std::move(p)) {}
    Account(Account&& a) noexcept: Entry(std::move(a)), pass(std::move(a.pass)) {}
    friend bool operator== (const Account& lhs, const Account& rhs) { return (lhs.name() == rhs.name() && lhs.pass == rhs.pass ); } //TODO: ???
    explicit operator std::pair<std::string,std::string>() { return std::make_pair(name(), pass); }
    [[nodiscard]] std::string to_string() const override = 0; //for operator<<

private:
    std::string pass;
};

class User: public Account
{
public:
    using Account::Account;
    User(User&& u) noexcept: Account(std::move(u)), books(std::move(u.books)) {}
    bool addBook(const ull& b) { return books.insert(b).second; }
    bool remBook(const ull& b) { return (books.erase(b) != 0 ); }
    [[nodiscard]] std::string to_string() const override;
private:
    std::set<ull> books;
};

class Admin: public Account
{
public:
    explicit Admin(std::string n, std::string p, std::string s = "Default", const ull& id = genID() ):
    Account(std::move(n),std::move(p),id), status(std::move(s)) {}
    Admin(Admin&& a) noexcept: Account(std::move(a)) {}

    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string getStatus() const { return status; }
    void changeStatus(const std::string& s) { status = s; }
private:
    std::string status;
};

//template<typename T>
//class Journal
//{
//public:
//    bool insert(T&& e ); //Move only
//    bool erase(T& e );
//    std::shared_ptr<T>& find(ull id); //TODO: ??
//private:
//    static bool compare(const std::shared_ptr<T> &lhs, const std::shared_ptr<T> &rhs) { return lhs->id() < rhs->id(); }
//    std::set<std::shared_ptr<T>, decltype(compare)*> entries{compare};
//};

class Data
{
public:
    Data(Data const&) = delete; //No copying, moving!
    void operator=(Data const&) = delete; //No copying!

    static Data& getInstance() //Returns a reference to the single static instance of Data.
    {
        static Data instance;
        return instance;
    }
    bool add(Entry& );
    bool remove(Entry& );
    void load();
    void save();
    bool replace(const Entry& e); //replace e in the container of e's
    void print(char what); //'b' for books, 'a' for authors, 'g' for genres 'U' for users 'A' for admins
    size_t amount(char);
    Entry& fetch(const Entry& what);

    bool passCheck(const std::string& l, const std::string& p, const bool& isadmin);
    bool loginCheck(std::string& s, bool isadmin);

    void changePass(const std::string& l, const std::string& p, const bool& isadmin);
    std::vector <Book*> searchBook(const std::string& s);

    const std::string loginprompt = "Enter the login or \"exit\" to exit:";
    const std::string passprompt = "Enter the password or \"exit\" to exit:";
    const std::string passconfirm = "Confirm the password or enter \"exit\" to exit: ";
    static std::map<ull, Book> books;
    static std::map<ull, Author> authors;
    static std::map<ull, Genre> genres;
    static std::map<ull, Admin> admins;
    static std::map<ull, User> users;
private:
    Data() = default;
    static void ensureFileExists(const std::string& f);
};