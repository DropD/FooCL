#ifndef FCL_PARSER_HPP
#define FCL_PARSER_HPP

#include <boost/spirit/include/qi.hpp>

namespace fcl
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template<typename Iterator>
    bool parse_numbers(Iterator first, Iterator last)
    {
        using qi::double_;
        using qi::phrase_parse;
        using ascii::space;

        bool r = phrase_parse(
                first,
                last,
                double_ >> *(',' >> double_),
                space
                );
        if(first != last) // fail if we did not  get a full match
            return false;
        return r;
    }

    class KernelParser
    {
    };
}

#endif
