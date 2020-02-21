
#ifndef LAB5_HEADER_H
#define LAB5_HEADER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <thread>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "sha256.h"
typedef unsigned nat;


void sleep(const int &);
std::string hash(const std::string& s);
bool readString(std::istream& is,  std::string& s,char mode);
// 's' for strings with spaces, 'n' for normal, 'd' for date

void bgProc(const std::string& );

class Book
{
private:
    std::string isbn = "";
    std::string name = "";
    std::string author = "";
    std::string date = "";

public:
    friend class Data;
    void printInfo();
};


class Data
{
private:
    Data() = default;

    //Actual Data
    std::vector<Book> vbooks;
    std::map<std::string, std::string> muser;
    std::map<std::string, std::string> madm;

public:
    friend class Book;

    static Data &getInstance()
    {
        static Data Instance;
        return Instance;
    }

    bool loginCheck(std::string &s, bool isadmin);
    bool passCheck(std::string &l, std::string &p, bool isadmin); //1 for admin, 0 for user
    bool uinit(const std::string &path);
    bool bookinit(const std::string &path);
    bool adminit(const std::string &path);
    void printbooks();
    std::map<std::string, std::string>& getmuser() { return muser; }
    std::map<std::string, std::string>& getmadm() { return madm; }
    void printCredentials(char which); // 'a' for admin, 'u' for user;

};


#endif //LAB5_HEADER_H