#include "header.h"

//BOOK

Book& Book::operator=(const Book& rhs)
{
    remFromAuthors();
    remFromGenres();
    //Handles self-assignment by removing pointers first
    id = rhs.id;
    name = rhs.name;
    year = rhs.year;
    authors = rhs.authors;
    genres = rhs.genres;

    addToAuthors(rhs);
    addToGenres(rhs);
    return *this;
}
Book::~Book()
{
    std::cout << "Book " << this->name << " Destroyed" << std::endl;
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
std::ostream &operator<<(std::ostream &os, const Book& b)
{
    os << b.id << "\n" << b.name << "\n" << std::setfill('0') << std::setw(4) << b.year << "\n";
    std::string delim;
    for (auto& g: b.genres)
    {
        os << delim << g->name;
        delim = ',';
    }
    os << "\n";
    delim.clear();
    for (auto& a: b.authors)
    {
        os << delim << a->name;
        delim = ',';
    }
    os << "\n" << std::setfill(' ') << std::endl;
    return os;
}

void Book::addToGenres(const Book& b)
{
    for (auto g: b.genres)
        g->addBook(*this);
}
void Book::addToAuthors(const Book& b)
{
    for (auto a: b.authors)
        a->addBook(*this);
}
void Book::remFromGenres()
{
    for (auto g: genres)
        g->remBook(*this);
}
void Book::remFromAuthors()
{
    for (auto a: authors)
        a->remBook(*this);
}
bool Book::check(const std::string& s)
{
    std::cout << "Function check was given " << s;
    if (name.find(s) != std::string::npos || s == std::to_string(year) || std::stoull(s) == id ) return true;
    for (auto el: genres)
        if (el->name.find(s) != std::string::npos) return true;
    for (auto el: authors)
        if (el->name.find(s) != std::string::npos) return true;
    return false;
}

//GENRE

Genre& Genre::operator=(const Genre& rhs)
{
    remFromAuthors();
    remFromBooks();
    //Handles self-assignment by removing pointers first
    id = rhs.id;
    name = rhs.name;
    authors = rhs.authors;
    books = rhs.books;

    addToAuthors(rhs);
    addToBooks(rhs);
    return *this;
}

Genre::~Genre()
{
    std::cout << "Genre " << this->name << " Destroyed" << std::endl;
    remFromAuthors();
    remFromBooks();
}


void Genre::addToBooks(const Genre& g)
{
    for (auto b: g.books)
        b->addGenre(*this);
}
void Genre::addToAuthors(const Genre& g)
{
    for (auto b: g.books)
        b->addGenre(*this);
}
void Genre::remFromBooks()
{
    for (auto b: books)
        b->remGenre(*this);
}
void Genre::remFromAuthors()
{
    for (auto a: authors)
        a->remGenre(*this);
}

void Genre::addAuthor(Author& a)
{
    authors.insert(&a);
    a.genres.insert(this);
}
void Genre::addBook(Book& b)
{
    books.insert(&b);
    b.genres.insert(this);
}
void Genre::remAuthor(Author& a)
{
    authors.erase(&a);
    a.genres.erase(this);
}
void Genre::remBook(Book& b)
{
    books.erase(&b);
    b.genres.erase(this);
}
std::ostream& operator<<(std::ostream &os, const Genre& g)
{
    os << g.id << "\n" << g.name << "\n";
    return os;
}

bool Genre::check(const std::string& s)
{
    if (name.find(s) != std::string::npos || s == id ) return true;
    for (auto el: books)
        if (el->name.find(s) != std::string::npos) return true;
    for (auto el: authors)
        if (el->name.find(s) != std::string::npos) return true;
    return false;
}

//AUTHOR

Author& Author::operator=(const Author& rhs)
{
    remFromGenres();
    remFromBooks();
    id = rhs.id;
    name = rhs.name;
    country = rhs.country;
    date = rhs.date;
    books = rhs.books;
    genres = rhs.genres;

    addToBooks(*this);
    addToGenres(*this);
    return *this;
}
Author::~Author()
{
    std::cout << "Author " << this->name << " Destroyed" << std::endl;
    remFromBooks();
    remFromGenres();
}

void Author::addToBooks(const Author& a)
{
    for (auto b: a.books)
        b->addAuthor(*this);
}
void Author::addToGenres(const Author& a)
{
    for (auto g: a.genres)
        g->addAuthor(*this);
}
void Author::remFromBooks()
{
    for (auto b: books)
        b->remAuthor(*this);
}
void Author::remFromGenres()
{
    for (auto g: genres)
        g->remAuthor(*this);
}

void Author::addGenre(Genre& g)
{
    genres.insert(&g);
    g.authors.insert(this);
}
void Author::addBook(Book& b)
{
    books.insert(&b);
    b.authors.insert(this);
}
void Author::remGenre(Genre& g)
{
    genres.erase(&g);
    g.authors.erase(this);
}
void Author::remBook(Book& b)
{
    books.erase(&b);
    b.authors.erase(this);
}
std::ostream& operator<<(std::ostream &os, const Author& a)
{
    os << a.id << "\n" << a.name << "\n" << a.date << "\n" << a.country << "\n";
    return os;
}
bool Author::check(const std::string& s)
{
    return (s == name || std::stoull(s) == id );
}

