#ifndef FCL_PARSER_HPP
#define FCL_PARSER_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/config/warning_disable.hpp>
#include <iostream>
#include <vector>
#include <string>

#define $(string) #string

namespace fcl
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;
    namespace fusion = boost::fusion;

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
        KernelParser() : KernelParser::base_type(kernel_)
        {
            using qi::float_;
            using qi::int_;
            using qi::phrase_parse;
            using ascii::space;
            using boost::lambda::_1;
            using qi::lit;

            start_paren %= '(';
            end_paren %= ')';

            type_ = lit("int") | "uint" | "float" | "int*" | "uint*" | "float*";
            identifier_ = lexeme[+(char_ - (',' | ')')];

            arg_ %= type_ >> identifier_;
            args_ %= arg_ % ',';

            sig_ %=
                lit("__kernel") >> "void" >> name_ >> start_paren
                >> args_ >> end_paren;

            kernel_ = space;
        }

        qi::rule<Iterator, KernelFunc()> kernel_;
        qi::rule<Iterator, std::string()> sig_;
        qi::rule<Iterator, std::string()> name_;
        qi::rule<Iterator, std::string()> arg_;
        qi::rule<Iterator, std::vector<std::string>()> args_;
        qi::rule<Iterator, std::string()> start_paren;
        qi::rule<Iterator, std::string()> end_paren;
        qi::rule<Iterator, std::string()> type_;
        qi::rule<Iterator, std::string()> identifier_;
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

    struct mini_xml;
    typedef boost::variant<boost::recursive_wrapper<mini_xml>, std::string> mini_xml_node;
    struct mini_xml
    {
        std::string name;
        std::vector<mini_xml_node> children;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    fcl::mini_xml,
    (std::string, name)
    (std::vector<fcl::mini_xml_node>, children)
)

namespace fcl
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;
    namespace fusion = boost::fusion;

    template <typename Iterator>
    struct mini_xml_grammar : qi::grammar<Iterator, mini_xml(), ascii::space_type>
    {
        mini_xml_grammar() : mini_xml_grammar::base_type(xml)
        {
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
            using namespace qi::labels;

            using phoenix::at_c;
            using phoenix::push_back;

            text = lexeme[+(char_ - '<')    [_val += _1]];
            node = (xml | text)             [_val = _1];

            start_tag = 
                    '<'
                >> !lit('/')
                >> lexeme[+(char_ - '>')    [_val += _1]]
                >> '>'
            ;

            end_tag =
                    "</"
                >> lit(_r1)
                >> '>'
            ;

            xml = 
                    start_tag       [at_c<0>(_val) = _1]
                >> *node            [push_back(at_c<1>(_val), _1)]
                >> end_tag(at_c<0>(_val))
            ;
        }

        qi::rule<Iterator, mini_xml(), ascii::space_type> xml;
        qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
        qi::rule<Iterator, std::string(), ascii::space_type> text;
        qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
        qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
    };

    template <typename Iterator>
    bool parse_mini_xml(Iterator first, Iterator last, mini_xml& result)
    {
        using ascii::space;
        using qi::phrase_parse;
        mini_xml_grammar<Iterator> mini_xml_parser;
        bool r = phrase_parse(first, last, mini_xml_parser, space, result);

        if(first != last)
            return false;
        return r;
    }
}

#endif
