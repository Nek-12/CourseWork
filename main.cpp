#include "header.h"
#include <iostream>
#include <conio.h>
std::string path; //extern global
inline void cls()
{ system("cls"); }

//TODO: Implement sort, implement filtering (date)?
//TODO: Make the book table be displayed only by request
//TODO: Implement case insensitivity
//TODO: Move the stuff to different folders/files;
//TODO: Recreate the readString function to add exit functionality edit usages;
//TODO: Add nullptrs returns for every function and checks
//TODO: Make the "Select the option" a function template for convenient use;
bool yesNo(const std::string& msg)
{
    std::cout << msg << " y/n" << std::endl;
    while (true)
    {
        switch (tolower(getch()))
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
    return std::stoul(id);
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

void showGenreStats()
{
    std::cout << "Not implemented" << std::endl;
    sleep(WAIT_TIME_MID);
}

std::vector<Author*> searchAuthors()
{
return std::vector<Author*>();
}

std::vector<Book*> searchBook()
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        while (true)
        {
            std::string s;
            std::cout << "Enter any property of the book to search: " << std::endl;
            while (!readString(std::cin, s, 's'));
            return data.searchBook(s);
        }
    }
}

std::vector<Genre*> searchGenre()
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        while (true)
        {
            std::string s;
            std::cout << "Enter any property of the genre to search: " << std::endl;
            while (!readString(std::cin, s, 's'));
            return data.searchGenre(s);
        }
    }
}

Book* searchBookID()
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        while (true)
        {
            std::string s;
            std::cout << "Enter the ID of the book to search: " << std::endl;
            while (!readString(std::cin, s, 'i'));
            return data.searchBook(std::stoul(s));
        }
    }
}

Book* selectBook()
{
    std::vector<Book*> sought;
    while(true)
    {
        sought = searchBook();
        if (sought.empty())
        {
            std::cerr << "Nothing found." << std::endl;
            if (yesNo("Try again?")) continue;
            else return nullptr;
        }
        else
        {
            std::cout << "Found: " << std::endl;
            if (yesNo("Try again?")) continue;
            else break;
        }
    }
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << it+1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the book: " << std::endl;
    return sought[select() - 1];
}

Genre* selectGenre()
{
    std::vector<Genre*> sought;
    while(true)
    {
        sought = searchGenre();
        if (sought.empty())
        {
            std::cerr << "Nothing found." << std::endl;
            if (yesNo("Try again?")) continue;
            else return nullptr;
        }
        else
        {
            std::cout << "Found: " << std::endl;
            if (yesNo("Try again?")) continue;
            else break;
        }
    }
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << it+1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the genre: " << std::endl;
    return sought[select() - 1];
}

void searchUI()
{
    cls();
    std::cout << "What would you like to search? "
                 "\n1 -> Book ID"
                 "\n2 -> Book title"
                 "\n4 -> Author"
                 "\nq -> Nothing" << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                searchBookID();
                return;
            case '2':
                searchBook();
                return;
            case '3':
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


Author* selectAuthor()
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
//    Data& data = Data::getInstance();
//    while (true)
//    {
//        //cls(); TODO: ??
//        std::string n, a, y;
//        ull id = inputID();
//        std::cout << "Enter the title of the book" << std::endl;
//        while (!readString(std::cin, n, 's'));
//        std::cout << "Enter the year the book was published" << std::endl;
//        while (!readString(std::cin, y, 'y'));
//        data.add(Book(id,n,std::stoul(y)));
//        std::vector<Author*> vecpa;
//        while (true)
//        {
//            Author* pa = selectAuthor();
//            if (pa == nullptr) return nullptr;
//            vecpa.push_back(pa);
//            if (!yesNo("Add another author to book " + n + " ?")) break;
//        }
//        auto data.
//        for (auto el: vecpa)
//            bres.addAuthor(*el);
//        std::cout << "Successfully added your book" << std::endl;
//        if (!yesNo("Add another one?")) return
//    }
std::cout << "Not implemented" << std::endl;
sleep(WAIT_TIME_NORMAL);
return nullptr;
}

void editBookAuthor(Book* b)
{
//    std::string temp;
//    Author* pa = selectAuthor();
//    if (pa == nullptr) return;
//    b->addAuthor(*pa);
//    std::cout << "Changed successfully." << std::endl;
//    sleep(WAIT_TIME_MID);
std::cout << "Not implemented" << std::endl;
sleep(WAIT_TIME_NORMAL);
}

void manageAuthor()
{
//    Data& data = Data::getInstance();
//    std::string temp;
//    auto sought = selectAuthor();
    std::cout << "Not implemented" << std::endl;
    sleep(WAIT_TIME_NORMAL);
}

