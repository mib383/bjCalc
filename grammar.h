#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <boost/config/warning_disable.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <list>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/phoenix/stl/algorithm.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include "vasm.h"
#include "ast.h"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

struct Parser : qi::grammar<std::string::const_iterator, operand(), ascii::space_type>
{
    std::ostream& _outstream;
    qi::rule<std::string::const_iterator, binary(), ascii::space_type> add_expr, mul_expr, pwr_expr, logical_expr, equality_expr, relational_expr;
    qi::rule<std::string::const_iterator, unary_post(), ascii::space_type> post_unary_expr;
    qi::rule<std::string::const_iterator, operand(), ascii::space_type> pre_unary_expr, simple, start;

    qi::rule<std::string::const_iterator, std::string(), ascii::space_type> identifier, core_const, last_var;
    qi::rule<std::string::const_iterator, variable_call(), ascii::space_type> var_call;
    qi::rule<std::string::const_iterator, variable_def(), ascii::space_type> var_def;
    qi::rule<std::string::const_iterator, if_expression(), ascii::space_type> if_expr;
    qi::rule<std::string::const_iterator, function_def(), ascii::space_type> fun_def;
    qi::rule<std::string::const_iterator, function_call(), ascii::space_type> fun_call;
    qi::rule<std::string::const_iterator, core_function_call(), ascii::space_type> core_fun_call;
    qi::rule<std::string::const_iterator, double_with_suffix(), ascii::space_type> suffix;

    qi::symbols<char, vasm::cmd> op_add, op_mul, op_pwr, op_un_pre, op_un_post, logical_op, equality_op, relational_op, core_func1;
    qi::real_parser<double, qi::ureal_policies<double> > udouble_;

    explicit Parser(std::ostream& outs) : Parser::base_type(start), _outstream(outs)
    {

        logical_op.add("&&", vasm::AND)("||", vasm::OR);
        equality_op.add("==", vasm::eq)("!=", vasm::neq);
        relational_op.add("<", vasm::lt)("<=", vasm::lte)(">", vasm::gt)(">=", vasm::gte);
        op_add.add("+", vasm::add)("-", vasm::sub);
        op_mul.add("*", vasm::mul)("/", vasm::div)("%", vasm::mod);
        op_pwr.add("^", vasm::pwr)("**", vasm::pwr);
        op_un_pre.add("+", vasm::plus)("-", vasm::minus)("!", vasm::NOT);
        op_un_post.add("!", vasm::fact);
        core_func1.add  ("#sin", vasm::sin)("#cos", vasm::cos)("#tan", vasm::tan)
                        ("#asin", vasm::asin)("#acos", vasm::acos)("#atan", vasm::atan)
                        ("#sinh",vasm::sinh)("#cosh", vasm::cosh)("#tanh", vasm::tanh)
                        ("#asinh",vasm::asinh)("#acosh", vasm::acosh)("#atanh", vasm::atanh)
                        ("#exp",vasm::exp)("#log", vasm::log)("#log10", vasm::log10)
                        ("#sqrt",vasm::sqrt)("#erf", vasm::erf)("#gamma", vasm::gamma)
                        ("#abs",vasm::abs)("#round", vasm::round)
                ;

        start = var_def | fun_def | if_expr;
        if_expr = logical_expr >> -('?' > if_expr > ':' > if_expr);
        logical_expr = equality_expr >> *(logical_op > equality_expr);
        equality_expr = relational_expr >> *(equality_op > relational_expr);
        relational_expr = add_expr >> *(relational_op > add_expr);

        add_expr = mul_expr   >> *(op_add > mul_expr);
        mul_expr = pwr_expr   >> *(op_mul > pwr_expr);
        pwr_expr = pre_unary_expr >> *(op_pwr > pre_unary_expr);


        pre_unary_expr  = post_unary_expr | (op_un_pre >> pre_unary_expr);
        post_unary_expr = simple >> *(op_un_post);
        simple = suffix | udouble_ |  core_fun_call | fun_call | var_call | ('(' > if_expr > ')');

        var_def = identifier >> '=' >> if_expr;
        var_call = identifier | core_const | last_var;
        last_var = ascii::string("#");
        fun_def = identifier  >> '(' >> -(identifier % ',') >> ')' >> '=' >> if_expr;
        fun_call = identifier >> '(' >> -(if_expr % ',') >> ')';

        core_fun_call = core_func1 >> '(' >> if_expr >> ')';
        identifier = qi::raw[qi::lexeme[(qi::alpha | '_') >> *(qi::alnum | '_')]];
        core_const = qi::raw[ascii::string("#") >> identifier];

        suffix = qi::lexeme[udouble_ >> qi::alpha];

        start.name("start");
        if_expr.name("?:");
        logical_expr.name("logical_expr");
        equality_expr.name("equality_expr");
        relational_expr.name("relational_expr");
        add_expr.name("add_expr");
        mul_expr.name("mul_expr");
        pwr_expr.name("pwr_expr");
        pre_unary_expr. name("un_pre");
        post_unary_expr.name("un_post");
        simple.name("simple");
        identifier.name("ident");
        core_const.name("const");
        last_var.name("last_var");
        var_call.name("var_call");
        var_def.name("var_def");
        fun_call.name("fun_call");
        fun_def.name("fun_def");
        core_fun_call.name("core_fun_call");
        suffix.name("suffix");


        /*qi::debug(start);
        qi::debug(if_expr);
        qi::debug(logical_expr);
        qi::debug(equality_expr);
        qi::debug(relational_expr);
        qi::debug(add_expr);
        qi::debug(mul_expr);
        qi::debug(pwr_expr);
        qi::debug(pre_unary_expr);
        qi::debug(post_unary_expr);
        qi::debug(simple);
        qi::debug(identifier);
        qi::debug(var_call);
        qi::debug(var_def);
        qi::debug(fun_call);
        qi::debug(fun_def);
        qi::debug(core_fun_call);
        qi::debug(suffix);
        qi::debug(core_const);*/

        qi::on_error<qi::fail>
                    (
                        start,
                        _outstream
                        << boost::phoenix::val(" expexting ")
                        << qi::_4                               // what failed?
                        << boost::phoenix::val(" ")
                        << boost::phoenix::construct<std::string>(qi::_1, qi::_3)
                        << boost::phoenix::val("[here]")
                        << boost::phoenix::construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
                        << std::endl
                    );
    }
};

#endif // GRAMMAR_H
