
#include "pch.h"
void bgProc()
{
    try
    {
        Data& data = Data::getInstance();
#ifdef DEBUG
        data.printbooks();
        std::cout << std::endl;
        std::cout << path << std::endl;
        data.printCredentials('u');
        std::cout << std::endl;
        data.printCredentials('a');
        std::cout << std::endl;
#endif
    }
    catch (std::runtime_error &msg)
    {
        std::cerr << msg.what() << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    catch (std::invalid_argument &msg)
    {
        std::cerr << msg.what() << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    catch (...)
    {
        std::cerr << "Processing failed." << std::endl;
        getch();
        exit(EXIT_FAILURE); //TODO: HACK
    }

}