void editBookGenre(Book* pBook)
{
    std::cout << "Not implemented" << std::endl;
    sleep(WAIT_TIME_NORMAL);
}
void manageBook()
{
    Data& data = Data::getInstance();
    std::string temp;
    Book* pbook = selectBook();
    if (!pbook) return;
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
                    std::cout << "Erased this book and removed all references." << std::endl;
                    data.erase(*pbook); //TODO: The pointer is invalidated isn't it
                    sleep(WAIT_TIME_NORMAL);
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
                                 "\n2 -> Publishing year"
                                 "\n3 -> Authors"
                                 "\n4 -> Genres"
                                 "\nq -> Nothing" << std::endl;
                    switch (getch())
                    {
                        case '1':
                            std::cout << "Enter the new title of the book: " << std::endl;
                            while (!readString(std::cin, temp, 's'));
                            pbook->rename(temp);
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '2':
                            std::cout << "Enter the new publishing year of the book: " << std::endl;
                            while (!readString(std::cin, temp, 'y'));
                            pbook->setYear(std::stoul(temp));
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '3':
                            editBookAuthor(pbook);
                            return;
                        case '4':
                            editBookGenre(pbook);
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
void editGenres()
{
    std::string temp;
    unsigned cnt = 0;
    Genre* pgenre = selectGenre();
    Book* pbook = nullptr;
    std::cout << *pgenre << std::endl;
    std::cout << "Select an option: "
              << "\n1 -> rename this genre "
              << "\n2 -> remove this genre from books"
              << "\n3 -> add this genre to books " //TODO: Implement removing from authors
              << "\nq -> go back" << std::endl;
    auto it = pgenre->getBooks().begin();
    switch(getch())
    {
        case '1':
            std::cout << "Enter the new genre's title" << std::endl;
            while (!readString(std::cin,temp,'s'));
            pgenre->rename(temp);
            std::cout << "Renamed to " << temp << std::endl;
            return;
        case '2':
            for (auto& b: pgenre->getBooks()) //TODO: Refactor this to work with vectors, maps with *s are bs
            {
                ++cnt;
                std::cout << "#" << cnt << ":\n" << *b.second << std::endl;
            }
            std::cout << "Select which book to remove: " << std::endl;
            std::advance(it, select()-1);
            pgenre->remBook(*it->second); //TODO: Test
            std::cout << "Removed successfully " << std::endl;
            sleep(WAIT_TIME_NORMAL);
            return;
        case '3':
            pbook = selectBook();
            if (!yesNo("Add genre " + pgenre->getName() + " to the book " + pbook->getName() + " ?")) return;
            else
                pgenre->addBook(*pbook);
            break;
        case 'q':
            return;
        default:
            break;
    }
}

void newGenre()
{
    Data& data = Data::getInstance();
    std::string temp;
    std::cout << "Enter the new genre's name" << std::endl;
    while (!readString(std::cin, temp, 's'));
    ull id = inputID();
    Genre* added = data.add(Genre(id, temp));
    if (!added)
    {
        std::cerr << "Such genre already exists." << std::endl;
        sleep(WAIT_TIME_NORMAL);
        return;
    }
    if (!yesNo("Do you want to add this genre to some books?")) return;
    while (true)
    {
        Book* pbook = selectBook();
        if (!yesNo("Add this genre to the book?")) return;
        else pbook->addGenre(*added);
        if (!yesNo("Add one more?")) return;
        else continue;
    }
}

void manageGenre()
{
    while(true)
    {
        cls();
        std::cout << "Select an option: "
                  << "\n1 -> add a new genre "
                  << "\n2 -> edit a genre "
                  << "\nq -> go back" << std::endl;
        switch (getch())
        {
            case 'q':
                return;
            case '1':
                newGenre();
                break;
            case '2':
                editGenres();
                break;
            default:
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
                      << "\n1 -> add a new book "
                      << "\n2 -> manage a book "
                      << "\n3 -> manage an author "
                      << "\n4 -> manage a genre "
                      << "\n5 -> search books "
                      << "\nq -> go back" << std::endl;
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
                    manageAuthor();
                    break;
                case '4':
                    manageGenre();
                    break;
                case '5':
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
                      << "\n1 -> Search"
                      << "\n2 -> Show genre stats "
                      << "\nq -> Go back" << std::endl;
            switch (getch())
            {
                case '1':
                    searchUI();
                    break;
                case '2':
                    showGenreStats();
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

int main(int, char* argv[]) try
{
//TODO: Implement autosaving.
    Data& data = Data::getInstance();
    data.load();
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
