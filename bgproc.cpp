//
// Created by ender on 19-Feb-20.
//

#include "pch.h"

void bgProc()
{
    try
    {
        Data& data = Data::getInstance();
        data.bookinit();
        data.adminit();
        data.uinit();
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
        system("pause");
        exit(EXIT_FAILURE);
    }
    catch (std::invalid_argument &msg)
    {
        std::cerr << msg.what() << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    catch (...)
    {
        std::cerr << "Processing failed." << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

}

