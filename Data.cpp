#include "header.h"


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
    tp.addColumn("Title", 42);
    tp.addColumn("Author", 42);
    tp.addColumn("ISBN", 16);
    tp.addColumn("Date", 10);
    for (const auto& el : this->vbooks)
    {
        tp << el.title << el.author << el.isbn << el.date;
    }
    tp.print();
}


bool Data::bookinit() //TODO: Refactor
{
    std::string line, temp, name = "books.txt";
    try
    {
        if (!std::filesystem::exists(path + name))
        {
            std::cerr << "Warning! The file " << path << name << " does not exist! Creating a blank one..."
                      << std::endl;
            std::ofstream f(path + name);
            f.close();
        }
        std::ifstream f(path + name);
        if (!f) return false;
        do //Starts parsing the file. Paragraphs are divided by a blank line
        {
            //TODO: Implement behaviour for a fresh file
            Book book;
            if (!readString(f, line, 's')) throw std::runtime_error("Error reading title: " + line);
            book.title = line;

            if (!readString(f, line, 's')) throw std::runtime_error("Error reading author: " + line);
            book.author = line;

            if (!readString(f, line, 'n'))
                throw std::runtime_error("Error reading isbn: " + line); //TODO: Implement datacheck
            book.isbn = line;

            if (!readString(f, line, 'd'))
                throw std::runtime_error("Error reading date: " + line);
            book.date = line;

            this->vbooks.push_back(book);

            if (!getline(f, line)) throw std::runtime_error("Error in separator, found this: " + line);
            if (!line.empty() && line != " ")
                throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

            //continues to read if f is good;
        } while (f);
        f.close();
        return true;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Encountered file " << name << " parsing error: " << e.what();
        return false;
    }

}


bool Data::uinit()
{
    std::string login, pass, temp, name = "user.txt";
    if (!std::filesystem::exists(path + name))
    {
        std::cerr << "Warning! The file " << path << name << " does not exist! Creating a blank one..." << std::endl;
        std::ofstream f(path + name);
        //f << "user\n" << hash("user") << "\n";
        f.close();
    }

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
    f.close();
    return true; //TODO: Test
}

bool Data::adminit()
{
    std::string login, pass, temp, name = "admin.txt";
    if (!std::filesystem::exists(path + name))
    {
        std::cout << "Warning! The file " << path << name << " does not exist! Creating a blank one..." << std::endl;
        std::ofstream f(path + name);
        //f << "admin\n" << hash("admin") << "\n";
        f.close();
    }
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
    f.close();
    return true; //TODO: Implement
}

void Data::printCredentials(bool isAdmin)
{
    if (isAdmin)
    {
        std::cout << "\nAdmin credentials : \n" << std::endl;
        for (const auto& el: this->madm())
            std::cout << el.first << "\n";
        std::cout << std::endl;
    }
    else
    {
        std::cout << "\nUser credentials : \n" << std::endl;
        for (const auto& el: this->muser())
            std::cout << el.first << "\n";
        std::cout << std::endl;
    }
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

Data::Data()
{
}

