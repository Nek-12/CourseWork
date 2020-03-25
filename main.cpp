#include "header.h"
std::string path; //extern global


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
                //std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }
    }
}

std::vector<Book>::iterator searchBooks()
{
    Data& data = Data::getInstance();
    auto sought = data.vBooks().end();
    while(true)
    {
        bool found = false;
        std::string title;
        std::cout << "Enter the title of the book to search: " << std::endl;
        while (!readString(std::cin, title, 's'));
        for (auto it = data.vBooks().begin(); it != data.vBooks().end(); ++it)
            if (it->title == title)
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
        while (!readString(std::cin, book.title, 's'));
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
    system("cls");
    std::cout << std::endl;
    result->printBook();

    if (data.vBooks().empty())
    {
        std::cout << "There are no books in the database. Add one first." << std::endl;
        sleep(2000);
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
                            result->title = temp;
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
                //std::cerr << "Wrong input. Try again." << std::endl;
                break;
        }
    }
}

void manageBooks(bool isadmin)
{
    Data& data = Data::getInstance();
    system("cls");
    data.printbooks();
    std::cout << ":ADMIN:" << std::endl;
    std::cout << "Select an option: "
              << "\n1 to add a new record "
              << "\n2 to manage a record "
              << "\n3 to search "
              << "\nq to go back" << std::endl;
    if (isadmin)
    {
        while (true)
        {
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
                    //std::cout << std::endl << "Invalid input. Try again:" << std::endl;
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
                    //std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;
            }
        }
    }
}

bool passChange(const std::string& l, const bool& isAdmin)
{
    Data& data = Data::getInstance();
    while(true) //TODO: Make this a function (code duplication)
    {
        std::string tempA, tempB;
        std::cout << "Enter your new password or \"exit\" to exit: " << std::endl;
        while (!readString(std::cin, tempA, 'p'));
        if (tempA == "exit") return false;
        std::cout << "Confirm the password or \"exit\" to exit: " << std::endl;
        while (!readString(std::cin, tempB, 'p'));
        if (tempB == "exit") return false;
        if(tempA != tempB)
        {
            std::cerr << "Your passwords don't match." << std::endl;
            continue;
        }
        (isAdmin ? data.madm() : data.muser() )[l] = hash(tempA);
        std::cout << "Your password was changed successfully." << std::endl;
        sleep(2000);
        return true;
    }
}

void manageUsr()
{
    Data& data = Data::getInstance();
    std::string l, p;
    data.printCredentials(false);
    while (true)
    {
        std::cout << "Type which account you want to delete or \"exit\" to exit" << std::endl;
        do
        {
            while (!readString(std::cin, l, 'n'));
            if (l == "exit") return;
        } while (!data.loginCheck(l, false));
        data.muser().erase(data.muser().find(l));
        std::cout << "Deleted account " << l << std::endl;

        if (data.muser().empty()) return;
        if (yesNo("Delete another one?")) continue;
        else return;
    }
}

void createAcc(bool isAdmin)
{
    Data& data = Data::getInstance();
    std::string l, p, temp;
    data.printCredentials(isAdmin);

    std::cout << "Enter the username for the new account or \"exit\" to exit" << std::endl;
    while (!readString(std::cin, l, 'n'));
    if (l == "exit") return;

    if (isAdmin ? data.madm().find(l) != data.madm().end()
                : data.muser().find(l) != data.muser().end())
    {
        std::cerr << "Such account already exists!" << std::endl;
        sleep(2000);
        return;
    }
    while(true)
    {
        std::cout << "Enter the password for " << l << " or \"exit\" to exit: " << std::endl;
        while (!readString(std::cin, p, 'p'));
        if (p == "exit") return;

        std::cout << "Confirm the password or enter \"exit\" to exit: " << std::endl;
        while (!readString(std::cin, temp, 'p'));
        if (temp == "exit") return;

        if (temp != p)
        {
            std::cerr << "Your passwords don't match." << std::endl;
            continue;
        }
        break;
    }
    (isAdmin ? data.madm() : data.muser())[l] = hash(p);
    std::cout << "Successfully created account " << l << " ! Going back...";
    data.printCredentials(false);
    sleep(2000);
}

