#include "pch.h"

void admConsole()
{
        while (true)
        {
            system("cls");
            std::cout << ":ADMIN:"
            switch (getch())
            {
                case 'q': goto Out;
                case '1':usrlogin();
                    break;
                case '2':admlogin();
                    break;
                default:std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;
            }
        }
        Out:

        return;
}
    void admlogin()
    {
        Data &data = Data::getInstance();
        std::string adm, pass;
        system("cls");
        do
        {
            std::cout << ":ADMIN: \n Enter login or \"exit\" to exit:" << std::endl;
            readString(std::cin, adm, 'n');
            if (adm == "exit") return;
        } while (!data.loginCheck(adm,true));

        do
        {
            std::cout << "Enter your password or \"exit\" to exit:" << std::endl;
            readString(std::cin, pass, 'n');
            if (pass == "exit") return;
        } while (!data.passCheck(adm, pass,true));

        std::cout << "Success. Redirecting..." << std::endl;
        sleep(2);
        admConsole();
    }

    void usrconsole()
    {
        Data &data = Data::getInstance();
        system("cls");
        //TODO: Show the data and implement edit
//    for (int i = data.books.size(); i != 0; --i)
//    {
//        std::cout <<
//    }
    }

    void usrlogin()
    {
        Data &data = Data::getInstance();
        std::string usr, pass;
        system("cls");
        do
        {
            std::cout << ":USER: \n Enter login or \"exit\" to exit:" << std::endl;
            readString(std::cin, usr, 'n');
            if (usr == "exit") return;
        } while (!data.loginCheck(usr, false));

        do
        {
            std::cout << "Enter your password or \"exit\" to exit:" << std::endl;
            readString(std::cin, pass, 'n');
            if (pass == "exit") return;
        } while (!data.passCheck(usr, pass, false));

        std::cout << "Success. Redirecting..." << std::endl;
        sleep(2);
        usrconsole();
    }

    int main(int argc, char *argv[])
    {
        try
        {
            Data &data = Data::getInstance();
            std::string path = argv[0];
            path.erase(path.find_last_of('\\') + 1); //Makes 'path' be the path to the app folder

            std::thread bgThread(bgProc, path);

            bgThread.join(); //TODO: Remove outta here when done debugging it
            while (true)
            {
                std::cout << "Welcome. Enter \n 1 for user sign in \n 2 for admin sign in \n q to exit" << std::endl;
                switch (getch())
                {
                    case 'q': goto Out;
                    case '1':usrlogin();
                        break;
                    case '2':admlogin();
                        break;
                    default:std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                        break;
                }
            }
            Out:

            sleep(2);
            return 0;

        }
        catch (std::invalid_argument &msg)
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
