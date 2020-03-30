#include "header.h"

#include <fstream>
#include <filesystem>
#include <utility>

void Data::printbooks()
{
    TablePrinter tp;
    tp.alignCenter();
    tp.setPadding(1);
    tp.setDashedRawsStyle();
    tp.addMergedColumn("Book Database");
    tp.addColumn("Title", 35);
    tp.addColumn("Authors", 35);
    tp.addColumn("Genres", 35);
    tp.addColumn("ID", 9);
    tp.addColumn("Year", 4);
    for (const auto& book: sb)
    {
        std::string authors, genres;
#ifndef NDEBUG
std::cout << book.second.genres.size() << ", " << book.second.authors.size() << std::endl;
#endif
        for (auto g: book.second.genres)
            genres += g.second->name + ", ";
        for (auto a: book.second.authors)
            authors += a.second->name + ", ";
//TODO: Implement multiline using libfort
        tp << book.second.name << authors << genres << book.second.id << book.second.year;
    }
    tp.print();
}

bool Data::delAccount(const std::string& l, const bool& isadmin)
{
    if (isadmin)
    {
        auto sought = ma.find(l);
        if (sought == ma.end() || mu.size() < 2) return false;
        ma.erase(sought);
    }
    else
    {
        auto sought = mu.find(l);
        if (sought == mu.end()) return false;
        mu.erase(sought);
    }
    return true;
}

void Data::printCredentials(bool isAdmin)
{
    std::cout << (isAdmin ? "Admin" : "User") << " credentials: " << std::endl;
    for (const auto& el: (isAdmin ? ma : mu))
        std::cout << el.first << "\n";
    std::cout << std::endl;
}

bool Data::passCheck(const std::string& l, const std::string& p, const bool& isadmin)
{ return ((isadmin ? ma.find(l) : mu.find(l))->second == hash(p)); }

bool Data::loginCheck(std::string& s, bool isadmin)
{ return (isadmin ? ma.find(s) != ma.end() : ma.find(s) != mu.end()); }

void Data::createAccount(const std::string& l, const std::string& p, const bool& isadmin)
{ (isadmin ? ma : mu)[l] = hash(p); }

size_t Data::enumAccounts(bool isadmin)
{ return (isadmin ? ma.size() : mu.size()); }

void Data::changePass(const std::string& l, const std::string& p, const bool& isadmin)
{ (isadmin ? ma : mu )[l] = hash(p); }

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
    std::vector<Book*> ret;
    for (auto book : sb)
        if (book.second.check(s)) ret.push_back(&book.second);
    return ret;
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
        sg.emplace(std::make_pair(std::stoull(id), Genre(std::stoull(id),name)));
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
        sa.emplace(std::make_pair(std::stoull(id),Author(std::stoull(id), name, date, country)));
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
    std::cout << "Current sb state: " << std::endl;
    for (auto& el: sb)
        std::cout << el.second << std::endl;
    std::cout << "Current sg state: " << std::endl;
    for (auto& el: sg)
        std::cout << el.second << std::endl;
    std::cout << "Current sa state: " << std::endl;
    for (auto& el: sa)
        std::cout << el.second << std::endl;
#endif
    while (bf) //TODO: Add checks for the authorinit and genreinit
    {
        std::cout << "Starting to parse a new book " << std::endl;
        std::string id, title, year, temp, entry;
        if (!readString(bf, id, 'i')) throw std::runtime_error("book id");
        if (!readString(bf, title, 's')) throw std::runtime_error("book name");
        if (!readString(bf, year, 'y')) throw std::runtime_error("book year");
        std::cout << "emplace_back " << id << ' ' << title << ' ' << std::stoul(year) << std::endl;
        auto curbook = sb.emplace(std::make_pair(std::stoull(id),Book(std::stoull(id), title, std::stoul(year) ) ) );
        std::cout << "emplace_back finished\n";
        //Place genres
        if (!readString(bf, temp, 's')) throw std::runtime_error("book genres");
        std::stringstream ss(temp);
        while (getline(ss, entry, ','))
        {
            Genre* sought = findName(sg, entry);
            std::cout << "findName returned: " << (sought == nullptr ? "Nothing" : sought->name) << std::endl;
            if (sought == nullptr) //If we didn't find anything
            {
                std::cout << "Executing new genre creation" << std::endl;
                ull gid = genID();
                sg.emplace(std::make_pair(gid, Genre(gid,entry)))->second.addBook(curbook->second); //create a new genre and bind it to the book
            }   //The book is bound to the genre too
            else
            {
                std::cout << "Executing adding pointer" << std::endl;
                sought->addBook(curbook->second);
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
            Author* sought = findName(sa, entry);
            std::cout << "findName returned: " << (sought == nullptr ? "Nothing" : sought->name) << std::endl;
            if (sought == nullptr)
            {
                std::cout << "Executing new author creation" << std::endl;
                ull gid = genID();
                sa.emplace(std::make_pair(gid, Author(gid,entry)))->second.addBook(curbook->second);
            }
            else
            {
                std::cout << "Executing adding pointer" << std::endl;
                sought->addBook(curbook->second);
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
        mu[login] = pass; //read pass, login and add them to the map. Duplicates removed.
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
        ma[login] = pass; //read pass, login and add them to the map. Duplicates removed.

        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ")
            throw std::invalid_argument("File " + name + " read error, check delimiters.");
        //continues to read if f is good;
    }
    if (ma.empty())
    {
        std::cerr << "Warning! We couldn't find any valid administrator accounts. \n"
                     "Created a new one: admin | admin" << std::endl;
        ma["admin"] = hash("admin");
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
    for (auto& el: ma)
        fadm << el.first << "\n" << el.second << "\n" << std::endl;
    for (auto& el: mu)
        fusr << el.first << "\n" << el.second << "\n" << std::endl;
    for (auto& el: sg)
        fg << el.second << std::endl;
    for (auto& el: sa)
        fa << el.second << std::endl;
    for (auto& book: sb)
        fb << book.second;
    //The destructor will close the files for me
}