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
    parameters(int argc, char * argv[]) : trainingMode{false}
    {
        parse(argc, argv);
    }

    bool training()
    {
        return trainingMode;
    }

private:
    void parse(int argc, char * argv[]);

    bool trainingMode;
};

#endif
