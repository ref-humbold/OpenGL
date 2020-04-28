#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <unistd.h>

class params_exception : public std::logic_error
{
public:
    explicit params_exception(const std::string & s) : std::logic_error(s)
    {
    }
};

class parameters
{
public:
    parameters(int argc, char * argv[]) : numRows{4}, numColumns{4}, numColours{6}, numSigns{3}
    {
        parse(argc, argv);
        validate();
    }

    ~parameters() = default;

    int rows()
    {
        return numRows;
    }

    int columns()
    {
        return numColumns;
    }

    int colours()
    {
        return numColours;
    }

    int signs()
    {
        return numSigns;
    }

private:
    void parse(int argc, char * argv[]);
    void validate();

    int numRows, numColumns, numColours, numSigns;
};

#endif
