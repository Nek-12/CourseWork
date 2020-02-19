
#ifndef LAB5_HEADER_H
#define LAB5_HEADER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <thread>
#include "sha256.h"
#include <conio.h>
#include <fstream>
#include <sstream>

typedef unsigned nat;


void sleep(const int &);

std::string read(std::string input, int type);

void bgProc(std::string);

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
    std::map<std::string, std::string> madmin;

public:
    friend class Book;

    static Data &getInstance()
    {
        static Data Instance;
        return Instance;
    }

    bool ulogin(std::string &s);

    bool upass(std::string &l, std::string &p);

    bool uinit(const std::string &path);

    bool bookinit(const std::string &path);

    bool adminit(const std::string &path);


    void printbooks();
};


#endif //LAB5_HEADER_H