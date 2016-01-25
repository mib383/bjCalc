#ifndef FUNCTION_H
#define FUNCTION_H

#include <boost/variant.hpp>

struct user_function
{
    user_function():nargs(0){}
    user_function(std::string n, size_t a, std::vector<boost::variant<vasm::cmd, double, std::string>> c) : name(n), nargs(a), code(c){}
    user_function(const user_function& f) : name(f.name), nargs(f.nargs), code(f.code){}
    std::string name;
    size_t nargs;
    std::vector<boost::variant<vasm::cmd, double, std::string>> code;
};

#endif // FUNCTION_H
