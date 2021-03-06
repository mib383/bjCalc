#include "compiler.h"

std::string Compiler::getVar2def() const
{
    return var2def;
}

Compiler::Result Compiler::compile(operand ast, std::vector<boost::variant<vasm::cmd, double, std::string> > &result)
{
    var2def.clear();
    code.clear();
    current = &code;
    isFuncDef = false;
    isVarDef = false;
    locals.clear();

    if( (*this)(ast) )
    {
        result = code;
        if(isVarDef)
            return Result::Var_def;
        if(isFuncDef)
            return Result::Fun_def;
        return Result::Calculation;
    }

    return Result::Error;
}

bool Compiler::operator()(operand x)
{
    if (!boost::apply_visitor(*this, x))
        return false;
    return true;
}

bool Compiler::operator()(binary x)
{
    if (!boost::apply_visitor(*this, x.first))
        return false;
    BOOST_FOREACH(binary_operation const& oper, x.rest)
    {
        if (!(*this)(oper))
            return false;
    }
    return true;
}

bool Compiler::operator()(binary_operation x)
{
    if (!boost::apply_visitor(*this, x.operand_))
        return false;
    current->push_back(x.operator_);
    return true;
}

bool Compiler::operator()(unary_post x)
{
    if (!boost::apply_visitor(*this, x.operand_))
        return false;
    BOOST_FOREACH(vasm::cmd const& oper, x.operator_)
    {
        current->push_back(oper);
    }
    return true;
}

bool Compiler::operator()(unary_pre x)
{
    if (!boost::apply_visitor(*this, x.operand_))
        return false;
    current->push_back(x.operator_);
    return true;
}

bool Compiler::operator()(double x)
{
    current->push_back(vasm::value);
    current->push_back(x);
    return true;
}

bool Compiler::operator()(double_with_suffix x)
{
    current->push_back(vasm::value);
    switch(x.suffix)
    {
        case 'y': x.value *= 1e-24; break;
        case 'z': x.value *= 1e-21; break;
        case 'a': x.value *= 1e-18; break;
        case 'f': x.value *= 1e-15; break;
        case 'p': x.value *= 1e-12; break;
        case 'n': x.value *= 1e-9;  break;
        case 'u': x.value *= 1e-6;  break;
        case 'm': x.value *= 1e-3;  break;
        case 'k': x.value *= 1e3;   break;
        case 'M': x.value *= 1e6;   break;
        case 'G': x.value *= 1e9;   break;
        case 'T': x.value *= 1e12;  break;
        case 'P': x.value *= 1e15;  break;
        case 'E': x.value *= 1e18;  break;
        case 'Z': x.value *= 1e21;  break;
        case 'Y': x.value *= 1e24;  break;

        default: _outstream << "unknown suffix: " << x.suffix << '\n';
        return false;
    }

    current->push_back(x.value);
    return true;
}

bool Compiler::operator()(variable_call x)
{
    // check locals
    if(isFuncDef)
    {
        std::map<std::string, int>::const_iterator locit = locals.find(x);
        if(locit != locals.end())
        {
            current->push_back(vasm::load_local);
            current->push_back(locit->second);
            return true;
        }
    }
    std::map<std::string, double>::const_iterator i = vars.find(x);
    if(i == vars.end()) //check globals
    {
        _outstream << "undefined variable: " << x << '\n';
        return false;
    }
    current->push_back(vasm::value);
    current->push_back(i->second);
    return true;
}

bool Compiler::operator()(variable_def x)
{
    isVarDef = true;
    var2def = x.name;
    if (!boost::apply_visitor(*this, x.rhs))
        return false;

    return true;
}

bool Compiler::operator()(function_def x)
{
    int li = 0;
    std::vector<std::string> locs;
    if(x.locals)
        locs = *(x.locals);
    for(size_t i = 0; i < locs.size(); i++)
    {
        locals[locs[i]] = li++;
    }

    isFuncDef = true;
    std::vector<boost::variant<vasm::cmd, double, std::string>> c;
    funs[x.name] = user_function(x.name, locals.size(), c);
    current = &(funs[x.name].code);
    current->push_back(vasm::stack_shift);
    current->push_back(locals.size());

    if (!(*this)(x.rhs))
        return false;

     _outstream << "function " << x.name << " with " << locals.size() << " args defined\n";
    return true;
}

bool Compiler::operator()(function_call x)
{
    std::map<std::string, user_function>::const_iterator uf = funs.find(x.name);

    if(uf == funs.end())
    {
        _outstream << "undefined function: " << x.name << '\n';
        return false;
    }

    std::vector<if_expression> args;
    if(x.args)
        args = *(x.args);

    if(args.size() != uf->second.nargs)
    {
        _outstream << "function " << x.name << " expects " << uf->second.nargs << " args, but got " << args.size() << '\n';
        return false;
    }

    BOOST_FOREACH(const if_expression& e, args)
    {
        if(!(*this)(e))
            return false;
    }

    current->push_back(vasm::call);
    current->push_back(uf->second.name);
    current->push_back(args.size());
    return true;
}

bool Compiler::operator()(if_expression x)
{
    if (!(*this)(x.if_))
        return false;
    if(!x.then_else_)
        return true;


    current->push_back(vasm::jump_if);
    current->push_back(0);
    std::size_t skip = current->size()-1;

    operand thn = (*(x.then_else_))[0];

    if (!(*this)(thn))
        return false;
    (*current)[skip] = current->size()-skip;

    (*current)[skip] = boost::get<double>((*current)[skip]) + 2;
    current->push_back(vasm::jump);
    current->push_back(0);
    std::size_t exit = current->size()-1;

    operand els = (*(x.then_else_))[1];

    if (!(*this)(els))
        return false;
    (*current)[exit] = current->size()-exit;
    return true;
}


bool Compiler::operator()(core_function_call x)
{
    if(!(*this)(x.core_arg))
        return false;
    current->push_back(x.core_f);
    return true;
}
