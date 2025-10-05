#include "func.hpp"
#include <regex>
#include <fstream>
#include <print>
#include <iostream>
#include <boost/program_options.hpp>
#include <format>
#include <unordered_set>

config argParse(const int argc, const char* argv[])
{
    namespace bp=boost::program_options;
    namespace fs=std::filesystem;
    bp::options_description desc{"Usage: copy-depends <input executable> <output directory>"};
    desc.add_options()("executable,e",bp::value<std::string>(),"input executable")
    ("output,o",bp::value<std::string>(),"output directory")
    ("help,h","help message")
    ("verbose,v",bp::bool_switch()->default_value(false),"output verbose message");
    bp::positional_options_description pd;
    pd.add("executable",1).add("output",1);
    bp::variables_map varMap;
    bp::store(bp::command_line_parser(argc,argv).options(desc).positional(pd).run(),varMap);
    bp::notify(varMap);
    if (varMap.contains("help"))
    {
        std::cout<<desc<<std::endl;
        exit (0);
    }
    if (!varMap.contains("executable")||!varMap.contains("output"))
        throw std::invalid_argument("Not enough arguments");
    config con;
    con.exe=varMap["executable"].as<std::string>();
    con.output=varMap["output"].as<std::string>();
    con.verbose=varMap["verbose"].as<bool>();

    return con;
}

std::unordered_set<std::filesystem::path> exeDepends(const std::filesystem::path& exe)
{
    if (!std::filesystem::exists(exe))
        throw std::invalid_argument("Can't find exe path");
    auto p=popen(std::format("ldd '{}'",std::filesystem::absolute(exe).string()).c_str(),"r");
    if (p==nullptr)
        throw std::runtime_error{"Use ldd failed"};
    char buffer[1024];
    std::unordered_set<std::filesystem::path> res;
    while (fgets(buffer,sizeof(buffer),p))
    {
        std::string_view bf = buffer;
        const auto pos1=bf.find('>');
        if (pos1==std::string_view::npos)
            continue;
        const auto pos2=bf.find('(');
        auto a=bf.substr(pos1+1,pos2-pos1-1);
        const auto p1=a.find_first_not_of(" \t");
        const auto p2=a.find_last_not_of(" \t");
        if (p1!=std::string::npos)
            res.insert(a.substr(p1,p2-p1+1));
    }
    pclose(p);
    return res;
}
void copyDepends(const std::unordered_set<std::filesystem::path>& depends,
                 const std::filesystem::path& output,
                 const std::unordered_set<std::string>& excludeList, bool verbose)
{
    namespace fs=std::filesystem;
    if (!fs::exists(output))
        fs::create_directories(output);
    for (const auto &t:depends)
    {
        auto outPath=output/t.filename();
        if (!fs::exists(t))
            throw std::invalid_argument(std::format("Can not find: {}",t.string()));
        std::smatch match;
        auto filename=t.filename().string();
        if (const std::regex regex{R"(.+\.so)"}; !(std::regex_search(filename,match,regex)&& excludeList.contains(match.str())))
        {
            fs::copy_file(t, outPath, fs::copy_options::overwrite_existing);
            if (verbose)
                std::println("Copy \t {} \t -> \t{} ", t.string(), outPath.string());
        }
    }
}

std::unordered_set<std::string> dependExcludeList()
{
    const std::filesystem::path elp{fileRoot()/"share/copy-depends/excludelist.txt"};
    if (!std::filesystem::exists(elp))
        throw std::invalid_argument("Exclude list is not exist");
    std::ifstream is{elp};
    std::unordered_set<std::string> res;
    std::string line;
    const std::regex regex{R"(^[^#.]+\.so)"};
    while (getline(is,line))
        if (std::smatch match; std::regex_search(line,match,regex))
            res.insert(match.str());
    return res;
}

std::filesystem::path selfPath()
{
    return std::filesystem::read_symlink("/proc/self/exe");
}
std::filesystem::path fileRoot()
{
    if (std::getenv("APPDIR"))
        return std::filesystem::path{std::getenv("APPDIR")}/"usr";
    return selfPath().parent_path().parent_path();
}
