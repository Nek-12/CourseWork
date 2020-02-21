//
// Created by ender on 19-Feb-20.
//

#include "pch.h"

void bgProc(const std::string& path)
{
    try
    {
        Data &data = Data::getInstance();
        data.bookinit(path);
        data.printbooks();
        std::cout << std::endl;
        data.adminit(path);
        data.printCredentials('a');
        std::cout << std::endl;
        data.uinit(path);
        data.printCredentials('u');
        std::cout << std::endl;
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

