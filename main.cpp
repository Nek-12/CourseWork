#include "header.h"
#ifndef __linux__
inline void cls() //This function depends on platform
{ system("cls"); }
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
std::string path; //extern global string
//TODO: Fix: When adding/editing books, there is no check that GENRE or AUTHOR with that ID is present and vice versa
//TODO: Try to remove duplicated code with OOP.
//TODO: When adding new entries, end the recursion after the first time (use boolean)
Book* newBook();
Genre* newGenre();
Author* newAuthor();
//Not all declarations are needed because I arranged the functions properly. However this is a bad practice

bool yesNo(const std::string& msg)
{ //Asks for confirmation
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

ull inputID() //You can choose if you want a new ID or you already know one
{
    std::string id;
    if (!yesNo("Generate an ID?"))
    {
        std::cout << "Enter the ID: ";
        while (!readString(std::cin, id, 'i'));
    }
    else
        return genID();
    return stoid(id);
}

ull select(const ull& limit) //Select from some kind of range, used in search primarily
{
    while (true)
    {
        std::string s;
        while (!readString(std::cin, s, 'i'));
        ull ret = stoid(s);
        if (ret > limit || ret == 0)
        {
            std::cerr << "You have selected bad value. Try again: " << std::endl;
            continue;
        }
        else return ret;
    }
}
//There should be different functions for each type.
// I could not make them work with dynamic binding because the differences are too radical. See header.h -> class Entry
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
    while (true)
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
        std::cout << "#" << it + 1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the book's #: " << std::endl;
    return sought[select(sought.size()) - 1];
}

Genre* selectGenre()
{
    std::vector<Genre*> sought;
    while (true)
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
        std::cout << "#" << it + 1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the genre: " << std::endl;
    return sought[select(sought.size()) - 1];
}

