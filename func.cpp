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
    return true;
}


bool Data::upass(std::string &l, std::string &p) //TODO: Implement check pass + decrypt
{
    return true;
}


void Book::printInfo()
{
    std::cout << this->name << " " << this->author << " " << this->created << " " << this->isbn;
}