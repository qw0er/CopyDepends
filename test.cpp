#define BOOST_TEST_MODULE copyDependsTest
#include <algorithm>
#include <filesystem>
#include "func.hpp"
#include <boost/test/unit_test.hpp>
#include <regex>
BOOST_AUTO_TEST_SUITE(func)
BOOST_AUTO_TEST_CASE(argparse)
{
    const char* args[3] = {"copyDepends","../other/testApp", "tmp"};
    auto [exe, output]=argParse(3,args);
    BOOST_TEST(!exe.empty());
    BOOST_TEST(!output.empty());
    BOOST_TEST(exe==args[1]);
}
BOOST_AUTO_TEST_CASE(dependlist,*boost::unit_test::depends_on("func/argparse"))
{
    const char* args[3] = {"copyDepends","../other/testApp", "tmp"};
    auto [exe, output]=argParse(3,args);
    const auto depends=exeDepends(exe);
    for (const auto &t: depends)
        BOOST_TEST(std::filesystem::exists(t));
}
BOOST_AUTO_TEST_CASE(excludelist)
{
    const auto list=dependExcludeList();
    const std::regex reg(R"(^[^.]+.so$)");
    for (const auto &t:list)
        BOOST_TEST(std::regex_match(t,reg));

}
BOOST_AUTO_TEST_CASE(copyd,*boost::unit_test::depends_on("func/dependlist")
    *boost::unit_test::depends_on("func/excludelist"))
{
    const char* args[3] = {"copyDepends","../other/testApp", "tmp"};
    auto [exe, output]=argParse(3,args);
    const auto depends=exeDepends(exe);
    const auto list=dependExcludeList();
    copyDepends(depends,output,list);
}
BOOST_AUTO_TEST_CASE(excludeList)
{
    const auto list=dependExcludeList();
    BOOST_TEST(!list.empty());
    const std::regex regex{R"(.+\.so)"};
    for (const auto &t:list)
        BOOST_TEST(std::regex_match(t,regex));
}
BOOST_AUTO_TEST_CASE(path)
{
    BOOST_TEST_MESSAGE(selfPath());
    BOOST_TEST_MESSAGE(fileRoot());
}
BOOST_AUTO_TEST_SUITE_END()