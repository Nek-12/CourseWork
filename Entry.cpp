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
void Book::addGenre(Genre&)
{

}
void Book::remAuthor(Author&)
{

}
void Book::remGenre(Genre&)
{

}
bool Book::check(const std::string&)
{
    return false;
}
std::string Book::to_string() const
{
    return std::string();
}

//Author

Author::~Author()
{
    for (auto id: this->books)
        Data::books.find(id)->second.remAuthor(*this);
    for (auto id: this->genres)
        Data::genres.find(id)->second.remAuthor(*this);
}
void Author::addGenre(Genre&)
{

}
void Author::addBook(Book&)
{

}
void Author::remGenre(Genre&)
{

}
void Author::remBook(Book&)
{

}
bool Author::check(const std::string& s)
{
    return true;
}
std::string Author::to_string() const
{
    return;
}

//GENRE

Genre::~Genre()
{
    for (auto id: this->authors)
        Data::authors.find(id)->second.remGenre(*this);
    for (auto id: this->books)
        Data::books.find(id)->second.remGenre(*this);
}
void Genre::addAuthor(Author&)
{

}
void Genre::addBook(Book&)
{

}
void Genre::remAuthor(Author&)
{

}
void Genre::remBook(Book&)
{

}
bool Genre::check(const std::string& s)
{
    return true;
}
std::string Genre::to_string() const
{
    return;
}

//USER

std::string User::to_string() const
{
    return;
}

//ADMIN

std::string Admin::to_string() const
{
    return;
}

//JOURNAL

//bool Journal::insert(Entry&& e)
//{
//    return false;
//}
//bool Journal::erase(const Entry& e)
//{
//    return false;
//}
//std::shared_ptr<Entry>& Journal::find(const ull& id)
//{
//
//}