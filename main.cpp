/*!
  * \brief "Unnamed calculator" console test
  *
  * \author Miroshnichenko Ilya
  *
  * \todo unicode support working with bool and int (hex, bin, manual system, ...), ternar operator, operator definition
  *
  */

#include <iostream>
#include <stdio.h>
#include <calculator.h>
#include <cmath>

using namespace std;

int main()
{
    std::cout << "Welcome to bjCalc by mib383 (mib383@gmail.com)" << std::endl
              << "Type expression to start or type \"q\" to exit." << std::endl << std::endl;

    Calculator calc;
    double result;
    std::string str;
    std::string mes;

    while(true)
    {
        std::getline(std::cin, str);

        if(str == "q")
            break;

        calc.calc(str, result, mes);
        std::cout << mes << "\n\n";
    }
    return 0;
}
