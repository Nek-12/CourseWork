#pragma once
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "fort.hpp"
#define NDEBUG
#ifndef __linux__
#include <conio.h>
#define CARRIAGE_RETURN_CHAR 13
#define BACKSPACE_CHAR 8
using ull = unsigned long long;
#define MAX_ID ULONG_LONG_MAX
#else
using ull = unsigned long;
#define MAX_ID ULONG_MAX
#define CARRIAGE_RETURN_CHAR 10
#define BACKSPACE_CHAR 127
#include <termios.h>
int getch();
#endif
#define MAX_ID_LENGTH 19

extern std::string path; //Path to the program folder, see main.cpp -> int main()

class Data;
class Entry;
class Book;
class Author;
class Genre;

void setTableProperties(fort::char_table&,unsigned,unsigned);
void pause();
ull stoid(const std::string& s);
std::string getPassword();
ull genID();
void cls();
unsigned getCurYear();
bool checkString(const std::string&, char);
std::string lowercase(const std::string&);
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
    Entry(const Entry& e) = delete; //No copies
    Entry& operator=(const Entry&) = delete; //No assigning
    virtual ~Entry() = default;
    explicit operator ull() const
    { return no; } //for convenience
    [[nodiscard]] const ull& id() const
    { return no; }
    [[nodiscard]] std::string getName() const
    { return name; }
    void rename(const std::string& s)
    { name = s; }
    virtual bool link(Entry* e) = 0;
    virtual bool unlink(Entry* e) = 0;
    [[nodiscard]] virtual std::string to_string() const = 0;
    [[nodiscard]] virtual bool check(const std::string& s) const = 0;
protected:
    Entry(Entry&& e) noexcept: no(e.no), name(std::move(e.name))
    {}
    explicit Entry(std::string n, const ull& id) : no(id), name(std::move(n))
    {
#ifndef NDEBUG
        std::cout << getName() << " was created \n";
#endif
    }
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
    Book(const Book& b) = delete;
    Book(Book&& b) noexcept;
    explicit Book(const std::string& t, const unsigned& y = 0, const ull& n = genID()) : Entry(t, n), year(y)
    {}
    [[nodiscard]] bool check(const std::string& s) const override;
    [[nodiscard]] std::string to_string() const override;
    bool link(Entry* pe) override;
    bool unlink(Entry* pe) override;
    void remGenre(const size_t& pos);
    void remAuthor(const size_t& pos);
    [[nodiscard]] size_t enumAuthors() const
    { return authors.size(); }
    [[nodiscard]] size_t enumGenres() const
    { return genres.size(); }
    [[nodiscard]] unsigned getYear() const
    { return year; }
    void setYear(unsigned int y)
    { year = y; }
private:
    unsigned year = 0;
    std::set<Entry*> authors;
    std::set<Entry*> genres;
};

class Author : public Entry
{
    friend class Book;
    friend class Data;
public:
    ~Author() override;
    Author(Author&& a) noexcept;
    explicit Author(const std::string& n, std::string d, std::string c, const ull& id = genID()) :
            Entry(n, id), country(std::move(c)), date(std::move(d))
    {}

    bool link(Entry* pe) override;
    bool unlink(Entry* pe) override;
    void remBook(const size_t& pos);
    void setCountry(const std::string& c)
    { country = c; }
    void setDate(const std::string& c)
    { date = c; }
    [[nodiscard]] size_t enumBooks() const
    { return books.size(); }
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] bool check(const std::string& s) const override;
private:
    std::string country;
    std::string date;
    std::set<Entry*> books;
};

class Genre : public Entry
{
    friend class Book;
    friend class Data;
public:
    explicit Genre(std::string n, ull id = genID()) : Entry(std::move(n), id)
    {}
    ~Genre() override;
    Genre(Genre&& g) noexcept;
    bool link(Entry* pe) override;
    bool unlink(Entry* pe) override;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] bool check(const std::string& s) const override;
    [[nodiscard]] size_t enumBooks() const
    { return books.size(); }
private:
    std::set<Entry*> books;
};

