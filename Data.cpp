#include "header.h"
#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include <filesystem>


std::string Data::genId()
{
    static std::default_random_engine e(time(nullptr));
    static std::uniform_int_distribution<unsigned long> rng(0, 999999999);
    return std::to_string(rng(e));
}

void Data::printbooks()
{
    TablePrinter tp;
    tp.alignCenter();
    tp.setPadding(1);
    tp.setDashedRawsStyle();
    tp.addMergedColumn("Book Database");
    tp.addColumn("Title", 30);
    tp.addColumn("Authors", 40);
    tp.addColumn("Genres", 40);
    tp.addColumn("ID", 9);
    tp.addColumn("Date", 10);
    for (const auto& book: vbooks)
    {
        std::string authors, genres;
        for (auto g: book.genres)
            genres += g->name + ", ";
        for (auto a: book.authors)
            authors += a->name + ", ";
//TODO: Implement multiline using libfort
        tp << book.name << authors << genres << book.id << book.year;
    }
    tp.print();
}

bool Data::delAccount(const std::string& l, const bool& isadmin)
{
    if (isadmin)
    {
        auto sought = madm.find(l);
        if (sought == madm.end() || muser.size() < 2) return false;
        madm.erase(sought);
    }
    else
    {
        auto sought = muser.find(l);
        if (sought == muser.end()) return false;
        muser.erase(sought);
    }
    return true;
}

void Data::printCredentials(bool isAdmin)
{
    std::cout << (isAdmin ? "Admin" : "User") << " credentials: " << std::endl;
    for (const auto& el: (isAdmin ? madm : muser))
        std::cout << el.first << "\n";
    std::cout << std::endl;
}

bool Data::passCheck(const std::string& l, const std::string& p, const bool& isadmin)
{ return ((isadmin ? madm.find(l) : muser.find(l))->second == hash(p)); }

bool Data::loginCheck(std::string& s, bool isadmin)
{ return (isadmin ? madm.find(s) != madm.end() : madm.find(s) != muser.end()); }

void Data::createAccount(const std::string& l, const std::string& p, const bool& isadmin)
{ (isadmin ? madm : muser)[l] = hash(p); }

size_t Data::enumAccounts(bool isadmin)
{ return (isadmin? madm.size() : muser.size()); }

void Data::changePass(const std::string& l, const std::string& p, const bool& isadmin)
{ (isadmin ? madm : muser )[l] = hash(p); }

void Data::ensureFileExists(const std::string& f)
{
    if (!std::filesystem::exists(path + f))
    {
        std::cerr << "Warning! The file " << path << f << " does not exist! Creating a blank one..."
                  << std::endl;
        std::ofstream file(path + f);
        file.close();
    }
}

std::vector<Book*> Data::searchBook(const std::string& s)
{
    std::vector<Book*> vret;
    for (auto book : vbooks)
        if (book.check(s)) vret.push_back(&book);
    return vret;
}

