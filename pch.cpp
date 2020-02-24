#include "pch.h"
std::string path; //extern global
bool isExiting;
//I use goto ONLY if i can't break from cycle-switch pair;

//TODO: Implement sort, implement filtering (date)?
//TODO: Implement date parsing

std::vector<Book>::iterator searchBooks()
{
    Data& data = Data::getInstance();
    auto sought = data.vBooks().end();
    Begin:
    bool found = false;
    std::string title;
    std::cout << "Enter the title of the book to search: " << std::endl;
    while (!readString(std::cin, title, 's'));
    for (auto it = data.vBooks().begin(); it != data.vBooks().end(); ++it)
        if (it->name == title)
        {
            std::cout << "Found this book: " << std::endl << std::endl;
            it->printBook();
            std::cout << std::endl;
            found = true;
            sought = it;
            break;
        }
    if (!found)
        std::cout << "Sorry, there is no such book.";
    while (!data.vBooks().empty())
    {
        std::cout << "Try again? y/n" << std::endl;
        switch (getch())
        {
            case 'y':
                goto Begin;
            case 'n':
                return sought;
            default :
                std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }
    }
    return sought;
}

void addRecord() //TODO: put exit everywhere
{
    Data& data = Data::getInstance();
    if (data.vBooks().empty())
    {
        std::cout << "There are no books in the database. Add one first." << std::endl;
        sleep(2);
        return;
    }
    Begin:
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

    while (true)
    {
        std::cout << "Add another one? y/n" << std::endl;
        switch (getch())
        {
            case 'y':
                goto Begin;
            case 'n':
                return;
            default :
                std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }
    }
}

void manageRecord()
{

    Data& data = Data::getInstance();
    std::string temp;
    auto result = searchBooks();
    if (result == data.vBooks().end()) return;
    std::cout << std::endl;
    result->printBook();
    if (data.vBooks().empty())
    {
        std::cout << "There are no books in the database. Add one first." << std::endl;
        sleep(2);
        return;
    }
    while (true)
    {
        std::cout << "What to do with this record? \n1 -> delete  \n2 -> edit \nq -> nothing " << std::endl;
        switch (getch())
        {
            case '1':
                while (true)
                {
                    std::cout << "Delete this record? y/n" << std::endl;
                    switch (getch())
                    {
                        case 'y':
                            data.vBooks().erase(result);
                            return;
                        case 'n':
                            return;
                        default :
                            std::cerr << "Wrong input. Try again." << std::endl;
                            break;
                    }
                }
            case '2':
                while (true)
                {
                    std::cout << "What would you like to edit? "
                                 "\n1 -> Title \n2 -> Author \n3 -> ISBN \n4 -> Date issued \nq -> Nothing"
                              << std::endl;
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
                std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }

    }
}

void manageBooks(bool isadmin)
{
    Data& data = Data::getInstance();
    if (isadmin)
    {
        while (true)
        {
            system("cls");
            data.printbooks();
            std::cout << ":ADMIN:" << std::endl;
            std::cout << "Select an option: "
                      << "\n1 to add a new record "
                      << "\n2 to manage a record "
                      << "\n3 to search"
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
                    std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;
            }
        }
    }
    else
    {
        system("cls");
        data.printbooks();
        while (true)
        {
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
                    std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;
            }
        }
    }
}

void manageUsr()
{
    Data& data = Data::getInstance();
    std::string l, p;
    data.printCredentials('u');
    mngBegin: //Yes, I'm nuts
    std::cout << "Type which account you want to delete or \"exit\" to exit" << std::endl;
    do
    {
        while (!readString(std::cin, l, 'n'));
        if (l == "exit") return;
    } while (!data.loginCheck(l, false));
    data.muser().erase(data.muser().find(l));
    std::cout << "Deleted account " << l << std::endl;

    while (!data.muser().empty())
    {
        std::cout << "Delete another one? y/n" << std::endl;
        switch (getch())
        {
            case 'y':
                goto mngBegin;
            case 'n':
                return;
            default :
                std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }
    }
}

bool manageAdmin(const std::string& adm, const std::string& pass, bool wantToAdd)
{
    Data& data = Data::getInstance();
    std::string l, p;
    data.printCredentials('a');
    if (wantToAdd)
    {
        std::cout << "Enter the username of a new admin." << std::endl;
        while (!readString(std::cin, l, 'n'));
        if (data.madm().find(l) != data.madm().end())
        {
            std::cerr << "Such admin already exists!" << std::endl;
            sleep(2);
            return false;
        }
        std::cout << "Enter the password for " << l << " ." << std::endl;
        while (!readString(std::cin, p, 'n'));
        if (!(data.madm().emplace(std::make_pair(l, hash(p))).second))
            std::cerr << "Element already exists" << std::endl;
        std::cout << "Successfully created admin " << l << " ! Going back...";
        sleep(2);
        return false;
    }
    else if (data.madm().size() > 1)
    {
        std::cerr << "THIS WILL DELETE YOUR ACCOUNT AND YOU WILL BE LOGGED OFF."
                  << " \nTYPE YOUR PASSWORD, " << adm << ", TO PROCEED OR \"exit\" TO CANCEL." << std::endl;
        do
        {
            while (!readString(std::cin, p, 'n'));
            if (p == "exit") return false;
        } while (!data.passCheck(adm, p, true));
        std::cout << "Deleting... " << std::endl;
        data.madm().erase(data.madm().find(adm));
        sleep(1);
        return true; //Was deleted
    }
    else
    {
        std::cerr << "You can't delete the last account!";
        sleep(2);
        return false;
    }
}

