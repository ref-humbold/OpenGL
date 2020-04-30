#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>

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
    parameters(int argc, char * argv[]) : numRows{4}, numColumns{4}
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

private:
    void parse(int argc, char * argv[]);
    void validate();

    int numRows, numColumns;
};

#endif
