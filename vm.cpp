#include "vm.h"
#include <cmath>
#include <boost/assert.hpp>
#include <iostream>
#include <cmath>

/**
 * @brief Vm::execute
 * @param code
 * @param pc
 * @param frame_ptr
 * @return
 */
double Vm::execute(std::vector<boost::variant<vasm::cmd, double, std::string>> const& code,
                   std::vector<boost::variant<vasm::cmd, double, std::string>>::const_iterator pc,
                   std::vector<double>::iterator frame_ptr)
{
    std::vector<double>::iterator stack_ptr = frame_ptr;

    while (pc != code.end())
    {
        BOOST_ASSERT(pc != code.end());

        if(std::distance(stack.begin(), stack_ptr) > (stack.size() - 1))
        {
            _outstream << "stack overflow\n";
            break;
        }

        switch (boost::get<vasm::cmd>(*pc++))
        {
            case vasm::minus:
                stack_ptr[-1] = -stack_ptr[-1];
                break;
            case vasm::plus:
            case vasm::nop:
                //do_nothing
            break;
            case vasm::fact:

                stack_ptr[-1] = std::tgamma(stack_ptr[-1]+1);
            break;

            case vasm::inc:
                stack_ptr[-1]++;
            break;

            case vasm::dec:
                stack_ptr[-1]--;
            break;

            case vasm::pwr:
                --stack_ptr;
                stack_ptr[-1] = std::pow(stack_ptr[-1], stack_ptr[0]);
            break;
            case vasm::NOT:
                stack_ptr[-1] = !bool(stack_ptr[-1]);
                break;
            case vasm::value:
                *stack_ptr++ = boost::get<double>(*pc++);
            break;

            case vasm::add:
                --stack_ptr;
                stack_ptr[-1] += stack_ptr[0];
                break;

            case vasm::sub:
                --stack_ptr;
                stack_ptr[-1] -= stack_ptr[0];
                break;

            case vasm::mul:
                --stack_ptr;
                stack_ptr[-1] *= stack_ptr[0];
                break;

            case vasm::div:
                --stack_ptr;
                stack_ptr[-1] /= stack_ptr[0];
                break;
            case vasm::mod:
                --stack_ptr;
                stack_ptr[-1] = (int)stack_ptr[-1] % (int)stack_ptr[0];
            break;
            case vasm::eq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] == stack_ptr[0]);
                break;

            case vasm::neq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] != stack_ptr[0]);
                break;

            case vasm::lt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] < stack_ptr[0]);
                break;

            case vasm::lte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] <= stack_ptr[0]);
                break;

            case vasm::gt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] > stack_ptr[0]);
                break;

            case vasm::gte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] >= stack_ptr[0]);
                break;

            case vasm::AND:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1]) && bool(stack_ptr[0]);
                break;

            case vasm::OR:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1]) || bool(stack_ptr[0]);
                break;
            case vasm::jump:
                pc += boost::get<double>(*pc);
            break;

            case vasm::jump_if:
                if (!bool(stack_ptr[-1]))
                    pc += boost::get<double>(*pc);
                else
                    ++pc;
                --stack_ptr;
            break;

            case vasm::load_local:
                *stack_ptr++ = frame_ptr[boost::get<double>(*pc++)];
            break;

            case vasm::stack_shift:
                stack_ptr += boost::get<double>(*pc++);
            break;

            case vasm::call:
            {
                std::string f = boost::get<std::string>(*pc++);
                int nargs = boost::get<double>(*pc++);

                double r = execute(funs[f].code, funs[f].code.begin(), stack_ptr - nargs);

                stack_ptr[-nargs] = r;
                stack_ptr -= (nargs - 1);
            }
            break;

            case vasm::sin:
                stack_ptr[-1] = std::sin(stack_ptr[-1]);
                    break;
            case vasm::cos:
                stack_ptr[-1] = std::cos(stack_ptr[-1]);
                    break;
            case vasm::tan:
                stack_ptr[-1] = std::tan(stack_ptr[-1]);
                    break;
            case vasm::asin:
                stack_ptr[-1] = std::asin(stack_ptr[-1]);
                    break;
            case vasm::acos:
                stack_ptr[-1] = std::acos(stack_ptr[-1]);
                    break;
            case vasm::atan:
                stack_ptr[-1] = std::atan(stack_ptr[-1]);
                    break;
            case vasm::sinh:
                stack_ptr[-1] = std::sinh(stack_ptr[-1]);
                    break;
            case vasm::cosh:
                stack_ptr[-1] = std::cosh(stack_ptr[-1]);
                    break;
            case vasm::tanh:
                stack_ptr[-1] = std::tanh(stack_ptr[-1]);
                    break;
            case vasm::asinh:
                stack_ptr[-1] = std::asinh(stack_ptr[-1]);
                    break;
            case vasm::acosh:
                stack_ptr[-1] = std::acosh(stack_ptr[-1]);
                    break;
            case vasm::atanh:
                stack_ptr[-1] = std::atanh(stack_ptr[-1]);
                    break;
            case vasm::exp:
                stack_ptr[-1] = std::exp(stack_ptr[-1]);
                    break;
            case vasm::log:
                stack_ptr[-1] = std::log(stack_ptr[-1]);
                    break;
            case vasm::log10:
                stack_ptr[-1] = std::log10(stack_ptr[-1]);
                    break;
            case vasm::sqrt:
                stack_ptr[-1] = std::sqrt(stack_ptr[-1]);
                    break;
            case vasm::erf:
                stack_ptr[-1] = std::erf(stack_ptr[-1]);
                    break;
            case vasm::gamma:
                stack_ptr[-1] = std::tgamma(stack_ptr[-1]);
                    break;
            case vasm::abs:
                stack_ptr[-1] = std::abs(stack_ptr[-1]);
                    break;
            case vasm::round:
                stack_ptr[-1] = std::round(stack_ptr[-1]);
                    break;
        }
    }
    return stack_ptr[-1];
}

