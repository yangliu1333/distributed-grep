#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include "grep.h"

using boost::asio::ip::tcp;
using json = nlohmann::json;

namespace po = boost::program_options;


grep::grep_options get_options(po::variables_map& vm) {
    grep::grep_options options;
    if(vm.count("ignore-case"))
    {
        options.ignore_case = true;
    }
    if(vm.count("byte-offset"))
    {
        options.print_byte_offset = true;
    }
    if(vm.count("count"))
    {
        options.count_only = true;
    }
    if(vm.count("files-without-match"))
    {
        options.print_non_matching_files = true;
    }
    if(vm.count("files-with-matches"))
    {
        options.files_only = true;
    }

    if(vm.count("line-number"))
    {
        options.print_line_numbers = true;
    }

    if(vm.count("input-pattern"))
    {
        options.pattern = vm["input-pattern"].as< std::string >();
    }
    return options;
}

std::string request(grep::grep_options& options) {
    json j = json{{"print_byte_offset", options.print_byte_offset},
                  {"count_only", options.count_only},
                  {"print_non_matching_files",options.print_non_matching_files},
                  {"files_only", options.files_only},
                  {"print_line_numbers", options.print_line_numbers},
                  {"ignore_case", options.ignore_case},
                  {"pattern", options.pattern}};
    return j.dump();
}

int main(int argc, char* argv[])
{

    po::options_description opts("Options");
    opts.add_options()
            ("help,h", "produce help message")
            ("byte-offset,b", "Print the byte offset within the input file before each line  of output.")
            ("count,c", "Suppress normal output; instead print a count of matching  lines for  each  input  file.  With the -v, --invert-match option (see below), count non-matching lines.")
            ("ignore-case,i", "Ignore case distinctions in  both  the  PATTERN  and  the  input files.")
            ("files-without-match,L", "Suppress  normal  output;  instead  print the name of each input file from which no output would normally have been printed.  The scanning will stop on the first match.")
            ("files-with-matches,l", "Suppress  normal  output;  instead  print the name of each input file from which output would normally have  been  printed.   The scanning will stop on the first match.")
            ("line-number,n", "Prefix each line of output with the line number within its input file.");
    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
            ("input-pattern", po::value< std::string >(), "input pattern");

    po::options_description cmdline_options;
    cmdline_options.add(opts).add(hidden);

    po::positional_options_description p;
    p.add("input-pattern", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(cmdline_options)/*.options(hidden)*/.positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << opts << "\n";
        return 0;
    }

    if(!vm.count("input-pattern")) {
        std::cout << "aa" << std::endl;
        std::cerr << "No pattern specified" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);

        tcp::resolver::results_type endpoints =
                resolver.resolve("localhost", "9000");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        //grep on local logs
        grep::grep_options options = get_options(vm);
        grep g_local(options);
        g_local.grep_on_logs(std::cout);

        std::string req = request(options);
        std::cout << req << std::endl;

        boost::asio::const_buffer write_buf = boost::asio::buffer(req, req.max_size());
        socket.write_some(write_buf);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}