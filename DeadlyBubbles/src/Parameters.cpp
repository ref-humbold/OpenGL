#include "Parameters.hpp"
#include <string>
#include <unistd.h>

using namespace std::string_literals;

void parameters::parse(int argc, char * argv[])
{
    const std::string optstring = "T"s;
    int option = getopt(argc, argv, optstring.c_str());

    opterr = 0;

    while(option != -1)
    {
        switch(option)
        {
            case 'T':
                trainingMode = true;
                break;

            case '?':
                throw params_exception("Unknown option -"s + static_cast<char>(optopt));

            default:
                break;
        }

        option = getopt(argc, argv, optstring.c_str());
    }
}
