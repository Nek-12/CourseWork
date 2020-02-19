#include "pch.h"

void sleep(const int &seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

std::string read(std::istream is, int type = 0)
//Type = 0 for integer, Type = 1 for double
{
    int i = 0;
    bool good = false;
    std::string input;
    do
    {
        try
        {
            std::getline(is, input);
            if (input.begin() == input.end())
            {
                throw std::invalid_argument("Empty string");
            }
            for (auto ch : input)
            {
                if (!isdigit(ch) && (ch != '.') && (ch != '-'))
                {
                    throw std::invalid_argument("Not a number");
                }
                if (ch == '.')
                {
                    i++;
                }
            }
            if (i > type)
            {
                throw std::invalid_argument("Not a number");
            }
            if (type == 1)
            {
                std::stod(input);
            }
            if (type == 0)
            {
                std::stoi(input);
            }
            good = true;
        }
        catch (std::invalid_argument(&msg))
        {
            std::cerr << std::endl << "Wrong input: " << input << "\n" << msg.what() << std::endl;
            continue;
        }
    } while (!good);


    return (input);
}

//Data

bool Data::ulogin(std::string &s) //TODO: Implement check login
{
    std::cout << std::endl << "upass";
    return false;
}


bool Data::upass(std::string &l, std::string &p) //TODO: Implement check pass + decrypt
{
    std::cout << std::endl << "upass";
    return false;
}

bool Data::bookinit(const std::string &path)
{
    std::string line, temp;
    std::ifstream f(path + "books.txt");
    if (!f) throw std::runtime_error("File could not be opened.");
    while (f) //Starts parsing the file. Paragraphs are divided by a blank line
    {
        //TODO: Implement various input checks for each data member
        //TODO: Implement behaviour for a fresh file
        Book book;
        if (!std::getline(f, line)) break;
        book.name = line;

        if (!std::getline(f, line)) break;
        book.author = line;

        if (!std::getline(f, line)) break;
        book.isbn = line;

        if (!std::getline(f, line)) break;
        book.date = line;

        this->vbooks.push_back(book);

        if (!std::getline(f, line)) break;
        if (!line.empty() && line != " ") throw std::invalid_argument("File read error, check delimiters.");

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
    return false; //TODO: Implement
}

bool Data::adminit(const std::string &path)
{
    return false; //TODO: Implement
}


void Book::printInfo()
{
    std::cout << this->name << " " << this->author << " " << this->isbn << " " << this->date;
}