#ifndef COMPILER_H
#define COMPILER_H

#include "vasm.h"
#include "grammar.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <set>
#include "user_function.h"
#include <sstream>

/**
 * @brief The Compiler class
 */
class Compiler
{
private:
    std::map<std::string, user_function>& funs;
    std::map<std::string, double>& vars;
    std::map<std::string, int> locals;
    std::string var2def;

    bool isFuncDef;
    bool isVarDef;

    std::vector<boost::variant<vasm::cmd, double, std::string>>* current;
    std::vector<boost::variant<vasm::cmd, double, std::string>> code;
    std::ostream& _outstream;

public:
    enum Result
    {
        Error,
        Calculation,
        Var_def,
        Fun_def
    };

    typedef bool result_type; // for boost::apply_visitor

    Compiler(std::map<std::string, user_function>& fs,
             std::map<std::string, double>& vs,
             std::ostream& outstream)
        : funs(fs), vars(vs), _outstream(outstream)
    {}

    Result compile(operand ast, std::vector<boost::variant<vasm::cmd, double, std::string> >& result);
    bool operator()(binary x);
    bool operator()(binary_operation x);
    bool operator()(unary_post x);
    bool operator()(unary_pre x);
    bool operator()(operand x);
    bool operator()(double x);
    bool operator()(double_with_suffix x);
    bool operator()(variable_call x);
    bool operator()(variable_def x);
    bool operator()(function_def x);
    bool operator()(function_call x);
    bool operator()(if_expression x);
    bool operator()(core_function_call x);
    std::string getVar2def() const;
};

#endif // COMPILER_H
