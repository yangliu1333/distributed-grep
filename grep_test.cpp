//
// Created by Yang Liu on 2019-04-20.
//

#include <fstream>
#include <cstdio>
#include "gtest/gtest.h"
#include "grep.h"

TEST(GREP, SimplePrint) {
    grep::grep_options options;
    options.pattern = "line";
    options.ignore_case = true;
    grep grep(options);
    //create a file
    std::string file_name = "test.txt";
    std::ofstream file(file_name);
    file << "Line 1\nline 2\nline 3\nline 4\n";
    file.close();
    grep.find_pattern(std::cout, file_name);

//    std::remove(file_name);
}


int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}