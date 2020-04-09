#include "header.h"
#ifdef WINDOWS
    #include <conio.h>
#else
# include <termios.h>
/* get a single char from stdin    */
int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(0, &oldattr);
    newattr=oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( 0, TCSANOW, &newattr);
    ch=getchar();
    tcsetattr(0, TCSANOW, &oldattr);
    return(ch);
}
#endif
std::string path; //extern global
inline void cls() { system("cls"); }

//TODO: Make the book table be displayed only by request
//TODO: Implement case insensitivity
//TODO: Move the stuff to different folders/files;
//TODO: Recreate the readString function to add exit functionality, edit usages;
//TODO: Add nullptrs returns for every function and checks
//TODO: Make the "Select the option" a function template for convenient use;
//TODO: Make books be printed by request only
//TODO: Make menus display -> instead of "for"
//TODO: Remove authors from genres and genres from authors
//TODO: Implement check for duplicate account and a big warning
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
        std::cout << "Enter the ID: ";
        while(!readString(std::cin, id, 'i'));
    }
    else
        return genID();
    return std::stoul(id);
}

unsigned select(const unsigned& limit)
{
    while(true)
    {
        std::string s;
        while (!readString(std::cin, s, 'i'));
        unsigned  ret = std::stoul(s);
        if (ret > limit || ret == 0)
        {
            std::cerr << "You have selected bad value. Try again: " << std::endl;
            continue;
        }
        else return ret;
    }
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
    Data& data = Data::getInstance();
    std::string y;
    while (true)
    {
        cls();
        std::cout << "Enter the time period in years: " << std::endl;
        while(!readString(std::cin,y,'y'));
        if (std::stoul(y) > getCurYear())
        {
            std::cerr << "The period you entered is invalid." << std::endl;
            continue;
        }
        data.printGenres(std::stoul(y));
        if (!yesNo("Try again?")) return;
    }
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
            std::cout << "Enter any property of the book to search (case ignored): " << std::endl;
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
            std::cout << "Enter any property of the genre to search (case ignored): " << std::endl;
            while (!readString(std::cin, s, 's'));
            return data.searchGenre(s);
        }
    }
}

std::vector<Author*> searchAuthor()
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        while (true)
        {
            std::string s;
            std::cout << "Enter any property of the author to search (case ignored): " << std::endl;
            while (!readString(std::cin, s, 's'));
            return data.searchAuthor(s);
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
            for (auto el: sought)
                std::cout << *el << std::endl;
            if (yesNo("Try again?")) continue;
            else break;
        }
    }
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << it+1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the book's #: " << std::endl;
    return sought[select(sought.size()) - 1];
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
            for (auto el: sought)
                std::cout << *el << std::endl;
            if (yesNo("Try again?")) continue;
            else break;
        }
    }
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << it+1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the genre: " << std::endl;
    return sought[select(sought.size()) - 1];
}

void searchUI()
{
    std::vector<Book*> books;
    std::vector<Genre*> genres;
    std::vector<Author*> authors;
    while (true)
    {
        cls();
        std::cout << "Select an option: "
                  << "\n1 -> search for a book "
                  << "\n2 -> search for a genre"
                  << "\n3 -> search for an author "
                  << "\nq -> go back" << std::endl;
        switch(getch())
        {
            case'1':
                books = searchBook(); //TODO: We need templates
                if (books.empty())
                    std::cout << "Nothing found." << std::endl;
                else
                {
                    std::cout << "Found:\n";
                    for (auto& el: books)
                        std::cout << *el << std::endl;
                }
                if (yesNo("Try again?")) break;
                else return;
            case '2':
                genres = searchGenre();
                if (genres.empty())
                    std::cout << "Nothing found." << std::endl;
                else
                {
                    std::cout << "Found:\n";
                    for (auto& el: genres)
                        std::cout << *el << std::endl;
                }
                if (yesNo("Try again?")) break;
                else return;
            case '3':
                authors = searchAuthor();
                if (authors.empty())
                    std::cout << "Nothing found." << std::endl;
                else
                {
                    std::cout << "Found:\n";
                    for (auto& el: authors)
                        std::cout << *el << std::endl;
                }
                if (yesNo("Try again?")) break;
                else return;
            case 'q':
                return;
            default:
                break;
        }
    }
}

void newAuthor()
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        std::string n, d, c;
        ull id = inputID();
        std::cout << "Enter author's credentials" << std::endl;
        while (!readString(std::cin, n, 's'));
        std::cout << "Enter the author's birthdate" << std::endl;
        while (!readString(std::cin, d, 'd'));
        std::cout << "Enter the author's country" << std::endl;
        while (!readString(std::cin, d, 's'));
        Author* pa = data.add(Author(id,n,d,c));
        while (true)
        {
            if(!yesNo("Add a book this author has written?")) break;
            Book* pb = selectBook();
            if (pb == nullptr) return;
            else
            {
                pa->addBook(*pb);
                std::cout << "Added " << pb->getName() << "to author " << n << "'s list of books" << std::endl;
            }
        }
        std::cout << "Successfully added author "<< n << std::endl;
        if (!yesNo("Add another one?")) return;
    }
}


