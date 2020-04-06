#include "header.h"
#include <fstream>
#include <filesystem>

void Data::printbooks()
{
    fort::char_table t;
    t << fort::header << "Title" << "Genres" << "Authors" << "Year" << "ID" << fort::endr;
    for (const auto& book: sb)
    {
        std::string authors, genres, delim;
        for (auto g: book.second.genres)
        {
            genres += delim + g.second->name ;
        delim = "\n";
        }
        delim.clear();
        for (auto a: book.second.authors)
        {
            authors += delim + a.second->name;
            delim = "\n";
        }
            t << book.second.name << genres << authors << book.second.year << book.second.id << fort::endr;
    }
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(4).set_cell_content_fg_color(fort::color::red);
    std::cout << t.to_string() << std::endl;
}
void Data::printAuthors()
{
    fort::char_table t;
    t << fort::header << "Name" << "Books" << "Birthdate" << "Country" << "ID" << fort::endr;
    for (const auto& author: sa)
    {
        std::string books, delim;
        delim.clear();
        for (auto a: author.second.books)
        {
            books += delim + a.second->name;
            delim = "\n";
        }
        t << author.second.name << books << author.second.date << author.second.country << author.second.id << fort::endr;
    }
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(5).set_cell_content_fg_color(fort::color::red);
    std::cout << t.to_string() << std::endl;
}

void Data::printGenres(unsigned years = getCurYear() )
{
    unsigned compar = getCurYear() - years;
    fort::char_table t;
    std::cout << "Books grouped by genres for the past " << years << " years" << std::endl;
    t << fort::header << "Name" << "Quantity" <<"Books" << "ID" << fort::endr;
    for (const auto& genre: sg)
    {
        unsigned long cnt = 0;
        std::string books, delim;
        for (auto b: genre.second.books)
        {
            if (b.second->year <= compar) continue;
            books += delim + b.second->name;
            delim = "\n";
            ++cnt;
        }
        delim.clear();
        t << genre.second.name << cnt << books << genre.second.id << fort::endr;
    }
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(2).set_cell_content_fg_color(fort::color::red);
    std::cout << t.to_string() << std::endl;
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
    for (auto& book : sb)
        if (book.second.check(s)) ret.push_back(&book.second);
    return ret;
}

std::vector<Genre*> Data::searchGenre(const std::string& s)
{
    std::vector<Genre*> ret;
    for (auto& g : sg)
        if (g.second.check(s)) ret.push_back(&g.second);
    return ret;
}

std::vector<Author*> Data::searchAuthor(const std::string& s)
{
    std::vector<Author*> ret;
    for (auto& a : sa)
        if (a.second.check(s)) ret.push_back(&a.second);
    return ret;
}
//TODO: Overload searchGenre by ID or remove

