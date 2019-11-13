#include <iostream>
#include "./include/Factory.hpp"
#include "./include/Operand.hpp"
#include "./include/Machine.hpp"

int main(int ac, char **av)
{
    Machine machine;
    std::string instruction;

    std::cout << "AVM:" << std::endl;

    if (ac > 1)
    {
        for (int i = 1; i < ac; i++)
        {
            machine.Execute(av[1]);
        }
    }
    else
    {
        while(true)
        {
            getline (std::cin, instruction);
            machine.ExecuteInstruction(instruction);
        }
    }
    return (0);
}