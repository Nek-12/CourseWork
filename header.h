//#define NDEBUG
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once
#include <map>
#include "table_printer.h"
#include <set>
#include <iostream>
#include <utility>
#include <memory>
using ull = unsigned long long;
template <typename T>
using sptr = std::shared_ptr<T>;
template <typename T>
using wptr = std::weak_ptr<T>;
template <typename T>
using uptr = std::unique_ptr<T>;

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
template <typename T> class Journal;

ull genID();
void cls();
bool checkString(const std::string&, char);
std::string lowercase(const std::string& );
void sleep(const unsigned& ms);
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode);
// 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for passwords, 'i' for numbers

class Entry
{
    friend class Journal<Entry>;
    friend bool operator< (Entry& lhs, Entry& rhs) { return lhs.id() < rhs.id(); }
    friend std::ostream& operator<<(std::ostream&, const Entry&); //Uses to_string that is virtual, applicable for any entry
    friend bool operator== (const Entry& lhs, const Entry& rhs) { return lhs.id() == rhs.id(); } //applicable to any entry because of dynamic binding
public:
    Entry() = delete; //No blank entries
    Entry(const Entry& e) = delete; //No copies!
    Entry& operator=(const Entry& ) = delete; //No copying the same entry
    virtual ~Entry() = default;
    explicit operator ull() const { return no; } //for convenience
    explicit operator std::string() const { return title; } //for convenience
    [[nodiscard]] const ull& id() const {return no;}
    [[nodiscard]] std::string name() const { return title; }
    [[nodiscard]] virtual std::string to_string(bool isUsingID) const = 0; //
    bool check(const std::string& s) {return (this->to_string(false).find(s) == std::string::npos);}
    void rename(const std::string& s) { title = s; }
protected:
    explicit Entry(std::string n, const ull& id = genID() ): no(id), title(std::move(n)) {}
private:
    ull no; //unique id
    std::string title;
};

class Book: public Entry, std::enable_shared_from_this<Book>
{
    friend class Journal<Book>;
    friend class Genre;
    friend class Author;
public:
    ~Book() override;

    void addAuthor(const sptr<Author>& );
    void addGenre(const sptr<Genre>& );
    void remAuthor(const sptr<Author>& );
    void remGenre(const sptr<Genre>& );
    [[nodiscard]] std::string to_string(bool) const override;

private:
    explicit Book(const std::string& n, unsigned y, const ull& no = genID() ): Entry(n, no), year(y) {}
    unsigned year = 0;
    std::set<wptr<Author>> authors;
    std::set<wptr<Genre>> genres;
};

class Author: public Entry, std::enable_shared_from_this<Author>
{
    friend class Journal<Author>;
    friend class Book;
    friend class Genre;
public:
    ~Author() override;

    void addGenre(const sptr<Genre>& );
    void addBook(const sptr<Book>&);
    void remGenre(const sptr<Genre>&);
    void remBook(const sptr<Book>&);
    [[nodiscard]] std::string to_string(bool) const override;
private:
    explicit Author(const std::string& n, std::string d, std::string c, const ull& no = genID()):
    Entry(n,no), date(std::move(d)), country(std::move(c)) {}
    std::string date;
    std::string country;
    std::set<wptr<Book>> books;
    std::set<wptr<Genre>> genres;
};

class Genre: public Entry, std::enable_shared_from_this<Genre>
{
    friend class Journal<Genre>;
    friend class Book;
    friend class Author;
public:
    ~Genre() override;

    void addAuthor(const sptr<Author>&);
    void addBook(const sptr<Book>&);
    void remAuthor(const sptr<Author>&);
    void remBook(const sptr<Book>& );
    [[nodiscard]] std::string to_string(bool) const override;
private:
    using Entry::Entry;
    std::set<wptr<Book>> books;
    std::set<wptr<Author>> authors;
};

class Account : public Entry
{
    friend class Journal<Account>;
public:
    friend bool operator== (const Account& lhs, const Account& rhs) { return (lhs.name() == rhs.name() && lhs.pass == rhs.pass ); } //TODO: ???
    explicit operator std::pair<std::string,std::string>() { return std::make_pair(name(), pass); }
    [[nodiscard]] std::string to_string(bool) const override = 0; //for operator<<
protected:
    explicit Account(std::string n, std::string p, const ull& id = genID() ): Entry(std::move(n),id), pass(std::move(p)) {}
private:
    std::string pass;
};

class User: public Account
{
    friend class Journal<User>;
public:
    bool addBook(const sptr<Book>& b) { return books.insert(b).second; }
    bool remBook(const sptr<Book>& b) { return (books.erase(b) != 0 ); }
    [[nodiscard]] std::string to_string(bool withPassword) const override;
private:
    using Account::Account;
    std::set<wptr<Book>> books;
};

class Admin: public Account
{
    friend class Journal<Admin>;
public:
    explicit Admin(std::string n, std::string p, std::string s = "Default", const ull& id = genID() ):
    Account(std::move(n),std::move(p),id), status(std::move(s)) {}
    [[nodiscard]] std::string to_string(bool withPassword) const override;
    [[nodiscard]] std::string getStatus() const { return status; }
    void changeStatus(const std::string& s) { status = s; }
private:
    std::string status;
};
template<typename T>
class Journal
{
public:
    bool erase(const sptr<T>& e );
    sptr<T> find(const ull& );
    std::vector<sptr<T>> multifind(const std::string& s);
    template<typename ...Args>
    uptr<T> emplace(Args&&... args)
    {
        return *entries.insert(std::make_unique(std::forward<Args>(args)...));
    }
private:
//    static bool compare(const sptr<T> &lhs, const sptr<T> &rhs) { return lhs->id() < rhs->id(); }
//    std::set<sptr<T>, decltype(compare)*> entries{compare};
std::vector<uptr<T> > entries;
};

class Data
{
    friend class Entry;
    friend class Book;
    friend class Genre;
    friend class Author;
    friend class Account;
    friend class Admin;
    friend class User;
public:
    Data(Data const&) = delete; //No copying, moving!
    void operator=(Data const&) = delete; //No copying!

    static Data& getInstance() //Returns a reference to the single static instance of Data.
    {
        static Data instance;
        return instance;
    }
    bool add(const std::string& s);
    bool remove(Entry& );
    void load();
    void save();
    bool replace(const Entry& e); //replace e in the container of e's
    void print(char what); //'b' for books, 'a' for authors, 'g' for genres 'U' for users 'A' for admins
    size_t amount(char);
    Entry& fetch(const Entry& what);

    bool passCheck(const std::string& l, const std::string& p, const bool& isadmin);
    bool loginCheck(std::string& s, bool isadmin);
    void createAccount(const std::string& l, const std::string& p, const bool& isadmin);

    void changePass(const std::string& l, const std::string& p, const bool& isadmin);
    std::vector <Book*> searchBook(const std::string& s);

    const std::string loginprompt = "Enter the login or \"exit\" to exit:";
    const std::string passprompt = "Enter the password or \"exit\" to exit:";
    const std::string passconfirm = "Confirm the password or enter \"exit\" to exit: ";
    Journal<Book> jbook;
    Journal<Author> jauthor;
    Journal<Genre> jgenre;
    Journal<User> juser;
    Journal<Admin> jadmin;

private:

    Data() = default;
    static void ensureFileExists(const std::string& f);
};
#pragma clang diagnostic pop