void Data::load() try
{
    std::string tempA, tempB, tempC, tempD, name = "user.txt";
    ensureFileExists(name);
    std::ifstream f(path + name);
    auto empty = [&f ]() { return f.peek() == std::ifstream::traits_type::eof(); };
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (empty()) break;
        if (!readString(f, tempA, 'n')) throw std::invalid_argument("File: " + name + " couldn't read line: " + tempA);
        if (!readString(f, tempB, 'n')) throw std::invalid_argument("File: " + name + " couldn't read line: " + tempB);
        mu[tempA] = tempB; //read pass, login and add them to the map. Duplicates removed.
        if (!std::getline(f, tempC)) break;
        if (!tempC.empty() && tempC != " ")
            throw std::invalid_argument("File " + name + " read error, check delimiters.");
        //continues to read if f is good;
    }
    std::cout << "Successfully read users" << std::endl;
    name = "admin.txt";
    f.close();
    ensureFileExists(name);
    f.open(path + name);
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (empty()) break;
        if (f.eof()) break;
        if (!readString(f, tempA, 'n')) throw std::invalid_argument("File: " + name + " couldn't read line: " + tempA);
        if (!readString(f, tempB, 'n')) throw std::invalid_argument("File: " + name + " couldn't read line: " + tempB);
        ma[tempA] = tempB; //read pass, login and add them to the map. Duplicates removed.
        if (!std::getline(f, tempC)) break;
        if (!tempC.empty() && tempC != " ")
            throw std::invalid_argument("File " + name + " read error, check delimiters.");
        //continues to read if f is good;
    }
    std::cout << "Successfully read admins" << std::endl;
    if (ma.empty())
    {
        std::cerr << "Warning! We  couldn't find any valid administrator accounts. \n"
                     "Created a new one: admin | admin" << std::endl;
        ma["admin"] = hash("admin");
    }
    name = "genres.txt";
    f.close();
    ensureFileExists(name);
    f.open(path + name);
    while (f)
    {
        if (empty()) break;
        if (!readString(f, tempA, 'i')) throw std::invalid_argument("File: " + name + " couldn't read line: " + tempA);
        if (!readString(f, tempB, 's')) throw std::invalid_argument("File: " + name + " couldn't read line: " + tempB);
        sg.emplace(std::make_pair(std::stoul(tempA), Genre(std::stoul(tempA), tempB)));
        getline(f, tempC); //Ignores 1 line.
    }
    std::cout << "Successfully read genres" << std::endl;
    name = "authors.txt";
    f.close();
    ensureFileExists(name);
    f.open(path + name);
    while (f)
    { //id, name, date, country
        if (empty()) break;
        if (!readString(f, tempA, 'i')) throw std::invalid_argument("File: " + name + " couldn't read id: " + tempA);
        if (!readString(f, tempB, 's')) throw std::invalid_argument("File: " + name + " couldn't read name: " + tempB);
        if (!readString(f, tempC, 'd')) throw std::invalid_argument("File: " + name + " couldn't read date: " + tempC);
        if (!readString(f, tempD, 's')) throw std::invalid_argument("File: " + name + " couldn't read country: " + tempD);
                sa.emplace(std::make_pair(std::stoul(tempA), Author(std::stoul(tempA), tempB, tempC, tempD)));
        getline(f, tempC); //Ignores 1 line.
    }
    std::cout << "Successfully read authors" << std::endl;
    f.close();
    name = "books.txt";
    ensureFileExists(name);
    f.open(path + name);
#ifndef NDEBUG
    std::cout << "Current sb state: " << std::endl;
    for (auto& el: sb)
        std::cout << "key: " << el.first << "\n" << el.second << std::endl;
    std::cout << "Current sg state: " << std::endl;
    for (auto& el: sg)
        std::cout << "key: " << el.first << "\n" << el.second << std::endl;
    std::cout << "Current sa state: " << std::endl;
    for (auto& el: sa)
        std::cout << "key: " << el.first << "\n" << el.second << std::endl;
