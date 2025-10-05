#define BOOST_TEST_MODULE copyDependsTest
#include <algorithm>
#include <filesystem>
#include "func.hpp"
#include <boost/test/unit_test.hpp>
#include <regex>
#include <boost/program_options/options_description.hpp>
BOOST_AUTO_TEST_SUITE(func)
    BOOST_AUTO_TEST_SUITE(argP)
BOOST_AUTO_TEST_CASE(arg1)
{
    const char* args[3] = {"copyDepends","../other/testApp", "tmp"};
    auto [exe, output,verbose]=argParse(3,args);
    BOOST_TEST(!exe.empty());
    BOOST_TEST(!output.empty());
    BOOST_TEST(exe==args[1]);
    BOOST_TEST(!verbose);
}
BOOST_AUTO_TEST_CASE(arg2)
{
    const char* args[]={"copy-depends"};
    BOOST_CHECK_THROW(argParse(1,args),std::invalid_argument);
}
BOOST_AUTO_TEST_CASE(arg3)
{
    const char* args[]={"copy-depends","-v","exe","ttt"};
    auto [exe, output,verbose]=argParse(4,args);
    BOOST_TEST(!exe.empty());
    BOOST_TEST(!output.empty());
    BOOST_TEST(exe==args[2]);
    BOOST_TEST(verbose);
}
BOOST_AUTO_TEST_CASE(arg4)
{
    const char* args[]={"copy-depends","-v","exe","ttt","hhhhh","helo"};
    BOOST_CHECK_THROW(argParse(6,args),boost::program_options::error);
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_CASE(dependlist,*boost::unit_test::depends_on("func/argP"))
{
    const char* args[3] = {"copyDepends","../other/testApp", "tmp"};
    auto [exe, output,v]=argParse(3,args);
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
    auto [exe, output,v]=argParse(3,args);
    const auto depends=exeDepends(exe);
    const auto list=dependExcludeList();
    copyDepends(depends,output,list, v);
    std::unordered_set<std::string> copiedFile;
    for (const auto &t:std::filesystem::recursive_directory_iterator{"tmp"})
        copiedFile.insert(t.path().filename());
    for (const auto &t: depends)
    {
        const std::regex regex{R"(.+\.so)"};
        std::smatch match;
        std::string filename{t.filename().string()};
        std::regex_search(filename,match,regex);
        BOOST_TEST((copiedFile.contains(t.filename().string())||list.contains(match.str())));
    }
}
BOOST_AUTO_TEST_CASE(path)
{
    BOOST_TEST_MESSAGE(selfPath());
    BOOST_TEST_MESSAGE(fileRoot());
}
BOOST_AUTO_TEST_SUITE_END()