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
void Book::remAuthor(const size_t& pos)
{
    if (pos >= authors.size()) throw std::invalid_argument("Deleting genre past the end of book " + getName());
    auto it = authors.begin();
    std::advance(it, pos);
    authors.erase(it);
}
void Book::remGenre(Genre& g)
{
    genres.erase(&g);
    g.books.erase(this);
}
void Book::remGenre(const size_t& pos)
{
    if (pos >= genres.size()) throw std::invalid_argument("Deleting genre past the end of book " + getName());
    auto it = genres.begin();
    std::advance(it, pos);
    genres.erase(it);
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
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(4).set_cell_content_fg_color(fort::color::red);
    return t.to_string();
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
void Author::remBook(const size_t& pos)
{
    if (pos >= books.size()) throw std::invalid_argument("Deleting genre past the end of book " + getName());
    auto it = books.begin();
    std::advance(it, pos);
    books.erase(it);
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
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(4).set_cell_content_fg_color(fort::color::red);
    return t.to_string();
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
    fort::char_table t;
    t << fort::header << "Name" << "Book quantity" << "ID" << fort::endr;
        t << getName() << books.size() << id() << fort::endr;
    t.set_cell_text_align(fort::text_align::center);
    t.set_border_style(FT_BASIC2_STYLE);
    t.column(0).set_cell_content_fg_color(fort::color::green);
    t.column(2).set_cell_content_fg_color(fort::color::red);
    return t.to_string();
}