Author* selectAuthor()
{
    std::vector<Author*> sought;
    while (true)
    {
        sought = searchAuthor();
        if (sought.empty())
        {
            std::cerr << "Nothing found." << std::endl;
            if (yesNo("Try again?")) continue;
            else if (yesNo("Add a new author then?")) return newAuthor();
            //because we need a different function, either I use dynamic binding here to call a proper function (which I don't want to use
            //because of the performance problems), or duplicate code like this : (
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
        std::cout << "#" << it + 1 - sought.begin() << ":\n" << **it;
    std::cout << "Select the author's #: " << std::endl;
    return sought[select(sought.size()) - 1];
}

void searchUI() //Allows us to search anything. You can search a book by its genres or authors, or a genre by its books etc.
{
    std::vector<Book*> books;
    std::vector<Genre*> genres;
    std::vector<Author*> authors;
    while (true)
    {
        cls();
        std::cout << SEARCH_UI_OPTIONS << std::endl;
        switch (getch())
        {
            case '1':
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

Author* newAuthor() //Add a new author and, if needed, provide a recursion to add something else.
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
    Author* pa = data.addAuthor(id, n, d, c, id);
    if (!pa)
    {
        std::cout << "Such author already exists" << std::endl;
        return nullptr;
    }
    while (true)
    {
        if (!yesNo("Add a book this author has written?")) break;
        Book* pb = selectBook();
        if (!pb) break;
        else
        {
            pa->addBook(*pb);
            std::cout << "Added " << pb->getName() << " to author " << n << "'s list of books" << std::endl;
        }
    }
    std::cout << "Successfully added author " << n << std::endl;
    pause();
    return pa;
}

Book* newBook()
{
    Data& data = Data::getInstance();
    cls();
    std::string n, a, y;
    ull id = inputID();
    std::cout << "Enter the title of the book" << std::endl;
    while (!readString(std::cin, n, 's'));
    std::cout << "Enter the year the book was published" << std::endl;
    while (!readString(std::cin, y, 'y'));
    Book* added = data.addBook(id, n, stoid(y), id);
    if (!added)
    {
        std::cerr << "Such book already exists" << std::endl;
        return nullptr;
    }
    while (true)
    {
        if (!yesNo("Add an author to this book?")) break;
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
        if (!yesNo("Add a genre to this book?")) break;
        Genre* pg = selectGenre();
        if (!pg) break;
        added->addGenre(*pg);
        std::cout << "Added " << pg->getName() << " to the book " << added->getName() << std::endl;
    }
    std::cout << "Successfully added your book" << std::endl;
    pause();
    return added;
}

Genre* newGenre()
{
    Data& data = Data::getInstance();
    std::string temp;
    std::cout << "Enter the new genre's name" << std::endl;
    while (!readString(std::cin, temp, 's'));
    ull id = inputID();
    Genre* added = data.addGenre(id, temp, id);
    if (!added)
    {
        std::cerr << "Such genre already exists." << std::endl;
        pause();
        return nullptr;
    }
    while (true)
    {
        if (!yesNo("Add a book to this genre?")) break;
        Book* pb = selectBook();
        if (!pb) break;
        else
        {
            added->addBook(*pb);
            std::cout << "Added " << pb->getName() << " to the genre " << added->getName() << std::endl;
        }
    }
    std::cout << "Successfully added your genre" << std::endl;
    pause();
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
                pbook->remAuthor(select(pbook->enumAuthors() - 1));
                std::cout << "Removed successfully" << std::endl;
                pause();
            case 'q':
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
                pauthor->remBook(select(pauthor->enumBooks() - 1));
                std::cout << "Removed successfully" << std::endl;
                pause();
            case 'q':
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
        std::cout << RECORD_EDITING_OPTIONS << std::endl;
        switch (getch())
        {
            case '1':
                if (yesNo("Delete this record?"))
                {
                    std::cout << "Erased this author and removed all references." << std::endl;
                    data.erase(*pauthor);
                    pause();
                    return;
                }
                else return;
            case '2':
                while (true)
                {
                    cls();
                    std::cout << *pauthor;
                    std::cout << MANAGE_BOOK_OPTIONS << std::endl;
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
                pbook->remGenre(select(pbook->enumGenres() - 1));
                std::cout << "Removed successfully" << std::endl;
                pause();
            case 'q':
                return;
            default:
                break;
        }
    }
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
        std::cout << RECORD_EDITING_OPTIONS << std::endl;
        switch (getch())
        {
            case '1':
                if (yesNo("Delete this record?"))
                {
                    std::cout << "Erased this book and removed all references." << std::endl;
                    data.erase(*pbook);
                    pause();
                    return;
                }
                else return;
            case '2':
                while (true)
                {
                    cls();
                    std::cout << *pbook;
                    std::cout << EDIT_BOOK_OPTIONS << std::endl;
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
                            pbook->setYear(stoid(temp));
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
void manageGenre()
{
    Data& data = Data::getInstance();
    std::string temp;
    Genre* pgenre = selectGenre();
    if (!pgenre) return;
    Book* pbook = nullptr;
    std::cout << *pgenre << std::endl;
    std::cout << MANAGE_GENRE_OPTIONS << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                if (yesNo("Delete this record?"))
                {
                    std::cout << "Erased this genre and removed all references." << std::endl;
                    data.erase(*pgenre);
                    pause();
                    return;
                }
                else return;
            case '2':
                std::cout << "Enter the new genre's title" << std::endl;
                while (!readString(std::cin, temp, 's'));
                pgenre->rename(temp);
                std::cout << "Renamed to " << temp << std::endl;
                pause();
                return;
            case '3':
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
    unsigned y = 0;
    cls();
    std::cout << SHOW_DATA_MENU_ENTRIES << std::endl;
    while (true)
    {
        switch (getch())
        {
            case '1':
                data.printBooks();
                pause();
                return;
            case '2':
                data.printAuthors();
                pause();
                return;
            case '3':
                std::cout << "Enter the time period in years: " << std::endl;
                while (!readString(std::cin, temp, 'y'));
                y = stoid(temp);
                if (y > getCurYear() || y == 0)
                {
                    std::cerr << "The period you entered is invalid." << std::endl;
                    pause();
                    return;
                }
                data.printGenres(y);
                pause();
                return;
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
                if (isadmin) manageGenre();
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
    pause();
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
                std::cout << "No accounts left. Exiting." << std::endl;
                pause();
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
    if (data.loginCheck(l, isadmin) || data.loginCheck(l, !isadmin))
    {
        std::cerr << "Such account already exists!" << std::endl;
        pause();
        return;
    }
    if (!passConfirm(p)) return;
    data.addAccount(l, p, isadmin);
    std::cout << "Successfully created account " << l << " ! Going back..." << std::endl;
#ifndef NDEBUG
    data.printCredentials(isadmin);
#endif
    pause();
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
        pause();
        return false;
    }
    std::cout << "Deleted account  " << l << std::endl;
    pause();
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
        if (!readString(std::cin, usr, 'n'))
        {
            pause();
            continue;
        }
        if (usr == "exit") return;
        if (data.loginCheck(usr, isadmin))
            break;
        else
            std::cout << "User not found." << std::endl;
        pause();
    }
    while (true)
    {
        cls();
        std::cout << PASSPROMPT << std::endl;
        if (!readString(std::cin, pass, 'p'))
        {
            pause();
            continue;
        }
        if (pass == "exit") return;
        if (data.passCheck(usr, pass, isadmin))
            break;
        else
            std::cout << "Wrong password.\n";
        pause();
    }
    std::cout << "Success. Redirecting..." << std::endl;
    pause();
    console(usr, isadmin);
}

int main(int, char* argv[]) try
//Try function block for convenience. Argc is unused, argv is an array of char arrays, each with an argument, first is path
{
    Data& data = Data::getInstance();
    path = argv[0];
    path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder, removing program name
    data.load(); //Loads ALL the data
#ifndef NDEBUG //For debugging
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
    bool workin = true,first = true;
    while (workin)
    {
        if(!first) cls();
        first = false;
        std::cout << WELCOME_MENU << std::endl;
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
    return EXIT_SUCCESS;
}
catch (std::exception& e) //If an exception is thrown, the program 100% can't continue. RIP.
{
    std::cerr << "Critical Error: " << e.what() << "\n The program cannot continue. Press any key to exit..." << std::endl;
    getch();
    return (EXIT_FAILURE);
}
catch (...) //Sometimes we can get something completely random. In this case we just exit
{
    std::cerr << "Undefined Error. \n The program cannot continue. Press any key to exit..." << std::endl;
    getch();
    return (EXIT_FAILURE);
}
