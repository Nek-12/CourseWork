#include "header.h"
#include <iostream>
#include <conio.h>


std::string path; //extern global

inline void cls()
{ system("cls"); }

//TODO: Implement sort, implement filtering (date)?
//TODO: Make the book table be displayed only by request
//TODO: Implement case insensitivity

bool yesNo(const std::string& msg)
{
    std::cout << msg << " y/n" << std::endl;
    while (true)
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

ull inputID()
{
    std::string id;
    if (!yesNo("Generate an ID?"))
    {
        while (true)
        {
            std::cout << "Enter the ID: ";
            readString(std::cin, id, 'i');
            if (id.size() == 9) break;
        }
    }
    else
        return genID();
    return std::stoull(id);
}

unsigned select()
{
    std::string s;
    while (!readString(std::cin, s, 'i'));
    return stoul(s);
}

bool passConfirm(std::string& p)
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

std::vector<Author*> searchAuthors()
{
return std::vector<Author*>();
}

std::vector<Book*> searchBooks()
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        std::string s;
        std::cout << "Enter the title of the book to search: " << std::endl;
        while (!readString(std::cin, s, 's'));
        if (s.size() < 3 ) continue;
        auto sought = data.searchBook(s);
        if (sought.empty())
            if (!yesNo("Nothing was found. Try again?")) return sought;
        std::cout << "Found: " << std::endl;
        for (auto el: sought)
            std::cout << *el;
        if (!yesNo("Try again?")) return sought;
    }
}

void searchUI()
{
    cls();
    std::cout << "What would you like to search? "
                 "\n1 -> Book"
                 "\n2 -> Author"
                 "\nq -> Nothing" << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                searchBooks();
                return;
            case '2':
                searchAuthors();
                return;
            case 'q':
                return;
            default :
                break;
        }
    }
}

Author* newAuthor()
{
    return nullptr;
//TODO: First you add a book/author, then you fill it with data for pointer safety.
// Consider what happens if you try to add an author for a nonexistent book
}


Author* selectAuthorFromSearch()
{
    std::vector<Author*> sought = searchAuthors();
    Author* pa = nullptr;
    if (sought.empty())
    {
        if (!yesNo("Nothing found. Return?")) return pa; //TODO: Implement this recursion.
        pa = newAuthor();
        return pa;
    }
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << it + 1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the author you wish to add" << std::endl;
    pa = sought[select() - 1];
    return pa;
}


Book* newBook() //TODO: put exit everywhere
{
    Data& data = Data::getInstance();
    while (true)
    {
        //cls(); TODO: ??
        std::string n, a, y;
        ull id = 0;
        std::cout << "Enter the title of the book" << std::endl;
        while (!readString(std::cin, n, 's'));
        std::vector<Author*> vecpa;
        while (true)
        {
            Author* pa = selectAuthorFromSearch();
            if (pa == nullptr) return nullptr;
            vecpa.push_back(pa);
            if (!yesNo("Add another author to book " + n + " ?")) break;
        }
        id = inputID();
        std::cout << "Enter the year the book was issued" << std::endl;
        while (!readString(std::cin, y, 'y'));
        auto& bres = data.sb.emplace(std::make_pair(id,Book(id, n, std::stoul(y))))->second;
        for (auto el: vecpa)
            bres.addAuthor(*el);
        std::cout << "Successfully added your book" << std::endl;
        if (!yesNo("Add another one?")) return &bres;
    }
}

void editBookAuthor(Book* b)
{
    std::string temp;
    Author* pa = selectAuthorFromSearch();
    if (pa == nullptr) return;
    b->addAuthor(*pa);
    std::cout << "Changed successfully." << std::endl;
    sleep(WAIT_TIME_MID);
}

