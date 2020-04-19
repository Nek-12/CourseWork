#include "header.h"
#include <fstream>
#include <filesystem>

void Data::printBooks()
{
    fort::char_table t;
    t << fort::header << "Title" << "Genres" << "Authors" << "Year" << "ID" << fort::endr;
    for (const auto& book: mbooks)
    {
        std::stringstream authors, genres;
        std::string delim;
        for (const auto& g: book.second.genres)
        {
            genres << delim << g->getName();
            delim = "\n";
        }
        delim.clear();
        for (const auto& a: book.second.authors)
        {
            authors << delim << a->getName();
            delim = "\n";
        }
        t << book.second.getName() << genres.str() << authors.str() << book.second.year << book.second.id() << fort::endr;
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
    for (const auto& author: mauthors)
    {
        std::stringstream books;
        std::string delim;
        delim.clear();
        for (const auto& a: author.second.books)
        {
            books << delim << a->getName();
            delim = "\n";
        }
        t << author.second.getName() << books.str() << author.second.date << author.second.country << author.second.id() << fort::endr;
    }
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(4).set_cell_content_fg_color(fort::color::red);
    std::cout << t.to_string() << std::endl;
}
void Data::printGenres(unsigned years)
{
    unsigned diff = getCurYear() - years;
    fort::char_table t;
    std::cout << "Books grouped by genres for the past " << years << " years" << std::endl;
    t << fort::header << "Name" << "Quantity" << "Books" << "Years" << "ID" << fort::endr;
    for (const auto& genre: mgenres)
    {
        unsigned long cnt = 0;
        std::stringstream books, syear;
        std::string delim;
        for (const auto& b: genre.second.books)
        {
            if (b->year != 0 && b->year <= diff) continue;
            books << delim << b->getName();
            syear << delim << b->getYear();
            delim = "\n";
            ++cnt;
        }
        t << genre.second.getName() << cnt << books.str() << syear.str() << genre.second.id() << fort::endr;
    }
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(3).set_cell_content_fg_color(fort::color::red);
    std::cout << t.to_string() << std::endl;
}
bool Data::delAccount(const std::string& l, const bool& isadmin)
{
    if (isadmin)
    {
        auto sought = admins.find(l);
        if (sought == admins.end() || users.size() < 2) return false;
        admins.erase(sought);
    }
    else
    {
        auto sought = users.find(l);
        if (sought == users.end()) return false;
        users.erase(sought);
    }
    return true;
}
void Data::printCredentials(bool isAdmin)
{
    std::cout << (isAdmin ? "Admin" : "User") << " credentials: " << std::endl;
    for (const auto& el: (isAdmin ? admins : users))
        std::cout << el.first << "\n";
    std::cout << std::endl;
}
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
    for (auto& book : mbooks)
        if (book.second.check(s)) ret.push_back(&book.second);
    return ret;
}

std::vector<Genre*> Data::searchGenre(const std::string& s)
{
    std::vector<Genre*> ret;
    for (auto& g : mgenres)
        if (g.second.check(s)) ret.push_back(&g.second);
    return ret;
}

std::vector<Author*> Data::searchAuthor(const std::string& s)
{
    std::vector<Author*> ret;
    for (auto& a : mauthors)
        if (a.second.check(s)) ret.push_back(&a.second);
    return ret;
}


