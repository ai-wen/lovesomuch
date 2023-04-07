#include <iostream>
#include "argparse.hpp"
#include "range.hpp"
#include "cmdline.hpp"
using namespace std;


#define VERSION "0.0.1"

namespace util
{

template <>
inline std::string type_string<StepRange>()
{
    return "StepRange";
}

template <>
StepRange parse_value<StepRange>(const std::string &str)
{
    auto pos = str.find(':');
    if (pos == std::string::npos)
    {
        throw(std::invalid_argument("invalid StepRange format"));
    }
    int64_t m_begin = std::stoll(str.substr(0, pos));
    auto next_pos = str.find(':', pos + 1);
    if (next_pos == std::string::npos)
    {
        throw(std::invalid_argument("invalid StepRange format"));
    }
    int64_t m_step = std::stoll(str.substr(pos + 1, next_pos - pos - 1));
    int64_t m_end = std::stoll(str.substr(next_pos + 1));
    return range(m_begin, m_end, m_step);
}

} // namespace util






#include "posix_getopt.hpp"


int main(int argc, char *argv[])
{

    //testGetOpt(argc, argv);
    //testGetOptLong(argc, argv);



#if 0    
    auto args = util::argparser("A quantum physics calculation program.");
    args.set_program_name("test")
        .add_help_option()
        .add_sc_option("-v", "--version", "show version info",
                       []() { std::cout << "version " << VERSION << std::endl; })
        .add_option("-o", "--openmp", "use openmp or not")
        .add_option("-m", "--mpi", "use mpi or not")
        .add_option<int>("-t", "--threads", "if openmp it set,\nuse how many threads,\ndefault is 4", 4)
        .add_option<util::StepRange>("-r", "--range", "range", util::range(0, 10, 2))
        .add_named_argument<std::string>("input", "initialize file")
        .add_named_argument<std::string>("output", "output file")
        .parse(argc, argv);
    if (args.has_option("--openmp"))
    {
        std::cout << "openmp is used, and we use " << args.get_option_int("--threads") << " threads" << std::endl;
    }
    if (args.has_option("--mpi"))
    {
        std::cout << "mpi is used" << std::endl;
    }
    std::cout << "calculate range is " << args.get_option<util::StepRange>("--range") << std::endl;
    std::cout << "the input file is " << args.get_argument<std::string>("input") << std::endl;
    std::cout << "the output file is " << args.get_argument<std::string>("output") << std::endl;
    args.print_as_ini(std::cout);

#else
    // create a parser
  cmdline::parser a;

  // add specified type of variable.
  // 1st argument is long name
  // 2nd argument is short name (no short name if '\0' specified)
  // 3rd argument is description
  // 4th argument is mandatory (optional. default is false)
  // 5th argument is default value  (optional. it used when mandatory is false)
  a.add<string>("host", 'h', "host name", true, "");

  // 6th argument is extra constraint.
  // Here, port number must be 1 to 65535 by cmdline::range().
  a.add<int>("port", 'p', "port number", false, 80, cmdline::range(1, 65535));

  // cmdline::oneof() can restrict options.
  a.add<string>("type", 't', "protocol type", false, "http", cmdline::oneof<string>("http", "https", "ssh", "ftp"));

  // Boolean flags also can be defined.
  // Call add method without a type parameter.
  a.add("gzip", '\0', "gzip when transfer");

  // Run parser.
  // It returns only if command line arguments are valid.
  // If arguments are invalid, a parser output error msgs then exit program.
  // If help flag ('--help' or '-?') is specified, a parser output usage message then exit program.
  a.parse_check(argc, argv);

  // use flag values
  cout << a.get<string>("type") << "://"
       << a.get<string>("host") << ":"
       << a.get<int>("port") << endl;

  // boolean flags are referred by calling exist() method.
  if (a.exist("gzip")) cout << "gzip" << endl;
#endif
    return 0;
}