void manageBook()
{
    Data& data = Data::getInstance();
    std::string temp;
    auto sought = searchBooks();
    if (sought.empty()) return;
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << std::distance(it, sought.begin())+1 << ":\n" << *it;
    std::cout << "Select the book you wish to edit" << std::endl;
    Book* pbook = sought[select() - 1];
    while (true)
    {
        cls();
        std::cout << *pbook;
        std::cout << "What to do with this record? "
                     "\n1 -> delete  "
                     "\n2 -> edit "
                     "\nq -> nothing " << std::endl;
        switch (getch())
        {
            case '1':
                if (yesNo("Delete this record?"))
                {
                    std::cout << "I did nothing" << std::endl;
                    data.sb.erase(pbook->id);
                    return;
                }
                else return;
            case '2':
                while (true)
                {
                    cls();
                    std::cout << *pbook;
                    std::cout << "What would you like to edit? "
                                 "\n1 -> Title"
                                 "\n2 -> Authors"
                                 "\n3 -> Year"
                                 "\nq -> Nothing" << std::endl;
                    switch (getch())
                    {
                        case '1':
                            std::cout << "Enter the new title of the book: " << std::endl;
                            while (!readString(std::cin, temp, 's'));
                            pbook->name = temp;
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '2':
                            editBookAuthor(pbook);
                            return;
                        case '3':
                            std::cout << "Enter the new publishment year of the book: " << std::endl;
                            while (!readString(std::cin, temp, 'y'));
                            pbook->year = std::stoul(temp);
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case 'q':
                            return;
                        default :
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
void management(bool isadmin)
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
                      << "\n1 to add a new book "
                      << "\n2 to manage a book " //TODO: Implement changing genres, authors
                      << "\n3 to search "
                      << "\nq to go back" << std::endl;
            switch (getch())
            {
                case 'q':
                    return;
                case '1':
                    newBook();
                    break;
                case '2':
                    manageBook();
                    break;
                case '3':
                    searchUI();
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
                    searchUI();
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
        while (!readString(std::cin, l, 'n'));
        if (l == "exit") return;
        if (!data.loginCheck(l, false))
            std::cout << "User not found." << std::endl;
        else
        {
            data.delAccount(l, false);
            std::cout << "Deleted account " << l << std::endl;
            if (data.enumAccounts(false) == 0)
            {
                std::cout << "No accounts left. " << std::endl;
                sleep(WAIT_TIME_NORMAL);
                return;
            }
        }
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
    std::cout << data.loginprompt << std::endl;
    while (!readString(std::cin, l, 'n'));
    if (l == "exit") return;
    if (!data.loginCheck(l, isadmin)) //TODO: Why is it behaving strangely
    {
        std::cerr << "Such account already exists!" << std::endl;
        sleep(WAIT_TIME_LONG);
        return;
    }
    if (!passConfirm(p)) return;
    data.createAccount(l, p, isadmin);
    std::cout << "Successfully created account " << l << " ! Going back..." << std::endl;
#ifndef NDEBUG
    data.printCredentials(isadmin);
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
        sleep(WAIT_TIME_NORMAL);
        return false;
    }
    std::cout << "Deleted account  " << l << std::endl;
    sleep(WAIT_TIME_NORMAL);
    return true; //Was deleted
}

void admConsole(std::string& adm)
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
                management(true);
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

    while (true)
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

    while (true)
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
    admConsole(adm);
}


void usrConsole(const std::string& usr)
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
                management(false);
                break;
            case '2':
                passChange(usr, false);
                break;
            case '0':
                if (delDialog(usr, false)) return;
                else break;
            default:
                break;
        }
    }
}

void usrLogin()
{
    Data& data = Data::getInstance();
    std::string usr, pass;

    while (true)
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
            std::cout << "Wrong password." << std::endl;
        sleep(WAIT_TIME_MID);
    }
    std::cout << "Success. Redirecting..." << std::endl;
    sleep(WAIT_TIME_NORMAL);
    usrConsole(usr);
}

int main(int argc, char* argv[]) try
{
//TODO: Implement autosaving.
    Data& data = Data::getInstance();
    data.uinit();
    data.adminit();
    data.genreinit();
    data.authorinit();
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
    system("pause");
#endif
    bool workin = true;
    while (workin)
    {
        cls();
        std::cout << "\nWelcome. \n 1 -> user sign in \n 2 -> admin sign in \n 3 -> search (debug) \n q -> exit" << std::endl;
        switch (getchar())
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
            case '3':
                searchBooks();
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