Author* selectAuthor()
{
    std::vector<Author*> sought;
    while(true)
    {
        sought = searchAuthor();
        if (sought.empty())
        {
            std::cerr << "Nothing found." << std::endl;
            if (yesNo("Try again?")) continue;
            else return nullptr;
        }
        else
        {
            std::cout << "Found: " << std::endl;
            for (auto el: sought)
                std::cout << *el << std::endl;
            if (yesNo("Try again?")) continue;
            else break;
        }
    }
    cls();
    for (auto it = sought.begin(); it != sought.end(); ++it)
        std::cout << "#" << it+1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the author's #: " << std::endl;
    return sought[select(sought.size()) - 1];
}

void newBook() //TODO: put exit everywhere
{
    Data& data = Data::getInstance();
    while (true)
    {
        cls();
        std::string n, a, y;
        ull id = inputID();
        std::cout << "Enter the title of the book" << std::endl;
        while (!readString(std::cin, n, 's'));
        std::cout << "Enter the year the book was published" << std::endl;
        while (!readString(std::cin, y, 'y'));
        Book* pb = data.add(Book(id,n,std::stoul(y)));
        while (true)
        {
            if(!yesNo("Add an author to the book?")) break;
            Author* pa = selectAuthor();
            if (pa == nullptr) return;
            else
            {
                pb->addAuthor(*pa);
                std::cout << "Added " << pa->getName() << "to the book " << n << std::endl;
            }
        }
        while (true)
        {
            if(!yesNo("Add a genre to the book?")) break;
            Genre* pg = selectGenre();
            if (pg == nullptr) return;
            else
            {
                pb->addGenre(*pg);
                std::cout << "Added " << pg->getName() << "to the book " << n << std::endl;
            }
        }
        std::cout << "Successfully added your book" << std::endl;
        if (!yesNo("Add another one?")) return;
    }
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
              << "\n3 -> add this genre to books "
              << "\nq -> go back" << std::endl;
    auto it = pgenre->getBooks().begin();
    switch(getch())
    {
        case '1':
            std::cout << "Enter the new genre's title" << std::endl;
            while (!readString(std::cin,temp,'s'));
            pgenre->rename(temp);
            std::cout << "Renamed to " << temp << std::endl;
            sleep(WAIT_TIME_NORMAL);
            return;
        case '2':
            for (auto& b: pgenre->getBooks())
            {
                ++cnt;
                std::cout << "#" << cnt << ":\n" << *b.second << std::endl;
            }
            std::cout << "Select which book to remove: " << std::endl;
            std::advance(it, select(pgenre->getBooks().size())-1);
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
    if (!yesNo("Do you want to add this genre to any books?")) return;
    while (true)
    {
        Book* pbook = selectBook();
        if (!yesNo("Add this genre to the book?")) return;
        pbook->addGenre(*added);
        std::cout << "Added successfully: \n" << *pbook << std::endl;
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
                      << "\n5 -> search anything "
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
                      << "\n1 -> Search anything"
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
    if (data.loginCheck(l, isadmin))
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
                  << "\nTYPE YOUR PASSWORD, " << l << ", TO PROCEED OR \"exit\" TO CANCEL." << std::endl;
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
        sleep(WAIT_TIME_LONG);
        return false;
    }
    std::cout << "Deleted account  " << l << std::endl;
    sleep(WAIT_TIME_LONG);
    return true; //Was deleted
}

void admConsole(std::string& adm)
{
    Data& data = Data::getInstance();
    data.save();
    while (true)
    {
        cls();
        std::cout << "      :ADMIN:" << std::endl;
        std::cout << "Select an option: "
                  << "\n1 -> manage book data "
                  << "\n2 -> delete users "
                  << "\n3 -> register an administrator"
                  << "\n4 -> change your password"
                  << "\n0 -> delete your account (careful!)"
                  << "\nq -> sign off" << std::endl;
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
    Data& data = Data::getInstance();
    data.save();
    while (true)
    {
        cls();
        std::cout << "      :USER:"
                  << "\nSelect an option: "
                  << "\n1 -> manage book data "
                  << "\n2 -> change your password"
                  << "\n0 -> delete your account"
                  << "\nq -> sign off" << std::endl;
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
    Data& data = Data::getInstance();
    data.load();
    path = argv[0];
    path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder
#ifndef NDEBUG
    data.printbooks();
    data.printAuthors();
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
        std::cout << "\nWelcome. "
                     "\n1 -> user sign in "
                     "\n2 -> admin sign in "
                     "\nq -> exit" << std::endl;
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
catch (std::exception& e)
{
    std::cerr << "Error: " << e.what();
    getch();
    return (-2);
}
catch (...)
{
    std::cerr << "Undefined Error";
    getch();
    return (-1);
}
