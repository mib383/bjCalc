#include "calculator.h"
#include "boost/math/constants/constants.hpp"

Calculator::Calculator() : parser(ss), compiler(funs, vars, ss), executer(funs, ss)
{
    vars["#"] = 0;
    vars["#pi"] = boost::math::double_constants::pi;
    vars["#e"] = boost::math::double_constants::e;
    vars["#phi"] = boost::math::double_constants::phi;
    vars["#euler"] = boost::math::double_constants::euler;
    vars["#catalan"] = boost::math::double_constants::catalan;
    vars["#glaisher"] = boost::math::double_constants::glaisher;
    vars["#khinchin"] = boost::math::double_constants::khinchin;
}

bool Calculator::calc(std::string &text, double &result, std::string &message)
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
            vars[compiler.getVar2def()] = executer.execute(vm_program);
            ss << "variable " << compiler.getVar2def() << " defined and equals " << result << "\n";
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