class Data // SINGLETON for storing all the nested structures
{
    friend class Book;
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
    void printGenres(unsigned = getCurYear());
    void printCredentials(bool isadmin);
    std::vector<Entry*> search(const std::string& s);
    template <typename ...Args>
    Book* addBook(const Args& ...args)
    {
        auto it = mbooks.try_emplace(args...);
        return (it.second ? &it.first->second : nullptr);
    }
    template<typename ...Args>
    Author* addAuthor(const Args& ...args)
    {
        auto it = mauthors.try_emplace(args...);
        return (it.second ? &it.first->second : nullptr);
    }
    template<typename ...Args>
    Genre* addGenre(const Args& ...args)
    {
        auto it = mgenres.try_emplace(args...);
        return (it.second ? &it.first->second : nullptr);
    }
    bool erase(Book& e)
    { return mbooks.erase(e.id()); }
    bool erase(Author& e)
    { return mbooks.erase(e.id()); }
    bool erase(Genre& e)
    { return mbooks.erase(e.id()); }
    bool delAccount(const std::string& l, const bool& isadmin);
    bool passCheck(const std::string& l, const std::string& p, const bool& isadmin)
    { return ((isadmin ? admins.find(l) : users.find(l))->second == hash(p)); }
    bool loginCheck(const std::string& s, const bool& isadmin)
    { return (isadmin ? admins.find(s) != admins.end() : users.find(s) != users.end()); }
    bool addAccount(const std::string& l, const std::string& p, const bool& isadmin)
    { return (isadmin ? admins : users).try_emplace(l, hash(p)).second; }
    size_t enumAccounts(const bool& isadmin)
    { return (isadmin ? admins.size() : users.size()); }
    void changePass(const std::string& l, const std::string& p, const bool& isadmin)
    { (isadmin ? admins : users)[l] = hash(p); }

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
#define ADMIN_CONSOLE_ENTRIES    ":ADMIN:"\
                                 "\nSelect an option: "\
                                 "\n1 -> Manage book data "\
                                 "\n2 -> Change your password"\
                                 "\n3 -> Register an administrator"\
                                 "\n4 -> Delete users "\
                                 "\n0 -> Delete your account (careful!)"\
                                 "\nq -> Sign off"
#define USER_CONSOLE_ENTRIES    ":USER:"\
                                "\nSelect an option: "\
                                "\n1 -> Manage book data "\
                                "\n2 -> Change your password"\
                                "\n0 -> Delete your account"\
                                "\nq -> Sign off"
#define ADMIN_MANAGEMENT_ENTRIES    ":ADMIN:"\
                                    "\nSelect an option: "\
                                    "\n1 -> Search anything "\
                                    "\n2 -> Show data "\
                                    "\n3 -> Manage entries "\
                                    "\n4 -> Add a new book"\
                                    "\n5 -> Add a new author"\
                                    "\n6 -> Add a new genre "\
                                    "\nq -> Go back"
#define USER_MANAGEMENT_ENTRIES    ":USER:"\
                                   "\nSelect an option: "\
                                   "\n1 -> Search anything"\
                                   "\n2 -> Show data "\
                                   "\nq -> Go back"
#define SHOW_DATA_MENU_ENTRIES    "Select an option: "\
                                  "\n1 -> Show all books "\
                                  "\n2 -> Show all authors "\
                                  "\n3 -> Show all genres for a given year "\
                                  "\nq -> Go back"
#define WELCOME_MENU "Welcome. "\
                     "\n1 -> User sign in "\
                     "\n2 -> Admin sign in "\
                     "\nq -> Save and exit"
#define EDIT_GENRE_OPTIONS "Select an option: "\
                             "\n1 -> Rename this genre "\
                             "\n2 -> Add this genre to books "\
                             "\n3 -> Remove this genre from books"\
                             "\n4 -> Delete this genre "\
                             "\nq -> Go back"
#define EDIT_BOOK_OPTIONS   "What would you like to do? "\
                            "\n1 -> Edit title"\
                            "\n2 -> Edit publishing year"\
                            "\n3 -> Edit entries"\
                            "\n4 -> Delete this book"\
                            "\nq -> Nothing"
#define EDIT_AUTHOR_OPTIONS "What would you like to do? "\
                            "\n1 -> Edit name"\
                            "\n2 -> Edit birthdate"\
                            "\n3 -> Edit country"\
                            "\n4 -> Add books"\
                            "\n5 -> Remove books"\
                            "\n6 -> Delete this author"\
                            "\nq -> Nothing"
