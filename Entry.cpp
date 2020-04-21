#include "header.h"

//BOOK

Book::~Book()
{
#ifndef NDEBUG
    std::cout << "Book " << getName() << " Destroyed" << std::endl;
#endif
    for (const auto& g: genres)
        g->unlink(this);
    for (const auto& a: authors)
        a->unlink(this);
}

bool Book::link(Entry* pe)
{
    if (typeid(*pe) == typeid(Genre))
    {
        genres.insert(pe);
        return static_cast<Genre*>(pe)->books.insert(this).second;
    }
    else if (typeid(*pe) == typeid(Author))
    {
        authors.insert(pe);
        return static_cast<Author*>(pe)->books.insert(this).second;
    }
    else return false;
}
bool Book::unlink(Entry* pe)
{
    if (typeid(*pe) == typeid(Genre))
    {
        genres.erase(pe);
        return static_cast<Genre*>(pe)->books.erase(this);
    }
    else if (typeid(*pe) == typeid(Author))
    {
        authors.erase(pe);
        return static_cast<Author*>(pe)->books.erase(this);
    }
    else return false;
}

void Book::remAuthor(const size_t& pos)
{
    if (pos >= authors.size()) throw std::invalid_argument("Deleting genre past the end of book " + getName());
    auto it = authors.begin();
    std::advance(it, pos);
    authors.erase(it);
}
void Book::remGenre(const size_t& pos)
{
    if (pos >= genres.size()) throw std::invalid_argument("Deleting genre past the end of book " + getName());
    auto it = genres.begin();
    std::advance(it, pos);
    genres.erase(it);
}
std::string Book::to_string() const
{
    fort::char_table t;
    t << fort::header << "Title" << "Genres" << "Authors" << "Year" << "ID" << fort::endr;
    std::string delim;
    std::stringstream g, a;
    size_t i = 0;
    for (const auto& el: genres)
    {
        i++;
        g << delim << i << ". " << el->getName();
        delim = "\n";
    }
    delim.clear(); i = 0;
    for (const auto& el: authors)
    {
        i++;
        a << delim << i << ". " << el->getName();
        delim = "\n";
    }
    t << getName() << g.str() << a.str() << year << id() << fort::endr;
    setTableProperties(t,0,4);
    return t.to_string();
}
bool Book::check(const std::string& s) const
{
    std::string ls = lowercase(s);
    return lowercase(getName()).find(ls) != std::string::npos || s == std::to_string(year) || s == std::to_string(id());
}
Book::Book(Book&& b) noexcept: Entry(std::move(b)), year(b.year), authors (std::move(b.authors)), genres(std::move(b.genres))
{
    for (const auto& a: authors)
    {
        a->link(this);
        a->unlink(&b);
    }
    for (const auto& g:genres)
    {
        g->link(this);
        g->unlink(&b);
    }
#ifndef NDEBUG
    std::cout << getName() << " was moved\n";
#endif
}

//AUTHOR

Author::~Author()
{
    for (const auto& b: books)
        b->unlink(this);
#ifndef NDEBUG
    std::cout << "Author " << this->getName() << " Destroyed" << std::endl;
#endif
}
bool Author::link(Entry* pe)
{
    if (typeid(*pe) == typeid(Book))
    {
        books.insert(pe);
        return static_cast<Book*>(pe)->authors.insert(this).second;
    }
    else return false;
}
bool Author::unlink(Entry* pe)
{
    if (typeid(*pe) == typeid(Book))
    {
        books.erase(pe);
        return static_cast<Book*>(pe)->authors.erase(this);
    }
    else return false;
}

void Author::remBook(const size_t& pos)
{
    if (pos >= books.size()) throw std::invalid_argument("Deleting book past the end of " + getName());
    auto it = books.begin();
    std::advance(it, pos);
    books.erase(it);
}
bool Author::check(const std::string& s) const
{
    std::string ls = lowercase(s);
    return lowercase(getName()).find(ls) != std::string::npos
           || s == std::to_string(id()) || lowercase(date).find(ls) != std::string::npos
           || lowercase(country).find(ls) != std::string::npos;
}

std::string Author::to_string() const
{
    fort::char_table t;
    t << fort::header << "Name" << "Books" << "Birthdate" << "Country" << "ID" << fort::endr;
    std::string delim;
    std::stringstream b;
    size_t i = 0;
    for (const auto& el: books)
    {
        i++;
        b << delim << i << ". " << el->getName();
        delim = "\n";
    }
    t << getName() << b.str() << date << country << id() << fort::endr;
    setTableProperties(t,0,4);
    return t.to_string();
}
Author::Author(Author&& a) noexcept: Entry(std::move(a)), country(std::move(a.country)), date(std::move(a.date)), books(std::move(a.books))
{
    for (const auto& b:books)
    {
        b->link(this);
        b->unlink(&a);
    }
#ifndef NDEBUG
    std::cout << getName() << " was moved\n";
#endif
}

//GENRE

Genre::~Genre()
{
#ifndef NDEBUG
    std::cout << "Genre " << this->getName() << " Destroyed" << std::endl;
#endif
    for (const auto& b: books)
        b->unlink(this);
}
bool Genre::link(Entry* pe)
{
    if (typeid(*pe) == typeid(Book))
    {
        books.insert(pe);
        return static_cast<Book*>(pe)->genres.insert(this).second;
    }
    else return false;
}
bool Genre::unlink(Entry* pe)
{
    if (typeid(*pe) == typeid(Book))
    {
        books.erase(pe);
        return static_cast<Book*>(pe)->genres.erase(this);
    }
    else return false;
}
bool Genre::check(const std::string& s) const
{
    std::string ls = lowercase(s);
    return lowercase(getName()).find(ls) != std::string::npos || s == std::to_string(id());
}

std::string Genre::to_string() const
{
    fort::char_table t;
    t << fort::header << "Name" << "Book quantity" << "ID" << fort::endr;
        t << getName() << books.size() << id() << fort::endr;
    setTableProperties(t,0,2);
    return t.to_string();
}
Genre::Genre(Genre&& g) noexcept: Entry(std::move(g)), books(std::move(g.books))
{
    for (const auto& b:books)
    {
        b->link(this);
        b->unlink(&g);
    }
#ifndef NDEBUG
    std::cout << getName() << " was moved\n";
#endif
}
