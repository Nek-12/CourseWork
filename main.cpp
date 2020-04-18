#include "header.h"
#ifndef __linux__
    inline void cls() { system("cls"); }
#else
int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(0, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(0, TCSANOW, &oldattr);
    return (ch);
}
inline void cls() { system("clear"); }
#endif
std::string path; //extern global
//TODO: Move the stuff to different folders/files;
//TODO: Recreate the readString function to add exit functionality, edit usages;
//@@@TODO: Add nullptrs returns for every function and CHECK EVERY POINTER!@@@
//TODO: Fix: When adding/editing books, there is no check that GENRE or AUTHOR with that ID is present and vice versa

Book* newBook();
Genre* newGenre();
Author* newAuthor();

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
            else if (yesNo("Add a new book then?")) return newBook();
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
            else if (yesNo("Add a new genre then?")) return newGenre();
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
            else if (yesNo("Add a new author then?")) return newAuthor();
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
                books = searchBook();
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

Author* newAuthor()
{
    Data& data = Data::getInstance();
    cls();
    std::string n, d, c;
    ull id = inputID();
    std::cout << "Enter author's credentials" << std::endl;
    while (!readString(std::cin, n, 's'));
    std::cout << "Enter the author's birthdate" << std::endl;
    while (!readString(std::cin, d, 'd'));
    std::cout << "Enter the author's country" << std::endl;
    while (!readString(std::cin, c, 's'));
    Author* pa = data.add(Author(n, d, c, id));
    if (!pa)
    {
        std::cout << "Such author already exists" << std::endl;
        return nullptr;
    }
    while (true)
    {
        if(!yesNo("Add a book this author has written?")) break;
        Book* pb = selectBook();
        if (!pb) break;
        else
        {
            pa->addBook(*pb);
            std::cout << "Added " << pb->getName() << " to author " << n << "'s list of books" << std::endl;
        }
    }
    std::cout << "Successfully added author "<< n << std::endl;
    sleep(WAIT_TIME_NORMAL);
    return pa;
}

Book* newBook() //TODO: put exit everywhere
{
    Data& data = Data::getInstance();
    cls();
    std::string n, a, y;
    ull id = inputID();
    std::cout << "Enter the title of the book" << std::endl;
    while (!readString(std::cin, n, 's'));
    std::cout << "Enter the year the book was published" << std::endl;
    while (!readString(std::cin, y, 'y'));
    Book* added = data.add(Book(n, std::stoul(y), id));
    if (!added)
    {
        std::cerr << "Such book already exists" << std::endl;
        return nullptr;
    }
    while (true)
    {
        if(!yesNo("Add an author to this book?")) break;
        Author* pa = selectAuthor();
        if (!pa) break;
        else
        {
            added->addAuthor(*pa);
            std::cout << "Added " << pa->getName() << " to the book " << added->getName() << std::endl;
        }
    }
    while (true)
    {
        if(!yesNo("Add a genre to this book?")) break;
        Genre* pg = selectGenre();
        if (!pg) break;
        added->addGenre(*pg);
        std::cout << "Added " << pg->getName() << " to the book " << added->getName() << std::endl;
    }
    std::cout << "Successfully added your book" << std::endl;
    sleep(WAIT_TIME_NORMAL);
    return added;
}

Genre* newGenre()
{
    Data& data = Data::getInstance();
    std::string temp;
    std::cout << "Enter the new genre's name" << std::endl;
    while (!readString(std::cin, temp, 's'));
    ull id = inputID();
    Genre* added = data.add(Genre(temp, id));
    if (!added)
    {
        std::cerr << "Such genre already exists." << std::endl;
        sleep(WAIT_TIME_NORMAL);
        return nullptr;
    }
    while (true)
    {
        if(!yesNo("Add a book to this genre?")) break;
        Book* pb = selectBook();
        if (!pb) break;
        else
        {
            added->addBook(*pb);
            std::cout << "Added " << pb->getName() << " to the genre " << added->getName() << std::endl;
        }
    }
    std::cout << "Successfully added your genre" << std::endl;
    sleep(WAIT_TIME_NORMAL);
    return added;
}

void editBookAuthor(Book* pbook)
{
    if (pbook == nullptr) return;
    cls();
    std::cout << "Select an option for book " << pbook->getName() <<
              "\n1 -> Add authors to the book "
              "\n2 -> Remove authors from the book"
              "\nq -> Go back" << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                while (true)
                {
                    Author* added = selectAuthor();
                    if (!added) return;
                    if (!yesNo("Add this genre to the book?")) return;
                    pbook->addAuthor(*added);
                    std::cout << "Added successfully" << std::endl;
                    if (!yesNo("Add one more?")) return;
                    else continue;
                }
            case '2':
                std::cout << *pbook << std::endl;
                std::cout << "Select an author: " << std::endl;
                pbook->remAuthor(select(pbook->enumAuthors()-1));
                std::cout << "Removed successfully" << std::endl;
                sleep(WAIT_TIME_NORMAL);
            case'q':
                return;
            default:
                break;
        }
    }
}

