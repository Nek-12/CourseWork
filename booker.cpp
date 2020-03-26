#include "header.h"
#include <iostream>

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
    remFromAuthors();
    remFromGenres();
}

void Book::addAuthor(Author& a)
{
    authors.insert(&a);
    a.addBook(*this);
}
void Book::addGenre(Genre& g)
{
    genres.insert(&g);
    g.addBook(*this);
}
void Book::remAuthor(Author& a)
{
    authors.erase(&a);
    a.remBook(*this);
}
void Book::remGenre(Genre& g)
{
    genres.erase(&g);
    g.remBook(*this);
}
void Book::print()
{
    std::cout << id << ", " << year << ", " << name << "\n Authors: \n";
    for (const auto& a: authors)
    {
        std::cout << a->name << ", ";
    }
    std::cout << "\n Genres: \n";
    for (const auto& g: genres)
    {
        std::cout << g->name << ", ";
    }
    std::cout << std::endl;
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
    a.addGenre(*this);
}
void Genre::addBook(Book& b)
{
    books.insert(&b);
    b.addGenre(*this);
}
void Genre::remAuthor(Author& a)
{
    authors.erase(&a);
    a.remGenre(*this);
}
void Genre::remBook(Book& b)
{
    books.erase(&b);
    b.remGenre(*this);
}
void Genre::print()
{
    std::cout << id << ", " << name << "\n Authors: \n";
    for (const auto& a: authors)
    {
        std::cout << a->name << ", ";
    }
    std::cout << "\n Books: \n";
    for (const auto& b: books)
    {
        std::cout << b->name << ", ";
    }
    std::cout << std::endl;
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
    g.addAuthor(*this);
}
void Author::addBook(Book& b)
{
    books.insert(&b);
    b.addAuthor(*this);
}
void Author::remGenre(Genre& g)
{
    genres.erase(&g);
    g.remAuthor(*this);
}
void Author::remBook(Book& b)
{
    books.erase(&b);
    b.remAuthor(*this);
}
void Author::print()
{
    std::cout << id << ", " << name << ", " << country << ", " << date << "\n Books: \n";
    for (const auto& b: books)
    {
        std::cout << b->name << ", ";
    }
    std::cout << "\n Genres: \n";
    for (const auto& g: genres)
    {
        std::cout << g->name << ", ";
    }
    std::cout << std::endl;
}

