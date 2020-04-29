#include "Parameters.hpp"
#include <cstring>
#include <unistd.h>

using namespace std::string_literals;

void parameters::parse(int argc, char * argv[])
{
    const std::string optstring = ":r:c:"s;
    int option = getopt(argc, argv, optstring.c_str());

    opterr = 0;

    while(option != -1)
    {
        switch(option)
        {
            case 'r':
                numRows = atoi(optarg);
                break;

            case 'c':
                numColumns = atoi(optarg);
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
        throw std::runtime_error("Incorrect number of columns, minimum 1, maximum 12, got "
                                 + std::to_string(numColumns));

    if((numRows * numColumns) % 2 == 1)
        throw std::runtime_error("Odd number of cards");
}
