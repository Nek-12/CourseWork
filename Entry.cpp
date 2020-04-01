#include "header.h"

std::ostream& operator<<(std::ostream& os, const Entry& e )
{
    os <<  e.to_string();
    return os;
}

//BOOK

Book::~Book()
{
    for (auto id: this->authors)
        Data::authors.find(id)->second.remBook(*this);
    for (auto id: this->genres)
        Data::genres.find(id)->second.remBook(*this);
}
void Book::addAuthor(Author& a)
{
    authors.insert(a.id());
    a.books.insert(id());
}
void Book::addGenre(Genre& g)
{
    genres.insert(g.id());
    g.books.insert(id());
}
void Book::remAuthor(Author& a )
{
    authors.erase(a.id());
    a.books.erase(id());
}
void Book::remGenre(Genre& g )
{
    genres.insert(g.id());
    g.books.insert(id());
}

std::string Book::to_string() const
{
    std::string ret = std::to_string(id()) + '\n' + name() + '\n' + std::to_string(year);
    std::string delim = "\n";
    for (auto el: genres)
    {
        ret += delim + Data::authors.find(el)->second.name();
        delim = ",";
    }
    delim = "\n";
    for (auto el: authors)
    {
        ret += delim + Data::authors.find(el)->second.name();
        delim = ",";
    }
    return ret;
}

//Author

Author::~Author()
{
    for (auto id: this->books)
        Data::books.find(id)->second.remAuthor(*this);
    for (auto id: this->genres)
        Data::genres.find(id)->second.remAuthor(*this);
}
void Author::addGenre(Genre& g )
{
    genres.insert(g.id());
    g.authors.insert(id());
}
void Author::addBook(Book& b)
{
    books.insert(b.id());
    b.authors.insert(id());
}
void Author::remGenre(Genre& g )
{
    genres.erase(g.id());
    g.authors.erase(id());
}
void Author::remBook(Book& b )
{
    books.erase(b.id());
    b.authors.erase(id());
}
std::string Author::to_string() const
{
    std::string ret = std::to_string(id()) + '\n' + name() + '\n' + date + '\n' + country;
    std::string delim = "\n";
    for (auto el: books)
    {
        ret += delim + Data::books.find(el)->second.name();
        delim = ",";
    }
    delim = "\n";
    for (auto el: genres)
    {
        ret += delim + Data::genres.find(el)->second.name();
        delim = ",";
    }
    return ret;
}

//GENRE

Genre::~Genre()
{
    for (auto id: this->authors)
        Data::authors.find(id)->second.remGenre(*this);
    for (auto id: this->books)
        Data::books.find(id)->second.remGenre(*this);
}
void Genre::addAuthor(Author& a )
{
 authors.insert(a.id());
 a.genres.insert(id());
}
void Genre::addBook(Book& b )
{
    books.insert(b.id());
    b.genres.insert(id());
}
void Genre::remAuthor(Author& a )
{
    authors.erase(a.id());
    a.genres.erase(id());
}
void Genre::remBook(Book& b )
{
    books.erase(b.id());
    b.genres.erase(id());
}
std::string Genre::to_string() const
{
    std::string ret = std::to_string(id()) + '\n' + name();
    std::string delim = "\n";
    for (auto el: books)
    {
        ret += delim + Data::books.find(el)->second.name();
        delim = ",";
    }
    delim = "\n";
    for (auto el: authors)
    {
        ret += delim + Data::authors.find(el)->second.name();
        delim = ",";
    }
    return ret;
}

//USER

std::string User::to_string() const
{

}

//ADMIN

std::string Admin::to_string() const
{
    return;
}

//JOURNAL
//template <typename T>
//bool Journal<T>::insert(T&& e )
//{
//
//}
//template <typename T>
//bool Journal<T>::erase(T& e )
//{
//
//}
//template <typename T>
//std::shared_ptr<T>& Journal<T>::find(ull id) //TODO: Return pointer or ref?
//{
//    for (auto p: entries)
//        if (id == *p) return p;