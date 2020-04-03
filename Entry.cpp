#include "header.h"

std::ostream& operator<<(std::ostream& os, const Entry& e )
{
    os <<  e.to_string(0);
    return os;
}

//BOOK

Book::~Book()
{
    for (const auto& ptr: this->authors)
        if (!ptr.expired())
            ptr.lock()->remBook(shared_from_this());
    for (const auto& ptr: this->genres)
        if (!ptr.expired())
            (ptr.lock())->remBook(shared_from_this());
}
void Book::addAuthor(const sptr<Author>& p)
{
    authors.insert(p);
    p->books.insert(shared_from_this());
}
void Book::addGenre(const sptr<Genre>& p)
{
    genres.insert(p);
    p->books.insert(shared_from_this());
}
void Book::remAuthor(const sptr<Author>& p)
{
    authors.erase(p);
    p->books.erase(shared_from_this());
}
void Book::remGenre(const sptr<Genre>& p)
{
    genres.erase(p);
    p->books.erase(shared_from_this());
}

std::string Book::to_string(bool isUsingID) const
{
    std::string ret = std::to_string(id()) + '\n' + name() + '\n' + std::to_string(year);
    std::string delim = "\n";
    for (const auto& el: genres)
    {
        if (!el.expired())
            ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    delim = "\n";
    for (const auto& el: authors)
    {
        if (!el.expired())
            ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    return ret;
}

//Author

Author::~Author()
{
    for (const auto& ptr: this->books)
        if (!ptr.expired())
            ptr.lock()->remAuthor(shared_from_this());
    for (const auto& ptr: this->genres)
        if (!ptr.expired())
            (ptr.lock())->remAuthor(shared_from_this());
}
void Author::addGenre(const sptr<Genre>& p)
{
    genres.insert(p);
    p->authors.insert(shared_from_this());
}
void Author::addBook(const sptr<Book>& p)
{
    books.insert(p);
    p->authors.insert(shared_from_this());
}
void Author::remGenre(const sptr<Genre>&p)
{
    genres.erase(p);
    p->authors.erase(shared_from_this());
}
void Author::remBook(const sptr<Book>& p)
{
    books.erase(p);
    p->authors.erase(shared_from_this());
}

std::string Author::to_string(bool isUsingID ) const
{
    std::string ret = std::to_string(id()) + '\n' + name() + '\n' + date + '\n' + country;
    std::string delim = "\n";
    for (const auto& el: books)
    {
        ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    delim = "\n";
    for (const auto& el: genres)
    {
        ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    return ret;
}

//GENRE

Genre::~Genre()
{
    for (const auto& ptr: this->authors)
        if (!ptr.expired())
            ptr.lock()->remGenre(shared_from_this());
    for (const auto& ptr: this->books)
        if (!ptr.expired())
            (ptr.lock())->remGenre(shared_from_this());
}
void Genre::addAuthor(const sptr<Author>& p)
{
    authors.insert(p);
    p->genres.insert(shared_from_this());
}
void Genre::addBook(const sptr<Book>& p)
{
    books.insert(p);
    p->genres.insert(shared_from_this());
}
void Genre::remAuthor(const sptr<Author>& p)
{
    authors.erase(p);
    p->genres.erase(shared_from_this());
}
void Genre::remBook(const sptr<Book>& p)
{
    books.erase(p);
    p->genres.erase(shared_from_this());
}
std::string Genre::to_string(bool isUsingID) const
{
    std::string ret = std::to_string(id()) + '\n' + name();
    std::string delim = "\n";
    for (const auto& el: books)
    {
        ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    delim = "\n";
    for (const auto& el: authors)
    {
        ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    return ret;
}

//USER

std::string User::to_string(bool isUsingID) const
{
    std::string ret = std::to_string(id()) + '\n' + name();
    std::string delim = "\n";
    for (const auto& el: books)
    {
        ret += delim + (isUsingID? std::to_string(el.lock()->id()) : el.lock()->name());
        delim = ",";
    }
    return ret;
}

//ADMIN

std::string Admin::to_string(bool isUsingID_UNUSED) const
{
    return std::to_string(id()) + '\n' + name() + '\n' + status;
}

//JOURNAL

template <typename T>
bool Journal<T>::erase(const sptr<T>& )
{

}
template <typename T>
sptr<T> Journal<T>::find(const ull& id)
{
    for (auto p: entries)
        if ( *p == id ) return p;
}
template <typename T>
std::vector<sptr<T> > Journal<T>::multifind(const std::string& s)
{
    std::vector<sptr<T> > v;
    for (auto p: entries)
        if (p->to_string().find(s) != std::string::npos)
            v.push_back(p);
    return v;
}