#include "func.hpp"
#include <array>
#include <boost/program_options.hpp>
#include <format>

config argParse(const int argc, const char* argv[])
{
    namespace bp=boost::program_options;
    namespace fs=std::filesystem;
    bp::options_description desc{"Usage: copyDepends -e <> -o <> [options]"};
    desc.add_options()("executable,e",bp::value<std::string>()->required(),"input executable")
    ("output,o",bp::value<std::string>()->required(),"output directory");
    bp::variables_map varMap;
    bp::store(bp::parse_command_line(argc,argv,desc),varMap);
    bp::notify(varMap);
    return {varMap["executable"].as<std::string>(),varMap["output"].as<std::string>()};
}

std::vector<std::filesystem::path> exeDepends(const std::filesystem::path& exe)
{
    if (!std::filesystem::exists(exe))
        throw std::invalid_argument("Can't find exe path");
    auto p=popen(std::format("ldd '{}'",std::filesystem::absolute(exe).string()).c_str(),"r");
    if (p==nullptr)
        throw std::runtime_error{"Use ldd failed"};
    char buffer[1024];
    std::vector<std::filesystem::path> res;
    while (fgets(buffer,sizeof(buffer),p))
    {
        std::string_view bf = buffer;
        const auto pos1=bf.find('>');
        if (pos1==std::string_view::npos)
            continue;
        const auto pos2=bf.find('(');
        auto a=bf.substr(pos1+1,pos2-pos1-1);
        auto p1=a.find_first_not_of(" \t");
        auto p2=a.find_last_not_of(" \t");
        if (p1!=std::string::npos)
            res.emplace_back(a.substr(p1,p2-p1+1));
    }
    pclose(p);
    return res;
}
void copyDepends(const std::vector<std::filesystem::path>& depends, const std::filesystem::path& output)
{
    namespace fs=std::filesystem;
    if (!fs::exists(output))
        fs::create_directories(output);
    for (const auto &t:depends)
    {
        if (!fs::exists(t))
            throw std::invalid_argument(std::format("Can not find: ",t.string()));
        fs::copy_file(t,output/t.filename());
    }
}