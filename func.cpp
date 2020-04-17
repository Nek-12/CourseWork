#include "header.h"
#include "sha256.h"
#include <thread>
#include <regex>
#include <random>
void sleep(const unsigned& ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

ull genID()
{
    static std::default_random_engine e(std::random_device{}());
    static std::uniform_int_distribution<ull> rng(0, MAX_ID);
    return rng(e);
}

std::string hash(const std::string& s)
{
    SHA256 sha256;
    return sha256(s);
}

unsigned getCurYear()
{
    time_t t = time(nullptr);
    tm* nowTm = localtime(&t);
    return (unsigned)nowTm->tm_year + 1900;
}

bool checkDate(const std::string& s)
{
    std::regex reg(R"((\d{1,2})([-. /])(\d{1,2})([-. /])(\d{4}))");
    std::smatch res;
    auto msgFalse = [& s](const std::string& what)
    {
        std::cerr << "The date " << s << " is invalid: " << what << std::endl;
        return false;
    };

    if (std::regex_match(s, res, reg))
    {
        time_t t = time(nullptr);
        tm* nowTm = localtime(&t);
        int day = std::stoi(res.str(1));
        int month = std::stoi(res.str(3));
        int year = std::stoi(res.str(5));
        if (res.str(2) != res.str(4)) msgFalse("Divisors don't match:" + res.str(2) + " < =/= > " + res.str(4));

        if (year > (nowTm->tm_year + 1900))
            msgFalse("The book was created in the future year: " + std::to_string(year));
        else if ((year == nowTm->tm_year + 1900) && month > nowTm->tm_mon + 1)
            msgFalse("The book was created in the future month: " + std::to_string(month));

        if (month > 12)
            msgFalse("More than 12 months");
        switch (month)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                if (day > 31)
                    msgFalse("More than 31 days");
                else
                    return true;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if (day > 30)
                    msgFalse("More than 30 days");
                else
                    return true;
                break;
            case 2:
                if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
                {
                    if (day > 29)
                        msgFalse("More than 29 days");
                    else
                        return true;
                }
                else
                {
                    if (day > 28)
                        msgFalse("More than 28 days");
                    else
                        return true;
                }
                break;
            default:
                throw std::invalid_argument("Default case when parsing month");
        }
    }
    else
        msgFalse("Wrong date formatting");
    return false;
}

bool checkYear(const std::string& s)
{
    for (auto ch: s)
        if(!isdigit(ch) || s.size() > 4)
            return false;
    return (std::stoul(s) < getCurYear() );
}

bool checkString(const std::string& s, char mode)
{
    auto msgFalse = [& s](const std::string& msg)
            { std::cerr << "The value " << s << " is invalid: \n" << msg << std::endl; return false; };
    if (s.empty())
        return msgFalse("No data?");
    switch (mode)
    {
        case 'p':
        case 'n':
            if (s.size() < 3) return msgFalse("too short for a word");
            for (auto ch: s)
                if (!(isalnum(ch) || ch == '.' || ch == '-' || ch == '_' || ch == '\''))
                    return msgFalse("invalid characters");
            break;
        case 's':
            if (s.size() < 2) return msgFalse("too short for a string");
            for (auto ch: s)
                if (!(isalnum(ch) || ispunct(ch) || ch == ' '))
                    return msgFalse("invalid characters");
            break;
        case 'd':
            return (checkDate(s));
        case 'i':
            if (s.size() > MAX_ID_LENGTH) return msgFalse("too long for a number");
            for (auto ch: s)
                if(!isdigit(ch))
                    return msgFalse("invalid characters in a number");
            break;
        case 'y':
            if (!checkYear(s)) return msgFalse("invalid year");
            break;
        default:
            throw std::invalid_argument("Bad argument for checkString");
    }
    return true;
}

bool readString(std::istream& is, std::string& ret, char mode = 'n')
 // 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for password
{
    std::string s;
    if (!std::getline(is, s)) return false;

    if (checkString(s, mode))
    {
        ret = s;
        return true;
    }
    else return false;
}

std::string lowercase(const std::string& s)
{
    std::string ret = s;
    for (auto& ch: ret)
        ch = tolower(ch);
    return ret;
}

