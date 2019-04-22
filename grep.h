//
// Created by Yang Liu on 2019-04-20.
//

#ifndef MP1_BOOST_GREP_H
#define MP1_BOOST_GREP_H


#include <string>
#include <boost/regex.hpp>

class grep {
public:

    typedef struct {
        bool print_byte_offset;
        bool count_only;
        std::string pattern;
        bool print_non_matching_files;
        bool files_only;
        bool print_line_numbers;
        bool ignore_case;
    } grep_options;

    explicit grep(grep_options& options);

    //take a output steam and options object and files to grep
    void find_pattern(std::ostream& output, const std::string& file_name);

    void grep_on_logs(std::ostream& ostream);

private:
    bool print_byte_offset = false;
    bool count_only = false;
    std::string pattern;
    bool print_non_matching_files = false;
    bool files_only = false;
    bool print_line_numbers = false;
    bool ignore_case = false;

    boost::regex_constants::syntax_option_type flags = boost::regex_constants::basic;
    boost::regex re;
    boost::smatch what;
    int file_count;
};


#endif //MP1_BOOST_GREP_H
