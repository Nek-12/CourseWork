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

bool readString(std::istream& is,  std::string &s, char mode)
// 's' for strings with spaces, 'n' for normal, 'd' for date
{
    if(!std::getline(is, s))
    {
        std::cerr << "\n Something went wrong. Try again: " << std::endl;
        return false;
    }
    if (s.empty() || s.length() < 4)
    {
        std::cerr << "\n The data is too short, use more than 3 characters.Try again: " << std::endl;
        return false;
    }
    switch (mode)
    {
        case 'n':
            for (auto ch: s)
            {
                if ((!isdigit(ch) && !isalpha(ch) && !ispunct(ch)))
                {
                    std::cerr << "\n The value " << s << " contains invalid characters. Use alphanumeric + punctuation this time: " << std::endl;
                    return false;
                }
            }
            break;
        case 's':
            for (auto ch: s)
            {
                if ((!isdigit(ch) && !isalpha(ch) && !ispunct(ch)) && !isspace(ch))
                {
                    std::cerr << "\n The value " << s << " contains invalid characters. Use alphanumeric + punctuation this time: " << std::endl;
                    return false;
                }
            }
            break;
        case 'd':
            for (auto ch: s)
            {
                if ((!isdigit(ch) && !isalpha(ch) && !ispunct(ch)))
                {
                    std::cerr << "\n The value " << s << " contains invalid characters. Use alphanumeric + punctuation this time: " << std::endl;
                    return false;
                }
            }
            std::cout << "\n parsing date not implemented " << std::endl; //TODO: Implement date
            break;
        default: throw std::invalid_argument("Bad argument for readString");
    }
    return true; //TODO: Test
}

//Data

bool Data::loginCheck(std::string &s, bool isadmin)
{
    if ( (isadmin? this->getmadm().find(s) != this->getmadm().end()
                 : this->getmuser().find(s) != this->getmuser().end() ) )
    {
        std::cout << "Welcome, " << s << " ." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "User not found." << std::endl;
        return false;
    }
}

bool Data::passCheck(std::string &l, std::string &p, bool isadmin) //1 for admin, 0 for user
{
    if ((isadmin ? this->getmadm().find(l) : this->getmuser().find(l) )->second == hash(p) )
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

bool Data::bookinit(const std::string &path)
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
        if (!line.empty() && line != " ") throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
    return false;
}

void Data::printbooks()
{
    for (auto el : this->vbooks)
    {
        el.printInfo();
        std::cout << std::endl;
    }
}

bool Data::uinit(const std::string &path)
{
    std::string login, pass, temp, name = "user.txt";
    if (!std::filesystem::exists(path + name))
    {
        std::ofstream f( path + "user.txt");
        f << "user\n" << hash("user") << std::endl;
        f.close();
    }
    std::ifstream f(path + name);
    if (!f) throw std::runtime_error("File " + path + name + " could not be opened after creating.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (!readString(f, login,'n')) break;
        if (!readString(f, pass,'n')) break;
        (this->getmuser())[login] = pass; //read pass, login and add them to the map. Duplicates removed.

        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ") throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
    return true; //TODO: Test
}

bool Data::adminit(const std::string &path)
{
    std::string login, pass, temp, name = "admin.txt";
    if (!std::filesystem::exists(path + name))
    {
        std::ofstream f(path + name);
        f << "admin\n" << hash("admin") << std::endl;
        f.close();
    }
    std::ifstream f(path + "admin.txt");
    if (!f) throw std::runtime_error("File " + path + name + " could not be opened.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        if (!readString(f, login,'n')) break;
        if (!readString(f, pass,'n')) break;
        (this->getmadm())[login] = pass; //read pass, login and add them to the map. Duplicates removed.

        if (!std::getline(f, temp)) break;
        if (!temp.empty() && temp != " ") throw std::invalid_argument("File " + path + name + " read error, check delimiters.");

        //continues to read if f is good;
    }
    return true; //TODO: Implement
}

void Data::printCredentials(char which) // 'a' for admin, 'u' for user;
{
    switch (which)
    {
        case 'a':
            std::cout << "Admin credentials : " << std::endl;
            for (const auto& el: this->getmadm())
                std::cout << el.first << " " << el.second<< std::endl;
            break;
        case 'u':
            std::cout << "User credentials : " << std::endl;
            for (const auto& el: this->getmuser())
                std::cout << el.first << " " << el.second << std::endl;
            break;
        default: throw std::invalid_argument("Bad parameter for printCredentials");
    }
}

void Book::printInfo() //TODO: Make it look like a table.
{
    std::cout << this->name << " " << this->author << " " << this->isbn << " " << this->date;
}


//TODO: Check if functions throw when the work was disrupted and return false when okay