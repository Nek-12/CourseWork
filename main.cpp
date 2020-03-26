#include "header.h"
#include <iostream>
#include <conio.h>


std::string path; //extern global

inline void cls() {system("cls");}

//TODO: Implement sort, implement filtering (date)?
//TODO: Make the book table be displayed only by request

bool yesNo(const std::string& msg)
{
    std::cout << msg << " y/n" << std::endl;
    while(true)
    {
        switch (getch())
        {
            case 'y':
                return true;
            case 'n':
                return false;
            default :
                break;
        }
    }
}

bool passConfirm (std::string& p)
{
    Data& data = Data::getInstance();
    std::string tempA, tempB;
    while (true)
    {
        cls();
        std::cout << data.passprompt << std::endl;
        while (!readString(std::cin, tempA, 'p'));
        if (tempA == "exit") return false;

        std::cout << data.passconfirm << std::endl;
        while (!readString(std::cin, tempB, 'p'));
        if (tempB == "exit") return false;
        if (tempA == tempB)
            break;
        std::cerr << "Your passwords don't match." << std::endl;
    }
    p = tempA;
    return true;
}

std::vector<Book>::iterator searchBooks()
{
    Data& data = Data::getInstance();
    while(true)
    {
        bool found = false;
        std::string title;
        std::cout << "Enter the title of the book to search: " << std::endl;
        while (!readString(std::cin, title, 's'));
        for (auto it = data.vBooks().begin(); it != data.vBooks().end(); ++it)
            if (it->name == title)
            {
                std::cout << "Found this book: " << std::endl << std::endl;
                it->print();
                std::cout << std::endl;
                found = true;
                sought = it;
                break;
            }
        if (!found)
            std::cout << "Sorry, there is no such book.";
        //if (data.vBooks().empty()) return sought;

        if (yesNo("Try again?")) continue;
        else break;
    }
    return sought;
}

void addRecord() //TODO: put exit everywhere
{
    Data& data = Data::getInstance();
    while(true)
    {
        Book book;
        std::cout << "Enter the title of the book" << std::endl;
        while (!readString(std::cin, book.name, 's'));
        std::cout << "Enter the author's name and surname" << std::endl;
        while (!readString(std::cin, book.author, 's'));
        std::cout << "Enter the ISBN of the book" << std::endl;
        while (!readString(std::cin, book.isbn, 'n'));
        std::cout << "Enter the date the book was issued" << std::endl;
        while (!readString(std::cin, book.date, 'd'));
        data.vBooks().push_back(book);
        std::cout << "Successfully added your book" << std::endl;

        if (!yesNo("Add another one?")) break;
    }
}

