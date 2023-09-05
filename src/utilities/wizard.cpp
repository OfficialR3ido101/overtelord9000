#include "./wizard.h"
#include <iostream>
#include <vector>

std::vector<std::string> options;
void DisplayOptions()
{
    for (const auto &option : options)
    {
        std::cout << option << std::endl;
    }
}

int GetSelection()
{
    int selection;
    std::cout << "Enter your selection: ";
    std::cin >> selection;
    return selection;
}