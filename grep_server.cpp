//
// Created by Yang Liu on 2019-04-20.
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "nlohmann/json.hpp"
#include "grep.h"

using boost::asio::ip::tcp;
using json = nlohmann::json;

#define LOGGING_DIR "/Users/yangliu/workspace/os/cs425/mp1_boost/logs"

grep::grep_options convert(const json& grep_options_json) {
    grep::grep_options options;
    grep_options_json["pattern"].get_to(options.pattern);
    grep_options_json["ignore_case"].get_to(options.ignore_case);
    grep_options_json["print_line_numbers"].get_to(options.print_line_numbers);
    grep_options_json["files_only"].get_to(options.files_only);
    grep_options_json["count_only"].get_to(options.count_only);
    grep_options_json["print_non_matching_files"].get_to(options.print_non_matching_files);
    grep_options_json["print_byte_offset"].get_to(options.print_byte_offset);
    return options;
}

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 9000));

        for (;;)
        {
            tcp::iostream io_s;
            acceptor.accept(io_s.socket());
//            tcp::socket socket(io_context);
//            std::string message = make_daytime_string();
//            boost::system::error_code ignored_error;
//            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            json js;

            std::cout << "hehe" << std::endl;

            io_s >> js;
            grep::grep_options options = convert(js);
            grep g(options);
            g.grep_on_logs(io_s);

            io_s.close();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}