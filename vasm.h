/**
  *
  *
  *
  *
  *
  *
  */

#ifndef VASM_H
#define VASM_H

namespace vasm{

#define CMD_LIST \
    X(add)\
    X(sub)\
    X(mul)\
    X(div)\
    X(mod)\
    X(pwr)\
    X(minus)\
    X(plus)\
    X(fact)\
    X(inc)\
    X(dec)\
    X(value)\
    X(nop)\
    X(AND)\
    X(OR)\
    X(eq)\
    X(neq)\
    X(lt)\
    X(lte)\
    X(gt)\
    X(gte)\
    X(jump_if)\
    X(jump)\
    X(NOT)\
    X(load_local)\
    X(call)\
    X(sin)\
    X(cos)\
    X(tan)\
    X(asin)\
    X(acos)\
    X(atan)\
    X(sinh)\
    X(cosh)\
    X(tanh)\
    X(asinh)\
    X(acosh)\
    X(atanh)\
    X(exp)\
    X(log)\
    X(log10)\
    X(sqrt)\
    X(erf)\
    X(gamma)\
    X(abs)\
    X(round)\
    X(stack_shift)

#define X(name) name,
enum cmd
{
    CMD_LIST
};
#undef X

extern char* cmd_names[];
}

#endif // VASM_H
