#include "header.h"

void sleep(const nat& ms)
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

bool checkString(const std::string& s, char mode)
{
    auto msgFalse = [& s]() { std::cerr << "The value " << s << " is invalid." << std::endl; return false; };
    if (s.empty() || s.length() < 2)
    {
        std::cerr << "The data is too short, use more than 2 characters. Try again: " << std::endl;
        return false;
    }
    switch (mode)
    {
        case 'p':
            for (auto ch: s)
                if (!(isalnum(ch) ) )
                    msgFalse();
        case 'n':
            for (auto ch: s)
                if (!(isalnum(ch) || ch == '.' || ch == '-' || ch == '_' || ch == '\''))
                    msgFalse();
            break;
        case 's':
            for (auto ch: s)
                if (!(isalnum(ch) || ispunct(ch) || ch == ' '))
                    msgFalse();
            break;
        case 'd':
            return (checkDate(s));
        default:
            throw std::invalid_argument("Bad argument for checkString");
    }
    return true;
}

//void readPassConsole(std::string& s) //Crutch
//{
//    std::cout << "Keyboard layout: ENG (US)" << std::endl << std::unitbuf; //No buffering to erase chars properly
//    std::string pass;
//    FlushConsoleInputBuffer( GetStdHandle( STD_INPUT_HANDLE ) );
//    sleep(100);
//        while (!(GetAsyncKeyState(VK_RETURN) & 1) )
//        {
//            for (int i = 0x30; i <= 0x69; i++)
//            {
//                if (GetAsyncKeyState(i) & 1)
//                {
//                    if (i >= 0x41 && i <= 0x5A &&
//                        ((GetKeyState(VK_CAPITAL) & 1) == 0 || GetAsyncKeyState(VK_SHIFT) & 1))
//                        pass += ((char) (i + 32));
//                    else if (i >= 0x41 && i <= 0x5A)
//                        pass += (char) i;
//                    else if (i >= 0x60 && i <= 0x69)
//                        pass += (char) (i - 48);
//                    else continue;
//                    std::cout << "*";
//                    sleep(25);
//                }
//                else if (GetAsyncKeyState(VK_BACK) & 1)
//                {
//                    if (!pass.empty())
//                    {
//                        pass.erase(pass.size() - 1);
//                        std::cout << "\b \b";  //Cursor moves 1 position backwards
//                        sleep(50);
//                    }
//                }
//            }
//        }
//    std::cout << std::nounitbuf << std::endl;
//    s = pass;
//}
//
//
bool readString(std::istream& is, std::string& ret, char mode = 'n')
 // 's' for strings with spaces, 'n' for normal, 'd' for date, 'p' for password
{
    std::string s;
    //if (mode == 'p')
        //readPassConsole(s);
    //else
        std::getline(is, s);

    if (checkString(s, mode))
    {
        ret = s;
        return true;
    }
    else return false;
}



//TODO: Check if functions throw when the work was disrupted and return false when okay