void Data::load() try
{
    std::string tempA, tempB, tempC, tempD, name = "user.txt";
    ensureFileExists(name);
    std::ifstream f(path + name);
    auto empty = [&f]()
    { return f.peek() == std::ifstream::traits_type::eof(); };
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (empty()) break;
        if (!readString(f, tempA, 'n')) throw std::invalid_argument("File: " + name + " couldn't read login");
        if (!readString(f, tempB, 'n')) throw std::invalid_argument("File: " + name + " couldn't read password");
        addAccount(tempA, tempB, false);
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
        if (!readString(f, tempA, 'n')) throw std::invalid_argument("File: " + name + " couldn't read login");
        if (!readString(f, tempB, 'n')) throw std::invalid_argument("File: " + name + " couldn't read password");
        addAccount(tempA, tempB, true);
        if (!std::getline(f, tempC)) break;
        if (!tempC.empty() && tempC != " ")
            throw std::invalid_argument("File " + name + " read error, check delimiters.");
        //continues to read if f is good;
    }
    std::cout << "Successfully read admins" << std::endl;
    if (admins.empty())
    {
        std::cerr << "Warning! We  couldn't find any valid administrator accounts. \n"
                     "Created a new one: admin | admin" << std::endl;
        addAccount("admin", "admin", true);
    }
    name = "genres.txt";
    f.close();
    ensureFileExists(name);
    f.open(path + name);
    while (f)
    {
        if (empty()) break;
        if (!readString(f, tempA, 'i')) throw std::invalid_argument("File: " + name + " couldn't read ID");
        if (!readString(f, tempB, 's')) throw std::invalid_argument("File: " + name + " couldn't read name");
        addGenre(stoid(tempA), tempB, stoid(tempA));
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
        if (!readString(f, tempA, 'i')) throw std::invalid_argument("File: " + name + " couldn't read id");
        if (!readString(f, tempB, 's')) throw std::invalid_argument("File: " + name + " couldn't read name");
        if (!readString(f, tempC, 'd')) throw std::invalid_argument("File: " + name + " couldn't read date");
        if (!readString(f, tempD, 's')) throw std::invalid_argument("File: " + name + " couldn't read country");
        addAuthor(stoid(tempA), tempB, tempC, tempD, stoid(tempA));
        getline(f, tempC); //Ignores 1 line.
    }
    std::cout << "Successfully read authors" << std::endl;
    f.close();
    name = "books.txt";
    ensureFileExists(name);
    f.open(path + name);
#ifndef NDEBUG
    std::cout << "Current books state: " << std::endl;
    for (auto& el: mbooks)
        std::cout << "key: " << el.first << "\n" << el.second << std::endl;
    std::cout << "Current genres state: " << std::endl;
    for (auto& el: mgenres)
        std::cout << "key: " << el.first << "\n" << el.second << std::endl;
    std::cout << "Current authors state: " << std::endl;
    for (auto& el: mauthors)
        std::cout << "key: " << el.first << "\n" << el.second << std::endl;
