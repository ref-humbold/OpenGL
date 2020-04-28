#include "Parameters.hpp"

using namespace std::string_literals;

void parameters::parse(int argc, char * argv[])
{
    const std::string optstring = ":R:C:c:s:"s;
    int option = getopt(argc, argv, optstring.c_str());

    opterr = 0;

    while(option != -1)
    {
        switch(option)
        {
            case 'R':
                numRows = atoi(optarg);
                break;

            case 'C':
                numColumns = atoi(optarg);
                break;

            case 'c':
                numColours = atoi(optarg);
                break;

            case 's':
                numSigns = atoi(optarg);
                break;

            case '?':
                throw params_exception("Unknown option -"s + static_cast<char>(optopt));

            case ':':
                throw params_exception("Option -"s + static_cast<char>(optopt)
                                       + " requires an argument"s);

            default:
                break;
        }

        option = getopt(argc, argv, optstring.c_str());
    }
}

void parameters::validate()
{
    if(numRows < 1 || numRows > 12)
        throw params_exception("Incorrect number of rows, minimum 1, maximum 12, got "
                               + std::to_string(numRows));

    if(numColumns < 1 || numColumns > 12)
        throw params_exception("Incorrect number of columns, minimum 1, maximum 12, got "
                               + std::to_string(numColumns));

    if(numColours < 2 || numColours > 6)
        throw params_exception("Incorrect number of card colours, minimum 2, maximum 6, got "
                               + std::to_string(numColours));

    if(numSigns < 1 || numSigns > 3)
        throw params_exception("Incorrect number of card signs, minimum 1, maximum 3, got "
                               + std::to_string(numSigns));

    if(numRows % 2 != 0 || numColumns % 2 != 0)
        throw params_exception("Odd number of cards");
}