#endif
    while (f) //TODO: Add checks for the authorinit and genreinit
    { //id, title, year, temp, entry
        if (empty()) break;
        std::cout << "Starting to parse a new book " << std::endl;
        if (!readString(f, tempA, 'i')) throw std::invalid_argument("File: " + name + " couldn't read id: " + tempA);
        if (!readString(f, tempB, 's')) throw std::invalid_argument("File: " + name + " couldn't read title: " + tempB);
        if (!readString(f, tempC, 'y')) throw std::invalid_argument("File: " + name + " couldn't read year: " + tempC);
#ifndef NDEBUG
        std::cout << "emplace_back " << tempA << ' ' << tempB << ' ' << tempC << std::endl;
#endif
        auto curbook = sb.emplace(std::make_pair(std::stoul(tempA), Book(std::stoul(tempA), tempB, std::stoul(tempC))));
#ifndef NDEBUG
        std::cout << "emplace_back finished\n";
#endif
        if (!curbook.second) throw std::runtime_error("Duplicate on emplace book");
        //Place genres
        if (!readString(f, tempA, 's')) throw std::invalid_argument("File: " + name + " couldn't read book's genres: " + tempA);
        std::stringstream ss(tempA); //tempA - line with genres, tempD - genre;
        while (getline(ss, tempD, ','))
        {
            if (!checkString(tempD, 'i')) throw std::invalid_argument("File: " + name + " couldn't read book's genre ID: " + tempD);
            auto sought = sg.find(std::stoul(tempD));
#ifndef NDEBUG
            std::cout << "sought.first is: " << (sought != sg.end() ? std::to_string(sought->first) : "NULL") << std::endl;
#endif
            if (sought == sg.end()) //If we didn't find anything
            {
#ifndef NDEBUG
                std::cout << "Executing new genre creation" << std::endl;
#endif
                sg.emplace(std::make_pair(std::stoul(tempD), Genre(std::stoul(tempD), "Unknown genre"))).first->second.addBook(
                        curbook.first->second); //create a new genre and bind it to the book
            }   //The book is bound to the genre too
            else
            {
#ifndef NDEBUG
                std::cout << "Executing adding pointer" << std::endl;
#endif
                sought->second.addBook(curbook.first->second);
            }
            tempD.clear();
        }
        std::cout << "Successfully linked books with genres\n";
        //Place authors
        if (!readString(f, tempA, 's')) throw std::invalid_argument("File: " + name + " couldn't read book's authors: " + tempA);
        ss.clear(); //TODO: is this needed?
        ss.str(tempA); //author's line, tempD is author's ID;
        while (getline(ss, tempD, ','))
        {
            if (!checkString(tempD, 'i')) throw std::invalid_argument("File: " + name + " couldn't read book's author ID " + tempD);
            auto sought = sa.find(std::stoul(tempD));
            std::cout << "sought.first is: " << (sought != sa.end() ? std::to_string(sought->first) : "NULL") << std::endl;
            if (sought == sa.end())
            {
#ifndef NDEBUG
                std::cout << "Executing new author creation" << std::endl;
#endif
                sa.emplace(std::make_pair(std::stoul(tempD), Author(std::stoul(tempD), "Unknown author"))).first->second.addBook(
                        curbook.first->second);
            }
            else
            {
#ifndef NDEBUG
                std::cout << "Executing adding pointer" << std::endl;
#endif
                sought->second.addBook(curbook.first->second);
            }
            tempD.clear();
        }
        getline(f, tempA); //Ignores 1 line. TODO:Test
#ifndef NDEBUG
        std::cout << "Linked book " << tempB << std::endl;
#endif
    }
#ifndef NDEBUG
    std::cout << "Successfully read books" << std::endl;
    system("pause");
#endif
}
catch (const std::exception& e)
{
    std::cerr << "Critical error while reading files. The program cannot continue. \n"
    << "Error: " << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cerr << "Unknown error while reading files. The program cannot continue." << std::endl;
   throw;
}
void Data::save()
{
    std::cout << "Saving..." << std::endl;
    std::ofstream f(path + "user.txt");
    f << std::setfill('0');
#ifndef NDEBUG
    printbooks();
    printCredentials(true);
    printCredentials(false);
#endif
    for (auto& el: mu)
        f << el.first << "\n" << el.second << "\n" << std::endl;
    f.close(); f.open(path + "admin.txt");
    for (auto& el: ma)
        f << el.first << "\n" << el.second << "\n" << std::endl;
    f.close(); f.open(path + "genres.txt");
    for (auto& el: sg)
        f <<  std::setw(9) << el.first  << '\n' << el.second.name << '\n' << std::endl;
    f.close(); f.open(path + "authors.txt");
    for (auto& el: sa)
        f <<  std::setw(9) << el.first << '\n' << el.second.name << '\n' << el.second.date << '\n'  << el.second.country << '\n' <<std::endl;
    f.close(); f.open(path + "books.txt");
    for (auto& b: sb)
        {
            f << std::setw(9) << b.first << "\n" << b.second.name << "\n" << std::setw(4) << b.second.year << "\n";
            std::string delim;
            for (auto& g: b.second.genres)
            {
                f << delim << std::setw(9) << g.second->id;
                delim = ',';
            }
            f << "\n";
            delim.clear();
            for (auto& a: b.second.authors)
            {
                f << delim << std::setw(9) << a.second->id;
                delim = ',';
            }
            f << "\n" << std::setfill(' ') << std::endl;
        }
    //The destructor will close the files for me
}