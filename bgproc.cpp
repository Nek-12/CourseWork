//
// Created by ender on 19-Feb-20.
//

#include "pch.h"

void bgProc(std::string path)
{
    try
    {
        Data &data = Data::getInstance();
        data.bookinit(path);
        //data.printbooks();

    }
    catch (std::runtime_error &msg)
    {
        std::cerr << msg.what() << std::endl;
        system("pause");
    }
    catch (std::invalid_argument &msg)
    {
        std::cerr << msg.what() << std::endl;
        system("pause");
    }
    catch (...)
    {
        std::cerr << "Processing failed" << std::endl;
        system("pause");
    }
}

