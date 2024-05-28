#include "lib/Parser.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(BrowserTests, TestOR) {
    int argc = 7;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "Slava",
                    (char*) "OR",
                    (char*) "University"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "\nFilename: C:\\Users\\huawei\\Desktop\\test\\first.txt\tFrequency: 2\tLines: 2 1 \n");
}

TEST(BrowserTests, TestAND) {
    int argc = 9;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "(Slava",
                    (char*) "OR",
                    (char*) "University)",
                    (char*) "AND",
                    (char*) "ITMO"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "\nFilename: C:\\Users\\huawei\\Desktop\\test\\first.txt\tFrequency: 3\tLines: 2 2 1 \n");
}

TEST(BrowserTests, TestInvalidRequest1) {
    int argc = 9;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "Slava",
                    (char*) "OR"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "Invalid request");
}

TEST(BrowserTests, TestInvalidRequest2) {
    int argc = 9;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "Slava",
                    (char*) "oR"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "Invalid request");
}

TEST(BrowserTests, TestInvalidRequest3) {
    int argc = 6;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "Slava",
                    (char*) "ITMO"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "Invalid request");
}

TEST(BrowserTests, TestInvalidRequest4) {
    int argc = 8;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "Slava",
                    (char*) "AND",
                    (char*) "OR",
                    (char*) "ITMO"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "Invalid request");
}

TEST(BrowserTests, TestToLower) {
    int argc = 9;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "(SlAvA",
                    (char*) "or",
                    (char*) "UNiVERSiTy)",
                    (char*) "AND",
                    (char*) "ITmo"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "\nFilename: C:\\Users\\huawei\\Desktop\\test\\first.txt\tFrequency: 3\tLines: 2 2 1 \n");
}

TEST(BrowserTests, TestEmptyAnswer) {
    int argc = 7;
    char* argv[] = {(char*) "labwork11.exe",
                    (char*) "--search",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index.txt",
                    (char*) "C:\\Users\\huawei\\CLionProjects\\labwork11\\Debug\\index_docs.txt",
                    (char*) "LOL",
                    (char*) "OR",
                    (char*) "kek"};

    std::ostringstream stream;
    Parser parser(argc, argv, stream, false);
    parser.ParseArguments();

    ASSERT_EQ(stream.str(), "There is no files that satisfy this request");
}