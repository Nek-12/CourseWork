#include "pch.h"

void usrconsole()
{
    Data &data = Data::getInstance();
    system("cls");
    for (int i = data.books.size(); i != 0; --i)
    {
        std::cout <<
    }
}

void usrlogin()
{
    Data &data = Data::getInstance();
    std::string usr, pass;
    system("cls");
    do
    {
        std::cout << "\t \t :USER: \n Enter login or q to exit:" << std::endl;
        std::getline(std::cin, usr);
        if (usr == "q") return;
    } while (!data.ulogin(usr));
    do
    {
        std::cout << "\t \t :USER: \n Enter password for user " << usr << ": " << std::endl
                  << "Or enter q to exit";
        std::getline(std::cin, pass);
        if (pass == "q") return;
    } while (!data.upass(usr, pass));

    std::cout << "Success. Redirecting..." << std::endl;
    sleep(2);
    usrconsole();
}

int main()
{
    try
    {
        Data &data = Data::getInstance();
        std::thread bgThread(bgProc);
        while (true)
        {
            std::cout << "Welcome. Enter \n 1 for admin sign in \n 2 for user sign in \n q to exit" << std::endl;
            switch (getch())
            {
                case 'q': goto Out;
                case '1':usrlogin();
                    break;
                default:std::cout << std::endl << "Invalid input. Try again:" << std::endl;
                    break;

            }
        }
        Out:

        bgThread.join();
        return 0;

    }
    catch (std::invalid_argument(&msg))
    {
        std::cerr << "Error: " << msg.what();
        return (-2);
    }
    catch (...)
    {
        std::cerr << "Undefined Error";
        return (-1);
    }


}
