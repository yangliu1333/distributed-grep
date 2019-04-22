//
// Created by Yang Liu on 2019-04-20.
//

#include <ostream>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include "grep.h"
#include "boost/regex.hpp"
#define LOGGING_DIR "/Users/yangliu/workspace/os/cs425/mp1_boost/logs"
namespace fs = boost::filesystem;


grep::grep(grep::grep_options &options, const std::string& server_name) : server_name(server_name) {
    print_byte_offset = options.print_byte_offset;
    count_only = options.count_only;
    pattern = options.pattern;
    print_non_matching_files = options.print_non_matching_files;
    files_only = options.files_only;
    print_line_numbers = options.print_line_numbers;
    print_whole_line = options.print_whole_line;

    if (ignore_case) {
        flags |= boost::regex_constants::icase;
    }
    re.assign(pattern, flags);
}

void grep::find_pattern(std::ostream& output, const std::string &current_file) {
    output << "File " << current_file << ":"<< std::endl;
    std::ifstream is(current_file);
    std::string line;
    int match_found = 0;
    int linenum = 1;
    while(std::getline(is, line))
    {
        bool result = boost::regex_search(line, what, re);
        if(result)
        {
            if(print_non_matching_files) {
                is.close();
                return;
            }
            if(files_only)
            {
                output << current_file << std::endl;
                is.close();
                return;
            }
            if(!match_found && !count_only && (file_count > 1))
            {
                output << current_file << ":\n";
            }
            ++match_found;
            if(!count_only)
            {
                if(print_line_numbers)
                {
                    output << linenum << ":";
                }
                if(print_byte_offset)
                {
                    output << what.position() << ":";
                }
                if (print_whole_line) {
                    output << line << " : ";
                }
                output << what[0] << std::endl;
            }
        }
        ++linenum;
    }
    if(count_only && match_found)
    {
        output << match_found << " matches found in file " << current_file << std::endl;
    }
    else if(print_non_matching_files && !match_found)
    {
        output << current_file << std::endl;
    }

}


void grep::grep_on_logs(std::ostream& ostream) {

    fs::path logs_path(std::string(LOGGING_DIR));
    ostream << "Logs in server: " << server_name << std::endl;


    try {
        if (fs::exists(logs_path) && fs::is_directory(logs_path)) {

            for (const fs::directory_entry& entry : fs::directory_iterator(logs_path)) {
                find_pattern(ostream, entry.path().string());
            }
        } else {
            std::cout << "logging directory doesn't exist" << std::endl;
        }

    } catch (const fs::filesystem_error& error) {
        std::cout << error.what() << std::endl;
    }

}
