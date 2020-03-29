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

#ifndef NDEBUG
std::cout << book.genres.size() << ", " << book.authors.size() << std::endl;
#endif

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

void Data::genreinit() try
{
std::string gfname = "genres.txt";
ensureFileExists(gfname);
std::ifstream gf(path + gfname);
    while (gf)
    {
        std::string id, name;
        if (!readString(gf, id, 'i')) throw std::runtime_error("genre id");
        if (!readString(gf, name, 's')) throw std::runtime_error("genre name");
        vgenres.emplace_back(id, name);
        getline(gf, name); //Ignores 1 line. TODO:Test

    }
#ifndef NDEBUG
    std::cout << "Successfully read genres"<< std::endl;
#endif
}
catch (std::runtime_error& e)
{
    std::cerr << "Couldn't read the file genres.txt:" << e.what() << std::endl;
    system("pause");
    return;
}
catch (std::exception& e)
{
    std::cout << "Fatal error in genreinit(): " << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cout << "Unresolved error in genreinit()" << std::endl;
    throw;
}


void Data::authorinit() try
{
    std::string afname = "authors.txt";
    ensureFileExists(afname);
    std::ifstream af(path + afname);

    while (af)
    {
        std::string id, name, date, country, temp;
        if (!readString(af, id, 'i')) throw std::runtime_error("author id");
        if (!readString(af, name, 's')) throw std::runtime_error("author name");
        if (!readString(af, date, 'd')) throw std::runtime_error("author date");
        if (!readString(af, country, 's')) throw std::runtime_error("author country");
        std::cout << " Read this: " << id << ' ' << name << ' ' << date << ' ' << country << ' ' << std::endl;
        vauthors.emplace_back(id, name, date, country);
        getline(af, temp); //Ignores 1 line.
    }
#ifndef NDEBUG
    std::cout << "Successfully read authors"<< std::endl;
#endif
}
catch (std::runtime_error& e)
{
    std::cerr << "Couldn't read the file authors.txt:" << e.what() << std::endl;
    system("pause");
    return;
}
catch (std::exception& e)
{
    std::cout << "Fatal error in authorinit(): " << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cout << "Unresolved error in authorinit()" << std::endl;
    throw;
}


void Data::bookinit() try//TODO: Optimize
{
    std::string bfname = "books.txt";
    ensureFileExists(bfname);
    std::ifstream bf(path + bfname);
#ifndef NDEBUG
    std::cout << "Current vbooks state: " << std::endl;
    for (auto& el: vbooks)
        std::cout << el << std::endl;
    std::cout << "Current vgenres state: " << std::endl;
    for (auto& el: vgenres)
        std::cout << el << std::endl;
    std::cout << "Current vauthors state: " << std::endl;
    for (auto& el: vauthors)
        std::cout << el << std::endl;
#endif
    while (bf)
    {
        std::cout << "Starting to parse a new book " << std::endl;
        std::string id, title, year, temp, entry;
        if (!readString(bf, id, 'i')) throw std::runtime_error("book id");
        if (!readString(bf, title, 's')) throw std::runtime_error("book name");
        if (!readString(bf, year, 'y')) throw std::runtime_error("book year");
        std::cout << "emplace_back " << id << ' ' << title << ' ' << std::stoul(year) << std::endl;
        Book& curbook = vbooks.emplace_back(id, title, std::stoul(year));
        std::cout << "emplace_back finished\n";
        //Place genres
        if (!readString(bf, temp, 's')) throw std::runtime_error("book genres");
        std::stringstream ss(temp);
        while (getline(ss, entry, ','))
        {
            Genre* sought = findName(vgenres, entry);
            std::cout << "findName returned: " << (sought == nullptr ? "Nothing" : sought->name) << std::endl;
            if (sought == nullptr) //If we didn't find anything
            {
                std::cout << "Executing new genre creation" << std::endl;
                (vgenres.emplace_back(genId(), entry)).addBook(curbook); //create a new genre and bind it to the book
            }   //The book is bound to the genre too
            else
            {
                std::cout << "Executing adding pointer" << std::endl;
                sought->addBook(curbook);
            }
        }
#ifndef NDEBUG
        std::cout << "Linked books with genres";
#endif
        //Place authors
        if (!readString(bf, temp, 's')) throw std::runtime_error("book authors");
        ss.clear();
        ss.str(temp);
        while (getline(ss, entry, ','))
        {
            Author* sought = findName(vauthors, entry);
            std::cout << "findName returned: " << (sought == nullptr ? "Nothing" : sought->name) << std::endl;
            if (sought == nullptr)
            {
                std::cout << "Executing new author creation" << std::endl;
                vauthors.emplace_back(genId(), entry).addBook(curbook);
            }
            else
            {
                std::cout << "Executing adding pointer" << std::endl;
                sought->addBook(curbook);
            }
        }
        getline(bf, temp); //Ignores 1 line. TODO:Test
        std::cout << "Linked book" << std::endl;
    }
#ifndef NDEBUG
    std::cout << "Successfully read books"<< std::endl;
    system("pause");
#endif
}
catch (std::runtime_error& e)
{
    std::cerr << "Couldn't read the file books.txt:" << e.what() << std::endl;
    system("pause");
    return;
}
catch (std::exception& e)
{
    std::cout << "Fatal error: " << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cout << "Unresolved error" << std::endl;
    throw;
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