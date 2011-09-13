#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <string>

using std::string;
using boost::any;
using boost::signals2::signal;

class Signal
{
    type_info &getType() const;
    std::string getName() const;
    boost::signal2::signal &getSignal() const;
    bool attachCallback(void(boost::any) callback);
};

int main(int argc, char *argv[])
{
    return 0;
}