void editAuthorBooks(Author* pauthor)
{
    if (pauthor == nullptr) return;
    cls();
    std::cout << "Select an option for the author " << pauthor->getName() <<
              "\n1 -> Add books to the author "
              "\n2 -> Remove books from the author"
              "\nq -> Go back" << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                while (true)
                {
                    Book* added = selectBook();
                    if (!added) return;
                    if (!yesNo("Add this book? ")) return;
                    pauthor->addBook(*added);
                    std::cout << "Added successfully" << std::endl;
                    if (!yesNo("Add one more?")) return;
                    else continue;
                }
            case '2':
                std::cout << *pauthor << std::endl;
                std::cout << "Select a book: " << std::endl;
                pauthor->remBook(select(pauthor->enumBooks()-1));
                std::cout << "Removed successfully" << std::endl;
                sleep(WAIT_TIME_NORMAL);
            case'q':
                return;
            default:
                break;
        }
    }
}

void manageAuthor()
{
    Data& data = Data::getInstance();
    std::string temp;
    Author* pauthor = selectAuthor();
    if (!pauthor) return;
    while (true)
    {
        cls();
        std::cout << *pauthor;
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
                    data.erase(*pauthor);
                    sleep(WAIT_TIME_NORMAL);
                    return;
                }
                else return;
            case '2':
                while (true)
                {
                    cls();
                    std::cout << *pauthor;
                    std::cout << "What would you like to edit? "
                                 "\n1 -> Name"
                                 "\n2 -> Birthdate"
                                 "\n3 -> Country"
                                 "\n4 -> Books"
                                 "\nq -> Nothing" << std::endl;
                    switch (getch())
                    {
                        case '1':
                            std::cout << "Enter the new name for the author: " << std::endl;
                            while (!readString(std::cin, temp, 's'));
                            pauthor->rename(temp);
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '2':
                            std::cout << "Enter the new birthdate: " << std::endl;
                            while (!readString(std::cin, temp, 'd'));
                            pauthor->setDate(temp);
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '3':
                            std::cout << "Enter the new country: " << std::endl;
                            while (!readString(std::cin, temp, 's'));
                            pauthor->setCountry(temp);
                            std::cout << "Changed successfully." << std::endl;
                            break;
                        case '4':
                            editAuthorBooks(pauthor);
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

void editBookGenre(Book* pbook)
{
    if (pbook == nullptr) return;
    cls();
    std::cout << "Select an option for book " << pbook->getName() <<
              "\n1 -> Add genres to the book "
              "\n2 -> Remove genres from the book"
              "\nq -> Go back" << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                while (true)
                {
                    Genre* added = selectGenre();
                    if (!added) return;
                    if (!yesNo("Add this genre to the book?")) return;
                    pbook->addGenre(*added);
                    std::cout << "Added successfully: \n" << *pbook << std::endl;
                    if (!yesNo("Add one more?")) return;
                    else continue;
                }
            case '2':
                std::cout << *pbook << std::endl;
                std::cout << "Select the genre: " << std::endl;
                pbook->remGenre(select(pbook->enumGenres()-1));
                std::cout << "Removed successfully" << std::endl;
                sleep(WAIT_TIME_NORMAL);
            case'q':
                return;
            default:
                break;
        }
    }
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
                    data.erase(*pbook);
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
    Genre* pgenre = selectGenre();
    if (!pgenre) return;
    Book* pbook = nullptr;
    std::cout << *pgenre << std::endl;
    std::cout << "Select an option: "
              << "\n1 -> rename this genre "
              << "\n2 -> add this genre to books "
              << "\nq -> go back" << std::endl;
    while(true)
    {
        switch (getch())
        {
            case '1':
                std::cout << "Enter the new genre's title" << std::endl;
                while (!readString(std::cin, temp, 's'));
                pgenre->rename(temp);
                std::cout << "Renamed to " << temp << std::endl;
                sleep(WAIT_TIME_NORMAL);
                return;
            case '2':
                pbook = selectBook();
                if (!pbook) return;
                if (yesNo("Add genre " + pgenre->getName() + " to the book " + pbook->getName() + " ?"))
                    pgenre->addBook(*pbook);
                return;
            case 'q':
                return;
            default:
                break;
        }
    }
}

void showData()
{
    Data& data = Data::getInstance();
    std::string temp;
    cls();
    std::cout << SHOW_DATA_MENU_ENTRIES << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                data.printBooks();
                std::cout << ANY_KEY << std::endl;
                getch();
                return;
            case '2':
                data.printAuthors();
                std::cout << ANY_KEY << std::endl;
                getch();
                return;
            case '3':
                std::cout << "Enter the time period in years: " << std::endl;
                while (!readString(std::cin, temp, 'y'));
                if (std::stoul(temp) > getCurYear())
                {
                    std::cerr << "The period you entered is invalid." << std::endl;
                    return;
                }
                data.printGenres(std::stoul(temp));
                if (!yesNo("Try again?")) return;
            case 'q':
                return;
            default:
                break;
        }
    }
}

