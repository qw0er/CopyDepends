#include <iostream>
#include <boost/program_options.hpp>

#include "func.hpp"
int main(int argc,const char* argv[])
{
    try
    {
        const auto config=argParse(argc,argv);
        const auto d=exeDepends(config.exe);
        copyDepends(d,config.output, dependExcludeList());
    }catch (const boost::program_options::error &e)
    {
        std::cerr<<"Arguments Error: "<<e.what()<<std::endl;
        return 1;
    }catch (const std::exception &e)
    {
        std::cerr<<"Error: "<<e.what()<<std::endl;
        return 1;
    }catch (...)
    {
        std::cerr<<"Unknow Error"<<std::endl;
        return 1;
    }

}