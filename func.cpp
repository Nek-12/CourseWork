#include "header.h"

void sleep(const unsigned& ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::string hash(const std::string& s)
{
    SHA256 sha256;
    return sha256(s);
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
        tm* now_tm = localtime(&t);
        int day = std::stoi(res.str(1));
        int month = std::stoi(res.str(3));
        int year = std::stoi(res.str(5));
        if (res.str(2) != res.str(4)) msgFalse("Divisors don't match:" + res.str(2) + " < =/= > " + res.str(4));

        if (year > (now_tm->tm_year + 1900))
            msgFalse("The book was created in the future year: " + std::to_string(year));
        else if ((year == now_tm->tm_year + 1900) && month > now_tm->tm_mon + 1)
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
            case 4:
            case 6:
            case 9:
            case 11:
                if (day > 30)
                    msgFalse("More than 30 days");
                else
                    return true;
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
    int year = 0;
    time_t t = time(nullptr); //TODO: Optimize for less objects;
    tm* now_tm = localtime(&t);
    for (auto ch: s)
        if(!isdigit(ch) || s.size() > 4)
            return false;
    year = std::stoi(s);
    return (year < now_tm->tm_year + 1900 && year > 200 ); //The first book appeared certainly later
}

bool checkString(const std::string& s, char mode)
{
    auto msgFalse = [& s](const std::string& msg) { std::cerr << "The value " << s << " is invalid: \n" << msg << std::endl; return false; };
    if (s.size() < 3 )
        msgFalse("The value should be longer than 3 characters.");
    switch (mode)
    {
        case 'p':
        case 'n':
            for (auto ch: s)
                if (!(isalnum(ch) || ch == '.' || ch == '-' || ch == '_' || ch == '\''))
                    msgFalse("invalid characters");
            break;
        case 's':
            for (auto ch: s)
                if (!(isalnum(ch) || ispunct(ch) || ch == ' '))
                    msgFalse("invalid characters");
            break;
        case 'd':
            return (checkDate(s));
        case 'i':
            for (auto ch: s)
                if(!isdigit(ch))
                    msgFalse("invalid characters");
            break;
        case 'y':
            if (!checkYear(s)) msgFalse("invalid year");
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
    if (!std::getline(is, s)) throw std::runtime_error("Critical read failure");

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
    for (auto ch: ret)
        ch = tolower(ch);
    return ret;
}

