#ifndef FCL_PARSER_HPP
#define FCL_PARSER_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <vector>

#define $(string) #string

namespace fcl
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    std::vector<double> dvec;

    template<typename Iterator>
    bool parse_numbers(Iterator first, Iterator last)
    {
        using qi::double_;
        using qi::phrase_parse;
        using ascii::space;
        using boost::lambda::_1;

        bool r = phrase_parse(
                first,
                last,
            double_[std::cout << _1 << "\n"] >> *(',' >> double_[std::cout << _1 << "\n"]),
                space
                );
        if(first != last) // fail if we did not  get a full match
            return false;
        return r;
    }

    struct qi_builtins_ : qi::symbols<char, unsigned>
    {
    } qi_builtins;

    template <typename Iterator>
    struct KernelParser : qi::grammar<Iterator, KernelFunc()>
    {
        KernelParser() : KernelParser::base_type(start)
        {
            using qi::float_;
            using qi::int_;
            using qi::phrase_parse;
            using ascii::space;
            using boost::lambda::_1;

            start = space;
        }

        qi::rule<Iterator, KernelFunc()> start;
    };

    template <typename Iterator>
    bool parse_complex(Iterator first, Iterator last, std::complex<double>& c)
    {
        using qi::double_;
        using qi::_1;
        using qi::phrase_parse;
        using ascii::space;
        using phoenix::ref;

        double rN = 0.0;
        double iN = 0.0;
        bool r = phrase_parse(first, last,

            // Begin grammar
            (
                '(' >> double_[ref(rN) = _1]
                    >> -(',' >> double_[ref(iN) = _1]) >> ')'
                | double_[ref(rN) = _1]
            ),
            // End grammar

            space);

        if(!r || first != last) // fail if we did not get a full match
            return false;
        c = std::complex<double>(rN, iN);
        return r;
    }

    template <typename Iterator>
    bool parse_list(Iterator first, Iterator last, std::vector<double>& v)
    {
        using qi::double_;
        using qi::phrase_parse;
        using qi::_1;
        using ascii::space;
        using phoenix::ref;
        using phoenix::push_back;

        bool r = phrase_parse(first, last, 

            // Begin grammar
            (
                double_/*[push_back(ref(v), _1)]*/ % ','
            ),
            // End grammar

            space, v);
        
        if(first != last)
            return false;
        return r;
    }

    struct hundreds_ : qi::symbols<char, unsigned>
    {
        hundreds_()
        {
            add
                ("C"    , 100)
                ("CC"   , 200)
                ("CCC"  , 300)
                ("CD"   , 400)
                ("D"    , 500)
                ("DC"   , 600)
                ("DCC"  , 700)
                ("DCCC" , 800)
                ("CM"   , 900)
            ;
        }
    } hundreds;

    struct tens_ : qi::symbols<char, unsigned>
    {
        tens_()
        {
            add
                ("X"    , 10)
                ("XX"   , 20)
                ("XXX"  , 30)
                ("XL"   , 40)
                ("L"    , 50)
                ("LX"   , 60)
                ("LXX"  , 70)
                ("LXXX" , 80)
                ("XC"   , 90)
            ;
        }
    } tens;

    struct ones_ : qi::symbols<char, unsigned>
    {
        ones_()
        {
            add
                ("I"    , 1)
                ("II"   , 2)
                ("III"  , 3)
                ("IV"   , 4)
                ("V"    , 5)
                ("VI"   , 6)
                ("VII"  , 7)
                ("VIII" , 8)
                ("IX"   , 9)
            ;
        }
    } ones;

    template <typename Iterator>
    struct roman : qi::grammar<Iterator, unsigned()>
    {
        roman() : roman::base_type(start)
        {
            using qi::eps;
            using qi::lit;
            using qi::_val;
            using qi::_1;
            using ascii::space;

            start = eps         [_val = 0] >>
                (
                    +lit('M')   [_val += 1000]
                    || hundreds [_val += _1]
                    || tens     [_val += _1]
                    || ones     [_val += _1]
                )
            ;
        }

        qi::rule<Iterator, unsigned()> start;
    };

    template <typename Iterator>
    bool parse_roman(Iterator first, Iterator last, unsigned& result)
    {
        roman<Iterator> roman_parser;
        bool r = parse(first, last, roman_parser, result);
        
        if(first != last)
            return false;
        return r;
    }
}

#endif
