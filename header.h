#define NDEBUG
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
#include <regex>
//#include <windows.h>

using tprinter::TablePrinter; //allows to use tableprinter namespace
typedef unsigned long nat;

extern std::string path; //Path to the program folder, see pch.cpp -> int main()

void sleep(const nat&); // const int - seconds to sleep_s uses std::this_thread::sleep_for
std::string hash(const std::string& s); //uses sha256.cpp and sha256.h for encrypting passwords, outputs hashed string
bool readString(std::istream& is, std::string& s, char mode); //allows for reading a line from the iostream object with input check (foolproofing)
// 's' for strings with spaces, 'n' for normal, 'd' for date

struct Book //contains data about books entries
{
    std::string isbn; //unique book number
    std::string title; //I mean title
    std::string author; //Author's name
    std::string date; //The date the book was CREATED

    inline void printBook() //prints all the members in a single line to console
    { std::cout << this->title << "\n" << this->author << "\n" << this->isbn << "\n" << this->date << std::endl; }

    inline bool empty() //returns 1 if the entry is empty (default-initialized)
    { return (title.empty() && author.empty() && isbn.empty() && date.empty()); }
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
    friend struct Book; //to use Books in functions
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