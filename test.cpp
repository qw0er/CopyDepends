#define BOOST_TEST_MODULE copyDependsTest
#include <filesystem>
#include "func.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(func)
BOOST_AUTO_TEST_CASE(parsearg)
{
    const char* args[5] = {"copyDepends","-e", "input/path", "-o", "output/path"};
    const auto [exe, output]=argParse(5,args);
    BOOST_TEST(!exe.empty());
    BOOST_TEST(!output.empty());
}
BOOST_AUTO_TEST_CASE(depends)
{
    const auto e=exeDepends("picWall");
    BOOST_TEST(!e.empty());
    for (const auto &t: e)
        BOOST_TEST(std::filesystem::exists(t));
    BOOST_CHECK_THROW( exeDepends("unknow") ,std::invalid_argument);
}
BOOST_AUTO_TEST_SUITE_END()