void management(bool isadmin)
{
        while (true)
        {
            cls();
            std::cout << (isadmin ? ADMIN_MANAGEMENT_ENTRIES : USER_MANAGEMENT_ENTRIES) << std::endl;
            switch (getch())
            {
                case 'q':
                    return;
                case '1':
                    searchUI();
                    break;
                case '2':
                    showData();
                    break;
                case '3':
                    if (isadmin) newBook();
                    break;
                case '4':
                    if (isadmin) manageBook();
                    break;
                case '5':
                    if (isadmin) newAuthor();
                    break;
                case '6':
                    if (isadmin) manageAuthor();
                    break;
                case '7':
                    if (isadmin) newGenre();
                    break;
                case '8':
                    if (isadmin) editGenres();
                    break;
                default:
                    break;
            }
    }
}

bool passConfirm(std::string& p)
{
    std::string tempA, tempB;
    while (true)
    {
        cls();
        std::cout << PASSPROMPT << std::endl;
        while (!readString(std::cin, tempA, 'p'));
        if (tempA == "exit") return false;

        std::cout << PASSCONFIRM << std::endl;
        while (!readString(std::cin, tempB, 'p'));
        if (tempB == "exit") return false;
        if (tempA == tempB)
            break;
        std::cerr << "Your passwords don't match." << std::endl;
    }
    p = tempA;
    return true;
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
        std::cout << LOGINPROMPT << std::endl;
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

void createAccPrompt(const bool& isadmin)
{
    Data& data = Data::getInstance();
    std::string l, p, temp;
    cls();
#ifndef NDEBUG
    data.printCredentials(isadmin);
#endif
    std::cout << LOGINPROMPT << std::endl;
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

bool delDialog(const std::string& l, const bool& isadmin)
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

void console(const std::string& usr, const bool& isadmin)
{
    Data& data = Data::getInstance();
    data.save();
    while (true)
    {
        cls();
        std::cout << (isadmin ? ADMIN_CONSOLE_ENTRIES : USER_CONSOLE_ENTRIES) << std::endl;
        switch (getch())
        {
            case '1':
                management(isadmin);
                break;
            case '2':
                passChange(usr, isadmin);
                break;
            case '3':
                if (isadmin) createAccPrompt(isadmin);
                else continue;
                break;
            case '4':
                if (isadmin) manageUsr();
                else continue;
                break;
            case '0':
                if (delDialog(usr, isadmin)) return;
                else break;
            case 'q':
                return;
            default:
                continue;
        }
    }
}

void login(const bool& isadmin)
{
    Data& data = Data::getInstance();
    std::string usr, pass;
    while (true)
    {
        cls();
        std::cout << LOGINPROMPT << std::endl;
        while (!readString(std::cin, usr, 'n'));
        if (usr == "exit") return;
        if (data.loginCheck(usr, isadmin))
            break;
        else
            std::cout << "User not found." << std::endl;
        sleep(WAIT_TIME_SMALL);
    }
    while (true)
    {
        cls();
        std::cout << PASSPROMPT << std::endl;
        while (!readString(std::cin, pass, 'p'));
        if (pass == "exit") return;
        if (data.passCheck(usr, pass, isadmin))
            break;
        else
            std::cout << "Wrong password." << std::endl;
        sleep(WAIT_TIME_SMALL);
    }
    std::cout << "Success. Redirecting..." << std::endl;
    sleep(WAIT_TIME_NORMAL);
    console(usr, isadmin);
}

int main(int, char* argv[]) try
{
    Data& data = Data::getInstance();
    path = argv[0];
    std::cout << path << std::endl;
    path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder
    std::cout << path << std::endl;
    data.load();
#ifndef NDEBUG
    data.printBooks();
    data.printAuthors();
    data.printGenres();
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
                if (yesNo("Have you got an account?")) login(false);
                else createAccPrompt(false);
                break;
            case '2':
                login(true);
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
    std::cerr << "Critical Error: " << e.what() << "\n The program cannot continue. Press any key to exit..." << std::endl;
    getch();
    return (-2);
}
catch (...)
{
    std::cerr << "Undefined Error. \n The program cannot continue. Press any key to exit..." << std::endl;
    getch();
    return (-1);
}
