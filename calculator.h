/**
  * \file
  *
  * \brief bjCalc engine header file.
  * \author mib383
  */

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "grammar.h"
#include "compiler.h"
#include "vm.h"
#include <string>
#include <cmath>

/**
 * @brief The bjCalc class
 *
 * Class encapsulates parser, compiler and virtual maschine in one and provides simple interface via calc function
 *
 * Usage:
 * \code
 * int main()
 * {
 *  Calculator bj;
 *  double result;
 *  std::string message;
 *  bj.calc("2+2", result, message)
 * }
 * \endcode
 */
class Calculator
{
    std::map<std::string, user_function> funs;
    std::map<std::string, double> vars;
    Parser parser;
    Compiler compiler;
    Vm executer;
    std::stringstream ss;
public:
    /**
     * @brief Calculator constructor
     *
     * Has no params.
     */
    Calculator();
    /**
     * @brief calc
     * @param text to calculate, for example "2+2"
     * @param result of calculation (if it has)
     * @param message contains error message or result of operation if no errors
     * @return is no errors
     */
    bool calc(std::string& text, double& result, std::string& message);
};
#endif // CALCULATOR_H
