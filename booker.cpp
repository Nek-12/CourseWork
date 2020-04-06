#include <iomanip>
#include "header.h"

//BOOK

Book::~Book()
{
#ifndef NDEBUG
    std::cout << "Book " << this->name << " Destroyed" << std::endl;
#endif
    remFromAuthors();
    remFromGenres();
}

void Book::addAuthor(Author& a)
{
    authors.insert(std::make_pair(a.id,&a));
    a.books.insert(std::make_pair(this->id,this));
}
void Book::addGenre(Genre& g)
{
    genres.insert(std::make_pair(g.id,&g));
    g.books.insert(std::make_pair(this->id,this));
}
void Book::remAuthor(Author& a)
{
    authors.erase(a.id);
    a.books.erase(this->id);
}
void Book::remGenre(Genre& g)
{
    genres.erase(g.id);
    g.books.erase(this->id);
}
std::ostream &operator<<(std::ostream &os, const Book& b)
{
    os << b.id << "\n" << b.name << "\n" << std::setfill('0') << std::setw(4) << b.year << "\n";
    std::string delim;
    for (auto& g: b.genres)
    {
        os << delim << g.second->name;
        delim = ',';
    }
    os << "\n";
    delim.clear();
    for (auto& a: b.authors)
    {
        os << delim << a.second->name;
        delim = ',';
    }
    os << "\n" << std::setfill(' ') << std::endl;
    return os;
}

void Book::addToGenres(const Book& b)
{
    for (auto g: b.genres)
        g.second->addBook(*this);
}
void Book::addToAuthors(const Book& b)
{
    for (auto a: b.authors)
        a.second->addBook(*this);
}
void Book::remFromGenres()
{
    for (auto g: genres)
        g.second->remBook(*this);
}
void Book::remFromAuthors()
{
    for (auto a: authors)
        a.second->remBook(*this);
}
bool Book::check(const std::string& s)
{
    std::string ls = lowercase(s);
    if (lowercase(name).find(ls) != std::string::npos || s == std::to_string(year) || s == std::to_string(id) ) return true;
    for (const auto& el: genres)
        if (lowercase(el.second->name).find(ls) != std::string::npos) return true;
    for (const auto& el: authors)
        if (lowercase(el.second->name).find(ls) != std::string::npos) return true;
    return false;
}

Genre::~Genre()
{
#ifndef NDEBUG
    std::cout << "Genre " << this->name << " Destroyed" << std::endl;
#endif
    remFromAuthors();
    remFromBooks();
}

void Genre::addToBooks(const Genre& g)
{
    for (auto b: g.books)
        b.second->addGenre(*this);
}
void Genre::addToAuthors(const Genre& g)
{
    for (auto b: g.books)
        b.second->addGenre(*this);
}
void Genre::remFromBooks()
{
    for (auto b: books)
        b.second->remGenre(*this);
}
void Genre::remFromAuthors()
{
    for (auto a: authors)
        a.second->remGenre(*this);
}

void Genre::addAuthor(Author& a)
{
    authors.insert(std::make_pair(a.id,&a));
    a.genres.insert(std::make_pair(this->id,this));
}
void Genre::addBook(Book& b)
{
    books.insert(std::make_pair(b.id,&b));
    b.genres.insert(std::make_pair(this->id,this));
}
void Genre::remAuthor(Author& a)
{
    authors.erase(a.id);
    a.genres.erase(this->id);
}
void Genre::remBook(Book& b)
{
    books.erase(b.id);
    b.genres.erase(this->id);
}
std::ostream& operator<<(std::ostream &os, const Genre& g)
{
    os << g.id << "\n" << g.name << "\n";
    return os;
}

bool Genre::check(const std::string& s)
{
    std::string ls = lowercase(s);
    if (lowercase(name).find(ls) != std::string::npos || s == std::to_string(id) ) return true;
    for (auto el: books)
        if (lowercase(el.second->name).find(ls) != std::string::npos) return true;
    for (auto el: authors)
        if (lowercase(el.second->name).find(ls) != std::string::npos) return true;
    return false;
}

//AUTHOR

Author::~Author()
{
    std::cout << "Author " << this->name << " Destroyed" << std::endl;
    remFromBooks();
    remFromGenres();
}

void Author::addToBooks(const Author& a)
{
    for (auto b: a.books)
        b.second->addAuthor(*this);
}
void Author::addToGenres(const Author& a)
{
    for (auto g: a.genres)
        g.second->addAuthor(*this);
}
void Author::remFromBooks()
{
    for (auto b: books)
        b.second->remAuthor(*this);
}
void Author::remFromGenres()
{
    for (auto g: genres)
        g.second->remAuthor(*this);
}

void Author::addGenre(Genre& g)
{
    genres.insert(std::make_pair(g.id,&g));
    g.authors.insert(std::make_pair(this->id,this));
}
void Author::addBook(Book& b)
{
    books.insert(std::make_pair(b.id,&b));
    b.authors.insert(std::make_pair(this->id,this));
}
void Author::remGenre(Genre& g)
{
    genres.erase(g.id);
    g.authors.erase(this->id);
}
void Author::remBook(Book& b)
{
    books.erase(b.id);
    b.authors.erase(this->id);
}
std::ostream& operator<<(std::ostream &os, const Author& a)
{
    os << a.id << "\n" << a.name << "\n" << a.date << "\n" << a.country << "\n";
    return os;
}
bool Author::check(const std::string& s)
{
    std::string ls = lowercase(s);
    if (lowercase(name).find(ls) != std::string::npos
        || s == std::to_string(id) || lowercase(date).find(ls) != std::string::npos
        || lowercase(country).find(ls) != std::string::npos ) return true;
    for (auto el: books)
        if (lowercase(el.second->name).find(ls) != std::string::npos) return true;
    for (auto el: genres)
        if (lowercase(el.second->name).find(ls) != std::string::npos) return true;
    return false;
}

