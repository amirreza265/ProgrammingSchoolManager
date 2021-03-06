#pragma once
#include<stdio.h>
#include<Windows.h>

enum class ConsoleColors
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    AQUA = 3,
    RED = 4,
    PURPLE = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_AQUA = 11,
    LIGHT_RED = 12,
    LIGHT_PURPLE = 13,
    LIGHT_YELLOW = 14,
    LIGHT_WHITE = 15
};

void setConsoleTextColor(const ConsoleColors foreground)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int k = int(foreground);
    SetConsoleTextAttribute(hConsole, k);
}