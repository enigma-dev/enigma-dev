#ifndef EMAKE_OPTIONSPARSER_HPP
#define EMAKE_OPTIONSPARSER_HPP

#include <boost/program_options.hpp>
#include <functional>

namespace opt = boost::program_options;

using func_t = std::function<int(const std::string&)>;
using list_t = std::vector<std::string>;

typedef enum
{
  OPTIONS_HELP = 2,
  OPTIONS_ERROR = 1,
  OPTIONS_SUCCESS = 0
} OptionsResult;

class OptionsParser
{
public:
  OptionsParser();
  int ReadArgs(int argc, char* argv[]);
  int HandleArgs();
  const char* APIyaml();
  std::string GetOption(std::string option);

private:
  int find_ey(const char* dir);
  int printInfo(const std::string &api);
  void printHelp();
  int searchAPI(const std::string &api, const std::string &target);
  int searchCompilers(const std::string &target);

  int help(const std::string &str);
  int output(const std::string &str);
  int mode(const std::string &str);
  int workdir(const std::string &str);
  int parse(const std::string &str);

  int graphics(const std::string &str);
  int audio(const std::string &str);
  int collision(const std::string &str);
  int widgets(const std::string &str);
  int network(const std::string &str);
  int platform(const std::string &str);
  int extensions(const std::string &str);

  int compiler(const std::string &str);

  bool _readArgsFail;

  opt::variables_map _rawArgs;
  std::map<std::string, std::string> _finalArgs;
  opt::options_description _desc;
  std::map<std::string, func_t> _handler;
  std::map<std::string, list_t> _api;
};

#endif