void Data::bookinit() //TODO: Optimize
{
    std::string bfname = "books.txt", gfname = "genres.txt", afname = "authors.txt";
    ensureFileExists(bfname);
    ensureFileExists(gfname);
    ensureFileExists(afname);
    std::ifstream bf(path + bfname);
    std::ifstream gf(path + gfname);
    std::ifstream af(path + afname);
    while (gf) //Genres
    {
        std::string id, name;
        if (!readString(gf, id, 'i')) continue;
        if (!readString(gf, name, 's')) continue;
        vgenres.emplace_back(id, name);
        getline(gf, name); //Ignores 1 line. TODO:Test
        std::cout << "Read genre" << std::endl;

    }
#ifndef NDEBUG
    std::cout << "Successfully read genres"<< std::endl;
    system("pause");
#endif
    while (af) //Authors
    {
        std::string id, name, date, country;
        if (!readString(gf, id, 'i')) continue;
        if (!readString(gf, name, 's')) continue;
        if (!readString(gf, date, 'd')) continue;
        if (!readString(gf, country, 's')) continue;
        vauthors.emplace_back(id, name, date, country);
        getline(gf, name); //Ignores 1 line. TODO:Test
        std::cout << "Read author" << std::endl;
    }
#ifndef NDEBUG
    std::cout << "Successfully read authors"<< std::endl;
    system("pause");
#endif
    while (bf)
    {
        std::string id, title, year, temp, entry;
        if (!readString(gf, id, 'i')) continue;
        if (!readString(gf, title, 's')) continue;
        if (!readString(gf, year, 'y')) continue;
        Book& curbook = vbooks.emplace_back(id, title, stoi(year));
        std::cout << "Read book" << std::endl;
        std::stringstream ss(temp);
        //Place genres
        if (!readString(gf, temp, 's')) continue;
        while (getline(ss, entry, ','))
        {
            auto sought = findName(vgenres, entry);
            if (sought == vgenres.end()) //If we didn't find anything
                vgenres.emplace_back(genId(), entry).addBook(curbook); //create a new genre and bind it to the book
                //The book is bound to the genre too
            else
                sought->addBook(curbook);
        }
        //Place authors
        if (!readString(gf, temp, 's')) continue;
        ss.str(temp);
        while (getline(ss, entry, ','))
        {
            auto sought = findName(vauthors, entry);
            if (sought == vauthors.end())
                vauthors.emplace_back(genId(), entry).addBook(curbook);
            else
                sought->addBook(curbook);
        }
        getline(gf, temp); //Ignores 1 line. TODO:Test
        std::cout << "Linked book" << std::endl;
    }
#ifndef NDEBUG
    std::cout << "Successfully read books"<< std::endl;
    system("pause");
#endif
}

void Data::uinit()
{
    std::string login, pass, temp, name = "user.txt";
    ensureFileExists(name);
    std::ifstream f(path + name);
    if (!f) throw std::runtime_error("File " + name + " could not be opened after creating.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (!readString(f, login, 'n')) break;
        if (!readString(f, pass, 'n')) break;
        muser[login] = pass; //read pass, login and add them to the map. Duplicates removed.
        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ")
            throw std::invalid_argument("File " + name + " read error, check delimiters.");
        //continues to read if f is good;
    }
}

void Data::adminit()
{
    std::string login, pass, temp, name = "admin.txt";
    ensureFileExists(name);
    std::ifstream f(path + name);
    if (!f) throw std::runtime_error("File " + name + " could not be opened.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (!readString(f, login, 'n')) break;
        if (!readString(f, pass, 'n')) break;
        madm[login] = pass; //read pass, login and add them to the map. Duplicates removed.

        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ")
            throw std::invalid_argument("File " + name + " read error, check delimiters.");
        //continues to read if f is good;
    }
    if (madm.empty())
    {
        std::cerr << "Warning! We couldn't find any valid administrator accounts. \n"
                     "Created a new one: admin | admin" << std::endl;
        madm["admin"] = hash("admin");
    }
}

void Data::save()
{
    std::cout << "Saving..." << std::endl;
    std::ofstream fb(path + "books.txt");
    std::ofstream fg(path + "books.txt");
    std::ofstream fa(path + "authors.txt");
    std::ofstream fusr(path + "user.txt");
    std::ofstream fadm(path + "admin.txt");
#ifndef NDEBUG
    printbooks();
    printCredentials(true);
    printCredentials(false);
#endif
    for (auto& el: madm)
        fadm << el.first << "\n" << el.second << "\n" << std::endl;
    for (auto& el: muser)
        fusr << el.first << "\n" << el.second << "\n" << std::endl;
    for (auto& el: vgenres)
        fg << el << std::endl;
    for (auto& el: vauthors)
        fa << el << std::endl;
    for (auto& book: vbooks)
        fb << book;
    //The destructor will close the files for me
}