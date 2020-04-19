#include "header.h"

//BOOK

Book::~Book()
{
#ifndef NDEBUG
    std::cout << "Book " << getName() << " Destroyed" << std::endl;
#endif
    for (const auto& g: genres)
        g->remBook(*this);
    for (const auto& a: authors)
        a->remBook(*this);
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
Book::Book(Book&& b) noexcept: Entry(std::move(b)), year(b.year), authors (std::move(b.authors)), genres(std::move(b.genres))
{
    for (const auto& a: authors)
    {
        a->addBook(*this);
        a->remBook(b);
    }
    for (const auto& g:genres)
    {
        g->addBook(*this);
        g->remBook(b);
    }
#ifndef NDEBUG
    std::cout << getName() << " was moved\n";
#endif
}

//AUTHOR

Author::~Author()
{
    for (const auto& b: books)
        b->remAuthor(*this);
#ifndef NDEBUG
    std::cout << "Author " << this->getName() << " Destroyed" << std::endl;
#endif
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
Author::Author(Author&& a) noexcept: Entry(std::move(a)), country(std::move(a.country)), date(std::move(a.date)), books(std::move(a.books))
{
    for (const auto& b:books)
    {
        b->addAuthor(*this);
        b->remAuthor(a);
    } //TODO: Do I need moving at all?
    std::cout << getName() << " was moved\n";
}

//GENRE

Genre::~Genre()
{
#ifndef NDEBUG
    std::cout << "Genre " << this->getName() << " Destroyed" << std::endl;
#endif
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
Genre::Genre(Genre&& g) noexcept: Entry(std::move(g)), books(std::move(g.books))
{
    for (const auto& b:books)
    {
        b->addGenre(*this);
        b->remGenre(g);
    } //TODO: Do I need moving at all?
#ifndef NDEBUG
    std::cout << getName() << " was moved\n";
#endif
}