bool deleteAcc(const std::string& l, bool isAdmin)
{
    Data& data = Data::getInstance();
    std::string p;
    if (isAdmin && data.madm().size() < 2)
    {
        std::cerr << "You can't delete the last account!";
        sleep(2000);
        return false;
    }
    std::cout << "THIS WILL DELETE YOUR ACCOUNT AND YOU WILL BE LOGGED OFF."
              << " \nTYPE YOUR PASSWORD, " << l << ", TO PROCEED OR \"exit\" TO CANCEL." << std::endl;
    do
    {
        while (!readString(std::cin, p, 'p'));
        if (p == "exit") return false;
    } while (!data.passCheck(l, p, isAdmin));

    std::cout << "Deleting... " << std::endl;
    isAdmin? data.madm().erase(data.madm().find(l))
            :data.muser().erase(data.muser().find(l));
    sleep(1000);
    return true; //Was deleted
}

void admConsole(std::string& adm, std::string& pass)
{
    while (true)
    {
        system("cls");
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
                createAcc(true);
                break;
            case '4':
                passChange(adm, true); //'u' for user
                break;
            case '0':
                if (deleteAcc(adm, true)) return;
                else break;
            case 'q':
                return;
            default:
                //std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                continue; //TODO: TEST
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
        while (!readString(std::cin, pass, 'p'));
        if (pass == "exit") return;
    } while (!data.passCheck(adm, pass, true));

    std::cout << "Success. Redirecting..." << std::endl;
    sleep(1000);
    admConsole(adm, pass);
}



void usrConsole(const std::string& usr, const std::string& pass)
{
    while (true)
    {
        system("cls");
        std::cout << ":USER:"
                  << "\nSelect an option: "
                  << "\n1 for managing book data "
                  << "\n2 for changing your password" //TODO:Implement
                  << "\n0 to delete your account (careful!)" //TODO: Implement
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
                if (deleteAcc(usr,false)) return; else break;
            default:
                //std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                break;
        }
    }
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
        while (!readString(std::cin, pass, 'p'));
        if (pass == "exit") return;
    } while (!data.passCheck(usr, pass, false));

    std::cout << "Success. Redirecting..." << std::endl;
    sleep(1000);
    usrConsole(usr, pass);
}


int main(int argc, char* argv[])
{
    //TODO: Implement autosaving.
    Data& data = Data::getInstance();
    data.adminit();
    data.uinit();
    data.bookinit() ;
    try
    {
        path = argv[0];
        path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder
#ifdef DEBUG
        data.printbooks();
        std::cout << std::endl;
        std::cout << path << std::endl;
        data.printCredentials(false);
        std::cout << std::endl;
        data.printCredentials(true);
        std::cout << std::endl;
        getch();
#endif

    while (true)
        {
            system("cls");
            std::cout << "\nWelcome. Enter \n 1 for user sign in \n 2 for admin sign in \n q to exit" << std::endl;
            switch (getch())
            {
                case 'q':
                    goto Out;
                case '1':
                    if (yesNo("Have you got an account?")) usrLogin(); else createAcc(false);
                    break;
                case '2':
                    admLogin();
                    break;
                default:
                    //std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;
            }
        }
        Out:
        data.save();
        return 0;
    }
    catch (std::invalid_argument& msg)
    {
        std::cerr << "Error: " << msg.what();
        system("pause");
        return (-3);
    }
    catch (std::runtime_error &msg)
    {
        std::cerr << msg.what() << std::endl;
        getch();
        return (-2);
    }
    catch (...)
    {
        std::cerr << "Undefined Error";
        system("pause");
        return (-1);
    }
}
