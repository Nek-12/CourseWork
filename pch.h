#define DEBUG
#ifndef LAB5_HEADER_H
#define LAB5_HEADER_H

#include <iostream>
#include <string>
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

using tprinter::TablePrinter;
typedef unsigned nat;

extern std::string path;

void sleep(const int&);
std::string hash(const std::string& s);
bool readString(std::istream& is, std::string& s, char mode);
// 's' for strings with spaces, 'n' for normal, 'd' for date

struct Book
{
    std::string isbn;
    std::string name;
    std::string author;
    std::string date;

    void printBook()
    { std::cout << this->name << "\n" << this->author << "\n" << this->isbn << "\n" << this->date << std::endl; }

    bool empty()
    { return (name.empty() && author.empty() && isbn.empty() && date.empty()); }
};

class Data
{
private:
    Data();

    //Actual Data
    std::vector<Book> vbooks;
    std::map<std::string, std::string> mapuser;
    std::map<std::string, std::string> mapadm;

public:
    friend struct Book;
    Data(Data const&) = delete;
    void operator=(Data const&) = delete;

    static Data& getInstance()
    {
        static Data Instance;
#ifdef DEBUG
        std::cout << "Got instance of data" << std::endl;
#endif
        return Instance;
    }

    void save();
    bool loginCheck(std::string& s, bool isadmin);
    bool passCheck(const std::string& l, const std::string& p, bool isadmin); //1 for admin, 0 for user
    bool uinit();
    bool bookinit();
    bool adminit();
    void printbooks();
    void printCredentials(bool isAdmin); // 'a' for admin, 'u' for user;

    std::map<std::string, std::string>& muser()
    { return mapuser; }

    std::map<std::string, std::string>& madm()
    { return mapadm; }

    std::vector<Book>& vBooks()
    { return vbooks; }

};


#endif //LAB5_HEADER_H