void admConsole(std::string& adm, std::string& pass)
{
    while (true)
    {
        system("cls");
        std::cout << ":ADMIN:" << std::endl;
        std::cout << "Select an option: "
                  << "\n1 for managing book data "
                  << "\n2 for managing user data "
                  << "\n3 to register an administrator"
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
                manageAdmin(adm, pass, true);
                break;
            case '0':
                if (manageAdmin(adm, pass, false)) return;
                else break;
            case 'q':
                return;
            default:
                std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                sleep(1);
                break;
        }
    }
}

void admLogin()
{
    Data& data = Data::getInstance();
    std::string adm, pass;
    system("cls");
    do
    {
        std::cout << ":ADMIN: \n Enter login or \"exit\" to exit:" << std::endl;
        while (!readString(std::cin, adm, 'n'));
        if (adm == "exit") return;
    } while (!data.loginCheck(adm, true));

    do
    {
        std::cout << "Enter your password or \"exit\" to exit:" << std::endl;
        while (!readString(std::cin, pass, 'n'));
        if (pass == "exit") return;
    } while (!data.passCheck(adm, pass, true));

    std::cout << "Success. Redirecting..." << std::endl;
    sleep(2);
    admConsole(adm, pass);
}

void usrConsole(const std::string& usr, const std::string& pass)
{
    system("cls");
    std::cout << ":USER:"
              << "\nSelect an option: "
              << "\n1 for managing book data "
              // << "\n0 to delete your account (careful!)" TODO: Implement
              << "\nq to sign off" << std::endl;
    while (true)
    {
        switch (getch())
        {
            case 'q':
                return;
            case '1':
                manageBooks(false);
                break;
                //case '0':
            default:
                std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                break;
        }
    }
}

void usrRegister()
{
    Data& data = Data::getInstance();
    std::string l, p;
    std::cout << "Enter your username or \"exit\" to exit" << std::endl;
    while (!readString(std::cin, l, 'n'));
    if (l == "exit") return;
    std::cout << "Enter the password for " << l << " or \"exit\" to exit" << std::endl;
    while (!readString(std::cin, p, 'n'));
    if (p == "exit") return;
    data.muser()[l] = hash(p);
    std::cout << "Successfully created user " << l << " ! Going back...";
    data.printCredentials('u');
    sleep(2);
}

void usrLogin()
{
    Data& data = Data::getInstance();
    std::string usr, pass;
    system("cls");
    do
    {
        std::cout << ":USER: \n Enter login or \"exit\" to exit:" << std::endl;
        while (!readString(std::cin, usr, 'n'));
        if (usr == "exit") return;
    } while (!data.loginCheck(usr, false));

    do
    {
        std::cout << "Enter your password or \"exit\" to exit:" << std::endl;
        while (!readString(std::cin, pass, 'n'));
        if (pass == "exit") return;
    } while (!data.passCheck(usr, pass, false));

    std::cout << "Success. Redirecting..." << std::endl;
    sleep(2);
    usrConsole(usr, pass);
}

void usrPrompt()
{
    do
    {
        std::cout << "Have you got an account? y/n" << std::endl;
        switch (getch())
        {
            case 'y':
                usrLogin();
                return;
            case 'n':
                usrRegister();
                return;
            default :
                std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }
    } while (true);
}


int main(int argc, char* argv[])
{
    //TODO: Implement autosaving.
    isExiting = false;
    Data& data = Data::getInstance();
    try
    {
        path = argv[0];
        path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder
        std::thread bgThread(bgProc);
        sleep(1);
#ifdef DEBUF
        system("cls");
#endif
        while (true)
        {
            //system("cls");
            std::cout << "\nWelcome. Enter \n 1 for user sign in \n 2 for admin sign in \n q to exit" << std::endl;
            switch (getch())
            {
                case 'q':
                    goto Out;
                case '1':
                    usrPrompt();
                    break;
                case '2':
                    admLogin();
                    break;
                default:
                    std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;
            }
        }
        Out:
        isExiting = true;
        bgThread.join();
        data.save();
        return 0;

    }
    catch (std::invalid_argument& msg)
    {
        std::cerr << "Error: " << msg.what();
        system("pause");
        return (-2);
    }
    catch (...)
    {
        std::cerr << "Undefined Error";
        system("pause");
        return (-1);
    }
}
