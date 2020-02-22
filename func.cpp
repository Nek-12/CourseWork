#include "pch.h"

void sleep(const int &seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

std::string hash(const std::string& s)
{
    SHA256 sha256;
    return sha256(s);
}

bool readString(std::istream& is, std::string& s, char mode)// 's' for strings with spaces, 'n' for normal, 'd' for date
{
    std::getline(is, s);

    if (s.empty() || s.length() < 4)
    {
        std::cerr << "The data is too short, use more than 3 characters.Try again: " << std::endl;
        return false;
    }
    switch (mode)
    {
        case 'n':
            for (auto ch: s)
            {
                if ((!isdigit(ch) && !isalpha(ch) && !ispunct(ch)))
                {
                    std::cerr << "The value " << s
                              << " contains invalid characters. Use alphanumeric + punctuation this time: "
                              << std::endl;
                    return false;
                }
            }
            break;
        case 's':
            for (auto ch: s)
            {
                if ((!isdigit(ch) && !isalpha(ch) && !ispunct(ch)) && !isspace(ch))
                {
                    std::cerr << "The value " << s
                              << " contains invalid characters. Use alphanumeric + punctuation this time: "
                              << std::endl;
                    return false;
                }
            }
            break;
        case 'd':
            for (auto ch: s)
            {
                if ((!isdigit(ch) && !isalpha(ch) && !ispunct(ch)))
                {
                    std::cerr << "The value " << s
                              << " contains invalid characters. Use alphanumeric + punctuation this time: "
                              << std::endl;
                    return false;
                }
            }
            std::cout << "Parsing date not implemented " << std::endl; //TODO: Implement date
            break;
        default:
            throw std::invalid_argument("Bad argument for readString");
    }
    return true; //TODO: Test
}

//Data

bool Data::loginCheck(std::string &s, bool isadmin)
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
        std::cerr << "Access denied. " << std::endl;
        return false;
    }
}

bool Data::bookinit()
{
    std::string line, temp, name = "books.txt";
    std::ifstream f(path + "books.txt");
    if (!f) throw std::runtime_error("File " + path + name + " not found.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        //TODO: Implement behaviour for a fresh file
        Book book;
        if (!readString(f, line, 's')) break;
        book.name = line;

        if (!readString(f, line,'s')) break;
        book.author = line;

        if (!readString(f, line,'n')) break; //TODO: Implement datacheck
        book.isbn = line;

        if (!readString(f, line,'d')) break; //TODO: implement datacheck
        book.date = line;

        this->vbooks.push_back(book);

        if (!std::getline(f, line)) break;
        if (!line.empty() && line != " ")
            throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
    f.close();
    return false;
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
        tp << el.name << el.author << el.isbn << el.date;
    }
    tp.print();
}

bool Data::uinit()
{
    std::string login, pass, temp, name = "user.txt";
    if (!std::filesystem::exists(path + name))
    {
        std::cerr << "Warning! The file " << path << name << " does not exist! Creating a blank one..." << std::endl;
        std::ofstream f(path + "user.txt");
        f << "user\n" << hash("user") << "\n";
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
        f << "admin\n" << hash("admin") << "\n";
        f.close();
    }
    std::ifstream f(path + "admin.txt");
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

void Data::printCredentials(char which) // 'a' for admin, 'u' for user
{
    switch (which)
    {
        case 'a':
            std::cout << "\nAdmin credentials : \n" << std::endl;
            for (const auto& el: this->madm())
                std::cout << el.first << ", ";
            std::cout << std::endl;
            break;
        case 'u':
            std::cout << "\nUser credentials : \n" << std::endl;
            for (const auto& el: this->muser())
                std::cout << el.first << ", ";
            std::cout << std::endl;
            break;
        default:
            throw std::invalid_argument("Bad parameter for printCredentials");
    }
}

void Data::save()
{
    std::cout << "Saving..." << std::endl;
    std::ofstream fbook(path + "books.txt");
    std::ofstream fuser(path + "user.txt");
    std::ofstream fadm(path + "admin.txt");
#ifdef DEBUG
    if (!fadm.good()) std::cerr << "Fadm BAD\n"; else std::cerr << "Fadm OK\n";
    if (!fbook.good()) std::cerr << "Fbook BAD\n"; else std::cerr << " Fbook OK\n";
    if (!fuser.good()) std::cerr << "Fuser BAD\n"; else std::cerr << " Fuser OK\n";
    printbooks();
    printCredentials('a');
    printCredentials('u');
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
        //td::cout << el.name << "\n" << el.author << "\n" << el.isbn << "\n" << el.date << std::endl;
        fbook << el.name << "\n" << el.author << "\n" << el.isbn << "\n" << el.date << "\n" << std::endl;
    }

    fadm.close();
    fuser.close();
    fbook.close();
}

//TODO: Check if functions throw when the work was disrupted and return false when okay