#endif
    while (f)
    { //id, title, year, temp, entry
        if (empty()) break;
#ifndef NDEBUG
        std::cout << "Starting to parse a new book " << std::endl;
#endif
        if (!readString(f, tempA, 'i')) throw std::invalid_argument("File: " + name + " couldn't read id");
        if (!readString(f, tempB, 's')) throw std::invalid_argument("File: " + name + " couldn't read title");
        if (!readString(f, tempC, 'y')) throw std::invalid_argument("File: " + name + " couldn't read year");
#ifndef NDEBUG
        std::cout << "emplace_back " << tempA << ' ' << tempB << ' ' << tempC << std::endl;
#endif
        auto curbook = addBook(stoid(tempA), tempB, stoid(tempC), stoid(tempA));
#ifndef NDEBUG
        std::cout << "emplace_back finished\n";
#endif
        if (!curbook) throw std::runtime_error("Duplicate on emplace book");
        //Place genres
        if (!readString(f, tempA, 's')) throw std::invalid_argument("File: " + name + " couldn't read book's genres");
        std::stringstream ss(tempA); //tempA - line with genres, tempD - genre;
        while (getline(ss, tempD, ','))
        {
            if (!checkString(tempD, 'i')) throw std::invalid_argument("File: " + name + " couldn't read book's genre ID");
            auto sought = mgenres.find(stoid(tempD));
#ifndef NDEBUG
            std::cout << "sought.first is: " << (sought != mgenres.end() ? std::to_string(sought->first) : "NULL") << std::endl;
#endif
            if (sought == mgenres.end()) //If we didn't find anything
            {
#ifndef NDEBUG
                std::cout << "Executing new genre creation" << std::endl;
#endif
                addGenre(stoid(tempD), "Unknown genre", stoid(tempD))->addBook(*curbook);
            }   //create a new genre and bind it to the book
            else
            {
#ifndef NDEBUG
                std::cout << "Executing adding pointer" << std::endl;
#endif
                sought->second.addBook(*curbook);
            }
            tempD.clear();
        }
        std::cout << "Successfully linked books with genres\n";
        //Place authors
        if (!readString(f, tempA, 's')) throw std::invalid_argument("File: " + name + " couldn't read book's authors");
        ss.clear();
        ss.str(tempA); //author's line, tempD is author's ID;
        while (getline(ss, tempD, ','))
        {
            if (!checkString(tempD, 'i')) throw std::invalid_argument("File: " + name + " couldn't read book's author ID ");
            auto author = mauthors.find(stoid(tempD));
#ifndef NDEBUG
            std::cout << "sought.first is: " << (author != mauthors.end() ? std::to_string(author->first) : "NULL") << std::endl;
#endif
            if (author == mauthors.end())
            {
#ifndef NDEBUG
                std::cout << "Executing new author creation" << std::endl;
#endif
                addAuthor(stoid(tempD), "Unknown author", "0.0.0000", "Unknown", stoid(tempD))->addBook(*curbook);
            }
            else
            {
#ifndef NDEBUG
                std::cout << "Executing adding pointer" << std::endl;
#endif
                author->second.addBook(*curbook);
            }
            tempD.clear();
        }
        getline(f, tempA); //Ignores 1 line.
#ifndef NDEBUG
        std::cout << "Linked book " << tempB << std::endl;
#endif
    }
#ifndef NDEBUG
    std::cout << "Successfully read books" << std::endl;
    getch();
#endif
}
catch (const std::exception& e)
{
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
    printBooks();
    printCredentials(true);
    printCredentials(false);
#endif
    for (auto& el: users)
        f << el.first << "\n" << el.second << "\n" << std::endl;
    f.close();
    f.open(path + "admin.txt");
    f << std::setfill('0');
    for (auto& el: admins)
        f << el.first << "\n" << el.second << "\n" << std::endl;
    f.close();
    f.open(path + "genres.txt");
    f << std::setfill('0');
    for (auto& el: mgenres)
        f << std::setw(MAX_ID_LENGTH) << el.first << '\n' << el.second.getName() << '\n' << std::endl;
    f.close();
    f.open(path + "authors.txt");
    f << std::setfill('0');
    for (auto& el: mauthors)
        f << std::setw(MAX_ID_LENGTH) << el.first << '\n' << el.second.getName() << '\n' << el.second.date << '\n' << el.second.country
          << '\n'
          << std::endl;
    f.close();
    f.open(path + "books.txt");
    f << std::setfill('0');
    for (auto& b: mbooks)
    {
        std::string delim;
        f << std::setw(MAX_ID_LENGTH) << b.first << "\n" << b.second.getName() << "\n" << std::setw(4) << b.second.year << "\n";
        //Place genres
        if (!b.second.genres.empty())
        {
            for (auto& g: b.second.genres)
            {
                f << delim << std::setw(MAX_ID_LENGTH) << g->id();
                delim = ',';
            }
        }
        else
        {
            std::cerr << "Warning! The book \n" << b.second << "\n Has zero genres! The data will be generated automatically!" << std::endl;
            f << genID();
        }
        f << "\n";
        delim.clear();
        if (!b.second.authors.empty())
        {
            for (auto& a: b.second.authors)
            {
                f << delim << std::setw(MAX_ID_LENGTH) << a->id();
                delim = ',';
            }
        }
        else
        {
            std::cerr << "Warning! The book \n" << b.second << "\n Has zero authors! The data will be generated automatically!"
                      << std::endl;
            f << genID();
        }
        f << "\n" << std::endl;
    }
    f << std::setfill(' ');
}