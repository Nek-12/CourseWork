#include "header.h"
//TODO: Remove output from class functions maybe

bool Data::loginCheck(std::string& s, bool isadmin)
{
    if ((isadmin ? this->madm().find(s) != this->madm().end()
                 : this->muser().find(s) != this->muser().end()))
    {
        std::cout << "Found user " << s << " ." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "User not found." << std::endl;
        return false;
    }
}

bool Data::passCheck(const std::string& l, const std::string& p, bool isadmin) //1 for admin, 0 for user
{
    if ((isadmin ? this->madm().find(l) : this->muser().find(l))->second == hash(p))
    {
        std::cout << "Access granted." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "Access denied." << std::endl;
        return false;
    }
}

void Data::printbooks()
{
    TablePrinter tp; //Makes use of table_printer library
    tp.alignCenter();
    tp.setPadding(1);
    tp.setDashedRawsStyle();
    tp.addMergedColumn("Book Database");
    tp.addColumn("Title", 30);
    tp.addColumn("Authors", 40);
    tp.addColumn("Genres",40);
    tp.addColumn("ID", 9);
    tp.addColumn("Date", 10);
    for (const auto& book: this->vbooks)
    {
        std::string authors, genres;
        for (auto g: book.genres)
            genres+= g->name + ", ";
        for (auto a: book.authors)
            authors+= a->name + ", ";

        tp << book.title << authors << genres << book.id << book.date;
    }
    tp.print();
}

void doesFileExist(const std::string& f)
{
    if (!std::filesystem::exists(path + f))
    {
        std::cerr << "Warning! The file " << path << f << " does not exist! Creating a blank one..."
                  << std::endl;
        std::ofstream file(path + f);
        file.close();
    }
}

std::string readline(const std::istream& is, )

void Data::bookinit()
{
    std::string bfname = "books.txt", gfname = "genres.txt", afname = "authors.txt";
    doesFileExist(bfname);
    doesFileExist(gfname);
    doesFileExist(afname);
    std::ifstream bf(path + bfname);
    std::ifstream gf(path + gfname);
    std::ifstream af(path + afname);

    while(gf)
    {
        std::string id, name;
        if (!readString(gf, id, 'n')) throw std::runtime_error("Error reading genre: " + id);
        if (!readString(gf, name, 's')) throw std::runtime_error("Error reading genre: " + name);

    }

    while (f)
    {
        std::string id, genre, name, title, date, country;
        if (!readString(f, line, 's')) throw std::runtime_error("Error reading title: " + line);
        book.title = line;

        if (!readString(fb, line, 's')) throw std::runtime_error("Error reading author: " + line);
        book.author = line;

        if (!readString(fb, line, 'n'))
            throw std::runtime_error("Error reading isbn: " + line); //TODO: Implement datacheck
        book.isbn = line;

        if (!readString(fb, line, 'd'))
            throw std::runtime_error("Error reading date: " + line);
        book.date = line;

        this->vbooks.push_back(book);

        if (!getline(f, line)) throw std::runtime_error("Error in separator, found this: " + line);
        if (!line.empty() && line != " ")
            throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
}


void Data::uinit()
{
    std::string login, pass, temp, name = "user.txt";
    doesFileExist(name);
    std::ifstream f(path + name);
    if (!f) throw std::runtime_error("File " + path + name + " could not be opened after creating.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (!readString(f, login, 'n')) break;
        if (!readString(f, pass, 'n')) break;
        (this->muser())[login] = pass; //read pass, login and add them to the map. Duplicates removed.
        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ")
            throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
}

void Data::adminit()
{
    std::string login, pass, temp, name = "admin.txt";
    doesFileExist(name);
    std::ifstream f(path + name);
    if (!f) throw std::runtime_error("File " + path + name + " could not be opened.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (!readString(f, login, 'n')) break;
        if (!readString(f, pass, 'n')) break;
        (this->madm())[login] = pass; //read pass, login and add them to the map. Duplicates removed.

        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ")
            throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
    if (this->madm().empty())
    {
        std::cerr << "Warning! We couldn't find any valid administrator accounts. \n"
                     "Created a new one: admin | admin" << std::endl;
        this->madm()["admin"] = hash("admin");
    }
}

void Data::printCredentials(bool isAdmin)
{
        std::cout << (isAdmin ? "Admin" : "User") << " credentials: " << std::endl;
        for (const auto& el: ( isAdmin ? this->madm() : this->muser() ) )
            std::cout << el.first << "\n";
        std::cout << std::endl;
}

void Data::save()
{
    std::cout << "Saving..." << std::endl;
    std::ofstream fbook(path + "books.txt");
    std::ofstream fuser(path + "user.txt");
    std::ofstream fadm(path + "admin.txt");
#ifndef NDEBUG
    if (!fadm.good()) std::cerr << "Fadm BAD\n"; else std::cerr << "Fadm OK\n";
    if (!fbook.good()) std::cerr << "Fbook BAD\n"; else std::cerr << " Fbook OK\n";
    if (!fuser.good()) std::cerr << "Fuser BAD\n"; else std::cerr << " Fuser OK\n";
    printbooks();
    printCredentials(true);
    printCredentials(false);
#endif

    for (auto& el: this->madm())
    {
        //std::cout << el.first << "\n" << el.second << std::endl;
        fadm << el.first << "\n" << el.second << "\n" << std::endl;
    }
    for (auto& el: this->muser())
    {
        //std::cout << el.first << "\n" << el.second << std::endl;
        fuser << el.first << "\n" << el.second << "\n" << std::endl;
    }
    for (auto& el: this->vBooks())
    {
        //std::cout << el.name << "\n" << el.author << "\n" << el.isbn << "\n" << el.date << std::endl;
        fbook << el.title << "\n" << el.author << "\n" << el.isbn << "\n" << el.date << "\n" << std::endl;
    }
    //The destructor will close the files for me
}

