#include <iomanip>
#include "header.h"

//BOOK

Book::~Book()
{
#ifndef NDEBUG
    std::cout << "Book " << getName() << " Destroyed" << std::endl;
#endif
    remFromAuthors();
    remFromGenres();
}

void Book::addAuthor(Author& a)
{
    authors.insert(&a);
    a.books.insert(this);
}
void Book::addGenre(Genre& g)
{
    genres.insert(&g);
    g.books.insert(this);
}
void Book::remAuthor(Author& a)
{
    authors.erase(&a);
    a.books.erase(this);
}
void Book::remGenre(Genre& g)
{
    genres.erase(&g);
    g.books.erase(this);
}

void Book::addToGenres(const Book& b)
{
    for (const auto& g: b.genres)
        g->addBook(*this);
}
void Book::addToAuthors(const Book& b)
{
    for (const auto& a: b.authors)
        a->addBook(*this);
}
void Book::remFromGenres()
{
    for (const auto& g: genres)
        g->remBook(*this);
}
void Book::remFromAuthors()
{
    for (const auto& a: authors)
        a->remBook(*this);
}
std::string Book::to_string() const
{
    std::string ret = std::to_string(id()) + '\n' + getName() + '\n' + std::to_string(year);
    std::string delim = "\n";
    for (const auto& el: genres)
    {
        ret += delim + el->getName();
        delim = ",";
    }
    delim = "\n";
    for (const auto& el: authors)
    {
        ret += delim + el->getName();
        delim = ",";
    }
    return ret;
}
bool Book::check(const std::string& s) const
{
    std::string ls = lowercase(s);
    if (lowercase(getName()).find(ls) != std::string::npos || s == std::to_string(year) || s == std::to_string(id())) return true;
    for (const auto& el: genres)
        if (lowercase(el->getName()).find(ls) != std::string::npos) return true;
    for (const auto& el: authors)
        if (lowercase(el->getName()).find(ls) != std::string::npos) return true;
    return false;
}

//AUTHOR

Author::~Author()
{
    std::cout << "Author " << this->getName() << " Destroyed" << std::endl;
    remFromBooks();
}

void Author::addToBooks(const Author& a)
{
    for (const auto& b: a.books)
        b->addAuthor(*this);
}

void Author::remFromBooks()
{
    for (const auto& b: books)
        b->remAuthor(*this);
}

void Author::addBook(Book& b)
{
    books.insert(&b);
    b.authors.insert(this);
}

void Author::remBook(Book& b)
{
    books.erase(&b);
    b.authors.erase(this);
}
bool Author::check(const std::string& s) const
{
    std::string ls = lowercase(s);
    if (lowercase(getName()).find(ls) != std::string::npos
        || s == std::to_string(id()) || lowercase(date).find(ls) != std::string::npos
        || lowercase(country).find(ls) != std::string::npos)
        return true;
    for (const auto& el: books)
        if (lowercase(el->getName()).find(ls) != std::string::npos) return true;
    return false;
}

std::string Author::to_string() const
{
    std::string ret = std::to_string(id()) + '\n' + getName() + '\n' + date + '\n' + country;
    std::string delim = "\n";
    for (const auto& el: books)
    {
        ret += delim + el->getName();
        delim = ",";
    }
    return ret;
}

//GENRE

Genre::~Genre()
{
#ifndef NDEBUG
    std::cout << "Genre " << this->getName() << " Destroyed" << std::endl;
#endif
    remFromBooks();
}
void Genre::addToBooks(const Genre& g)
{
    for (const auto& b: g.books)
        b->addGenre(*this);
}
void Genre::remFromBooks()
{
    for (const auto& b: books)
        b->remGenre(*this);
}
void Genre::addBook(Book& b)
{
    books.insert(&b);
    b.genres.insert(this);
}

void Genre::remBook(Book& b)
{
    books.erase(&b);
    b.genres.erase(this);
}
std::ostream& operator<<(std::ostream& os, const Genre& g)
{
    os << g.id() << "\n" << g.getName() << "\n";
    return os;
}

bool Genre::check(const std::string& s) const
{
    std::string ls = lowercase(s);
    if (lowercase(getName()).find(ls) != std::string::npos || s == std::to_string(id())) return true;
    for (auto el: books)
        if (lowercase(el->getName()).find(ls) != std::string::npos) return true;
    return false;
}

std::string Genre::to_string() const
{
    std::string ret = std::to_string(id()) + '\n' + getName();
    std::string delim = "\n";
    for (const auto& el: books)
    {
        ret += delim + el->getName();
        delim = ",";
    }
    return ret;
}
