
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

typedef unsigned nat;

void sleep(const int &);

std::string read(std::string input, int type);

void bgProc();

class Book
{
private:
    std::string isbn = "";
    std::string name = "";
    std::string author = "";
    std::string created = "";

public:
    friend class Data;

    void printInfo(); //TODO: Implement
};

class Data
{
private:
    Data() = default;

    //Actual Data
    std::map<std::string, Book> books;
    std::map<std::string, std::string> user;
    std::map<std::string, std::string> admin;

public:
    friend class Book;

    static Data &getInstance()
    {
        static Data Instance;
        return Instance;
    }

    bool ulogin(std::string &s);

    bool upass(std::string &l, std::string &p);


};


#endif //LAB5_HEADER_H