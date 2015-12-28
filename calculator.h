/**
  * bjCalc engine.
  * \todo
  * - # - variable, that contains last ansver
  * - refactor interface: add string massage
  */

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <boost/spirit/include/support_utree.hpp>
#include <grammar.h>
#include <compiler.h>
#include <vm.h>
#include <string>
#include <cmath>



class Calculator
{
    std::map<std::string, user_function> funs;
    std::map<std::string, double> vars;
    Parser parser;
    Compiler compiler;
    Vm executer;
    std::stringstream ss;
public:

    Calculator() : parser(ss), compiler(funs, vars, ss), executer(funs, ss)
    {
        vars["#"] = 0;
    }

    bool calc(std::string& text, double& result, std::string& message)
    {
        auto start = text.cbegin();
        auto end = text.cend();

        operand ast;
        std::vector<boost::variant<vasm::cmd, double, std::string> > vm_program;

        boost::spirit::ascii::space_type space;
        message.clear();
        //ss.flush();
        ss.str( std::string() );
        ss.clear();

        bool parse_res = phrase_parse(start, end, parser, space, ast) && (start == end);

        if(parse_res)
        {
            Compiler::Result compile_res;
            compile_res = compiler.compile(ast, vm_program);

            switch(compile_res)
            {
                case Compiler::Result::Calculation:
                    result = executer.execute(vm_program);
                    vars["#"] = result;
                    ss << result << "\n";
                    message = ss.str();
                return true;
                case Compiler::Result::Fun_def:
                    message = ss.str();
                return true;
                case Compiler::Result::Var_def:
                    vars[compiler.getVar2def()] = result;
                    ss << "variable " << compiler.getVar2def() << " defined\n";
                    message = ss.str();
                return true;
                case Compiler::Result::Error:
                    message = ss.str();
                return false;
            }
        }
        message = "parse error " + ss.str();
        return false;
    }
};
#endif // CALCULATOR_H
