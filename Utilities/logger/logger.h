#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>

class Logger
{
public:

    enum Colors
    {
        BLUE = 1,
        RED = 4,
        YELLOW = 2,
        GREY = 8,
        PURPLE = 5,
        ORIGINAL = 7
    };

    static void SetTextColor(Colors color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        switch (color)
        {
        case BLUE:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case RED:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        case YELLOW:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case PURPLE:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case GREY:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            break;
        case ORIGINAL:
            SetConsoleTextAttribute(hConsole, 7);
            break;
        default:
            break;
        }
    }

    template <typename ... Ty>
    static void Info(const Ty&... args)
    {
        std::stringstream oss;
        (oss << ... << args);

        if (oss.str() == "???")
        {
            return;
        }

        SetTextColor(BLUE);
        std::cout << "[ INFO ] ";
        SetTextColor(ORIGINAL);


        std::cout << oss.str() << "\n";
    }
    template <typename ... Ty>
    static void Debug(const Ty&... args)
    {
        std::stringstream oss;
        (oss << ... << args);

        if (oss.str() == "???")
        {
            return;
        }

        SetTextColor(PURPLE);
        std::cout << "[ DEBUG ] ";
        SetTextColor(ORIGINAL);

        std::cout << oss.str() << "\n";
    }
    template <typename ... Ty>
    static void Error(const Ty&... args)
    {
        std::stringstream oss;
        (oss << ... << args);

        if (oss.str() == "???")
        {
            return;
        }

        SetTextColor(RED);
        std::cout << "[ ERROR ] ";
        SetTextColor(ORIGINAL);

        std::cout << oss.str() << "\n";
    }
    template <typename ... Ty>
    static void Warn(const Ty&... args)
    {
        std::stringstream oss;
        (oss << ... << args);

        if (oss.str() == "???")
        {
            return;
        }

        SetTextColor(YELLOW);
        std::cout << "[ INFO ] ";
        SetTextColor(ORIGINAL);

        std::cout << oss.str() << "\n";
    }
};
