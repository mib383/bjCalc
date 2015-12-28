#ifndef VM_H
#define VM_H

#include "vasm.h"
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include "user_function.h"

class Vm
{
    double execute(
        const std::vector<boost::variant<vasm::cmd, double, std::string> > &code            // the program code
      , std::vector<boost::variant<vasm::cmd, double, std::string>>::const_iterator pc     // program counter
      , std::vector<double>::iterator frame_ptr    // start of arguments and locals
    );

public:
    Vm(std::map<std::string, user_function>& fs, std::ostream& os, unsigned stackSize = 4096)
      :  funs(fs), _outstream(os), stack(stackSize)
    {
    }


    double execute(std::vector<boost::variant<vasm::cmd, double, std::string> > const& code)
    {
        return execute(code, code.begin(), stack.begin());
    }


public:
    std::map<std::string, user_function>& funs;
    std::ostream& _outstream;
    std::vector<double> stack;
};

#endif // VM_H
