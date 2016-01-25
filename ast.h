/**
  * \file
  * \author mib383
  * \brief header file which defines types of abstract syntax tree (ast) for bjCalc
  */

#ifndef AST_H
#define AST_H

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>

struct binary;
struct unary_pre;
struct unary_post;
struct if_expression;
struct function_def;
struct function_call;
struct core_function_call;

typedef std::string variable_call;

struct variable_def;

struct double_with_suffix
{
    double value;
    char suffix;
};


/**
 * @brief operand type
 *
 * Variant type for single operand in ast. Recursive wrapper because structs can contain operand fields
 */
typedef boost::variant<
    double,
    double_with_suffix,
    variable_call,
    boost::recursive_wrapper<core_function_call>,
    boost::recursive_wrapper<if_expression>,
    boost::recursive_wrapper<function_call>,
    boost::recursive_wrapper<function_def>,
    boost::recursive_wrapper<variable_def>,
    boost::recursive_wrapper<unary_post>,
    boost::recursive_wrapper<unary_pre>,
    boost::recursive_wrapper<binary>
> operand;


/**
 * @brief The variable definition abstraction
 *
 * Used for variable definition expressions. name=rhs.
 */
struct variable_def
{
    std::string name;
    operand rhs;
};

struct unary_post
{
    operand operand_;
    std::list<vasm::cmd> operator_;
};

struct unary_pre
{
    vasm::cmd operator_;
    operand operand_;
};


struct binary_operation
{
    vasm::cmd operator_;
    operand operand_;
};

struct binary
{
    operand first;
    std::list<binary_operation> rest;
};

struct if_expression_simple
{
    operand then_;
    binary else_;
};

typedef boost::optional<std::vector<operand> > then_else;

struct if_expression
{
    binary if_;
    then_else then_else_;
};

struct core_function_call
{
    vasm::cmd core_f;
    if_expression core_arg;
};

typedef boost::optional<std::vector<if_expression> > call_args;
struct function_call
{
    std::string name;
    call_args args;
};

typedef boost::optional<std::vector<std::string> > def_args;
struct function_def
{
    std::string name;
    def_args locals;
    if_expression rhs;
};


/// Adapt structs as Boost.Fusion to parse into this structs

BOOST_FUSION_ADAPT_STRUCT(
    double_with_suffix,
    (double, value)
    (char, suffix)
)

BOOST_FUSION_ADAPT_STRUCT(
    binary_operation,
    (vasm::cmd, operator_)
    (operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    variable_def,
    (std::string, name)
    (operand, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    binary,
    (operand, first)
    (std::list<binary_operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    if_expression_simple,
    (operand, then_)
    (binary, else_)
)

BOOST_FUSION_ADAPT_STRUCT(
    if_expression,
    (binary, if_)
    (then_else, then_else_)
)

BOOST_FUSION_ADAPT_STRUCT(
    core_function_call,
    (vasm::cmd, core_f)
    (if_expression, core_arg)
)

BOOST_FUSION_ADAPT_STRUCT(
    unary_post,
    (operand, operand_)
    (std::list<vasm::cmd>, operator_)
)

BOOST_FUSION_ADAPT_STRUCT(
    unary_pre,
    (vasm::cmd, operator_)
    (operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    function_def,
    (std::string, name)
    (def_args, locals)
    (if_expression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    function_call,
    (std::string, name)
    (call_args, args)
)


#endif // AST_H