void manageRecord()
{
    Data& data = Data::getInstance();
    std::string temp;
    auto result = searchBooks();
    if (result == data.vBooks().end()) return;
    cls();
    std::cout << std::endl;
    result->printBook();

    if (data.vBooks().empty())
    {
        std::cout << "There are no books in the database. Add one first." << std::endl;
        sleep(WAIT_TIME_LONG);
        return;
    }
    while (true)
    {
        std::cout << "What to do with this record? "
                     "\n1 -> delete  "
                     "\n2 -> edit "
                     "\nq -> nothing " << std::endl;
        switch (getch())
        {
            case '1':
                if (yesNo("Delete this record?"))
                {
                    data.vBooks().erase(result);
                    return;
                }
                else return;
            case '2':
                while (true)
                {
                    std::cout << "What would you like to edit? "
                                 "\n1 -> Title "
                                 "\n2 -> Author "
                                 "\n3 -> ISBN "
                                 "\n4 -> Date issued "
                                 "\nq -> Nothing" << std::endl;
                    switch (getch())
                    {
                        case '1':
                            std::cout << "Enter the new title of the book: " << std::endl;
                            while (!readString(std::cin, temp, 's'));
                            result->name = temp;
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '2':
                            std::cout << "Enter the new author of the book: " << std::endl;
                            while (!readString(std::cin, temp, 's'));
                            result->author = temp;
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '3':
                            std::cout << "Enter the new ISBN of the book: " << std::endl;
                            while (!readString(std::cin, temp, 'n'));
                            result->isbn = temp;
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '4':
                            std::cout << "Enter the new issue date of the book: " << std::endl;
                            while (!readString(std::cin, temp, 'd'));
                            result->date = temp;
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case 'q':
                            return;
                        default :
                            std::cerr << "Wrong input. Try again." << std::endl;
                            break;
                    }
                }
            case 'q':
                return;
            default :
                break;
        }
    }
}
//TODO: Left Here. (From bottom to up)
void manageBooks(bool isadmin)
{
    Data& data = Data::getInstance();
    if (isadmin)
    {
        while (true)
        {
            cls();
            data.printbooks();
            std::cout << ":ADMIN:" << std::endl;
            std::cout << "Select an option: "
                      << "\n1 to add a new record "
                      << "\n2 to manage a record "
                      << "\n3 to search "
                      << "\nq to go back" << std::endl;
            switch (getch())
            {
                case 'q':
                    return;
                case '1':
                    addRecord();
                    break;
                case '2':
                    manageRecord();
                    break;
                case '3':
                    searchBooks();
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        while (true)
        {
            cls();
            data.printbooks();
            std::cout << ":USER:" << std::endl;
            std::cout << "Select an option: "
                      << "\n1 to search"
                      << "\nq to go back" << std::endl;
            switch (getch())
            {
                case '1':
                    searchBooks();
                    break;
                case 'q':
                    return;
                default:
                    break;
            }
        }
    }
}

bool passChange(const std::string& l, const bool& isadmin)
{
    Data& data = Data::getInstance();
    std::string p;
    if (!passConfirm(p)) return false;
    data.changePass(l, p, isadmin);
    std::cout << "Your password was changed successfully." << std::endl;
    sleep(WAIT_TIME_LONG);
    return true;
}

void manageUsr()
{
    Data& data = Data::getInstance();
    std::string l, p;
    while (true)
    {
        cls();
        data.printCredentials(false);
        std::cout << data.loginprompt << std::endl;
        while(true)
        {
            while (!readString(std::cin, l, 'n'));
            if (l == "exit") return;
            if(data.loginCheck(l, false)) break;
            else
                std::cout << "User not found." << std::endl;
        }
        data.delAccount(l,false);
        std::cout << "Deleted account " << l << std::endl;

        if (data.enumAccounts(false) == 0) return;
        if (yesNo("Delete another one?")) continue;
        else return;
    }
}

void createAccPrompt(bool isadmin)
{
    Data& data = Data::getInstance();
    std::string l, p, temp;
    cls();
#ifndef NDEBUG
    data.printCredentials(isadmin);
#endif
    std::cout << "New account: " << data.loginprompt << std::endl;
    while (!readString(std::cin, l, 'n'));
    if (l == "exit") return;

    if(data.loginCheck(l,isadmin))
    {
        std::cerr << "Such account already exists!" << std::endl;
        sleep(WAIT_TIME_LONG);
        return;
    }
    if (!passConfirm(p)) return;
    data.createAccount(l, p, isadmin);
    std::cout << "Successfully created account " << l << " ! Going back...";
#ifndef NDEBUG
    data.printCredentials(false);
#endif
    sleep(WAIT_TIME_LONG);
}

bool delDialog(const std::string& l, bool isadmin)
{
    Data& data = Data::getInstance();
    std::string p;
    while (true)
    {
        cls();
        std::cout << "THIS WILL DELETE YOUR ACCOUNT AND YOU WILL BE LOGGED OFF."
                  << " \nTYPE YOUR PASSWORD, " << l << ", TO PROCEED OR \"exit\" TO CANCEL." << std::endl;
        while (!readString(std::cin, p, 'p'));
        if (p == "exit") return false;
        if (data.passCheck(l, p, isadmin))
            break;
        else
            std::cerr << "Wrong password." << std::endl;
    }
    if (!data.delAccount(l, isadmin))
    {
        std::cerr << "You can't delete the last account!" << std::endl;
        return false;
    }
    std::cout << "Deleted account  " << l << std::endl;
    sleep(WAIT_TIME_NORMAL);
    return true; //Was deleted
}

void admConsole(std::string& adm, std::string& pass)
{
    while (true)
    {
        cls();
        std::cout << ":ADMIN:" << std::endl;
        std::cout << "Select an option: "
                  << "\n1 for managing book data "
                  << "\n2 for deleting users "
                  << "\n3 to register an administrator"
                  << "\n4 to change your password"
                  << "\n0 to delete your account (careful!)"
                  << "\nq to sign off" << std::endl;
        switch (getch())
        {
            case '1':
                manageBooks(true);
                break;
            case '2':
                manageUsr();
                break;
            case '3':
                createAccPrompt(true);
                break;
            case '4':
                passChange(adm, true);
                break;
            case '0':
                if (delDialog(adm, true)) return;
                else break;
            case 'q':
                return;
            default:
                continue;
        }
    }
}

void admLogin()
{
    Data& data = Data::getInstance();
    std::string adm, pass;

    while(true)
    {
        cls();
        std::cout << data.loginprompt << std::endl;
        while (!readString(std::cin, adm, 'n'));
        if (adm == "exit") return;
        if (data.loginCheck(adm, true))
            break;
        else
            std::cout << "User not found." << std::endl;
        sleep(WAIT_TIME_NORMAL);
    }

    while(true)
    {
        cls();
        std::cout << data.passprompt << std::endl;
        while (!readString(std::cin, pass, 'p'));
        if (pass == "exit") return;
        if (data.passCheck(adm, pass, true)) break;
        else
            std::cout << "Wrong password." << std::endl;
        sleep(WAIT_TIME_NORMAL);
    }

    std::cout << "Success. Redirecting..." << std::endl;
    sleep(WAIT_TIME_NORMAL);
    admConsole(adm, pass);
}



void usrConsole(const std::string& usr, const std::string& pass)
{
    while (true)
    {
        cls();
        std::cout << ":USER:"
                  << "\nSelect an option: "
                  << "\n1 for managing book data "
                  << "\n2 for changing your password"
                  << "\n0 to delete your account"
                  << "\nq to sign off" << std::endl;
        switch (getch())
        {
            case 'q':
                return;
            case '1':
                manageBooks(false);
                break;
            case '2':
                passChange(usr,false);
                break;
            case '0':
                if (delDialog(usr, false)) return; else break;
            default:
                break;
        }
    }
}

void usrLogin()
{
    Data& data = Data::getInstance();
    std::string usr, pass;

    while(true)
    {
        cls();
        std::cout << data.loginprompt << std::endl;
        while (!readString(std::cin, usr, 'n'));
        if (usr == "exit") return;
        if (data.loginCheck(usr, false))
            break;
        else
            std::cout << "User not found." << std::endl;
        sleep(WAIT_TIME_MID);
    }
    while (true)
    {
        cls();
        std::cout << data.passprompt << std::endl;
        while (!readString(std::cin, pass, 'p'));
        if (pass == "exit") return;
        if (data.passCheck(usr, pass, false))
            break;
        else
            std::cout << "Wrong password." <<std::endl;
        sleep(WAIT_TIME_MID);
    }
    std::cout << "Success. Redirecting..." << std::endl;
    sleep(WAIT_TIME_NORMAL);
    usrConsole(usr, pass);
}

int main(int argc, char* argv[]) try
{
//TODO: Implement autosaving.
    Data& data = Data::getInstance();
    data.uinit();
    data.adminit();
    data.bookinit();

    path = argv[0];
    path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder
#ifndef NDEBUG
    data.printbooks();
    std::cout << std::endl;
    std::cout << path << std::endl;
    data.printCredentials(false);
    std::cout << std::endl;
    data.printCredentials(true);
    std::cout << std::endl;
    getch();
#endif
    bool workin = true;
    while (workin)
    {
        cls();
        std::cout << "\nWelcome. Enter \n 1 for user sign in \n 2 for admin sign in \n q to exit" << std::endl;
        switch (tolower(getch()))
        {
            case 'q':
                workin = false;
                break;
            case '1':
                if (yesNo("Have you got an account?")) usrLogin();
                else createAccPrompt(false);
                break;
            case '2':
                admLogin();
                break;
            default:
                break;
        }
    }
    data.save();
    return 0;
}
catch (std::invalid_argument& msg)
{
    std::cerr << "Error: " << msg.what();
    getch();
    return (-3);
}
catch (std::runtime_error& msg)
{
    std::cerr << msg.what() << std::endl;
    getch();
    return (-2);
}
catch (...)
{
    std::cerr << "Undefined Error";
    getch();
    return (-1);
}
