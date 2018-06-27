#include "OptionsParser.hpp"
#include "Main.hpp"

#include "eyaml/eyaml.h"
#include "OS_Switchboard.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <iostream>

namespace fs = boost::filesystem;

inline std::string word_wrap(std::string text, unsigned per_line)
{
  unsigned line_begin = 0;

  while (line_begin < text.size())
  {
    const unsigned ideal_end = line_begin + per_line ;
    unsigned line_end = ideal_end <= text.size() ? ideal_end : text.size()-1;

    if (line_end == text.size() - 1)
      ++line_end;
    else if (std::isspace(text[line_end]))
    {
      text[line_end] = '\n';
      ++line_end;
    }
    else
    {
      unsigned end = line_end;
      while ( end > line_begin && !std::isspace(text[end]))
        --end;

      if (end != line_begin)
      {
        line_end = end;
        text[line_end++] = '\n';
      }
      else
        text.insert(line_end++, 1, '\n');
    }

    line_begin = line_end;
  }

  size_t pos = text.find("\n");
  while (pos != std::string::npos)
  {
    text.insert(pos + 1, 2, '\t');
    pos = text.find("\n", pos + 1);
  }

  return text;
}

inline list_t splitString(const std::string &str)
{
  list_t list;

  std::stringstream ss(str);
  while( ss.good() )
  {
    std::string substr;
    getline( ss, substr, ',' );
    list.push_back( substr );
  }

  return list;
}

OptionsParser::OptionsParser() : _desc("Options")
{
  // Platform Defaults
  std::string def_platform, def_workdir, def_compiler;
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    def_platform = "Win32";
    def_workdir = "%LOCALAPPDATA%/ENIGMA/";
    def_compiler = "gcc";
  #elif CURRENT_PLATFORM_ID ==  OS_MACOSX
    def_platform = "Cocoa";
    def_workdir = "/tmp/ENIGMA/";
    def_compiler = "clang";
  #else
    def_platform = "xlib";
    def_workdir = "/tmp/ENIGMA/";
    def_compiler = "gcc";
  #endif

  _desc.add_options()
    ("help,h", "Print help messages")
    ("list,l", "List available types, globals & functions")
    ("info,i", opt::value<std::string>(), "Provides a listing of Platforms, APIs and Extensions")
    ("input",   opt::value<std::string>()->default_value(""), "Input game file; currently, only test harness single-object games (*.sog) are supported. The --input string is optional.")
    ("quiet,q", opt::bool_switch()->default_value(false), "Suppresses output to std::out and std::err streams.")
#ifdef CLI_ENABLE_SERVER
    ("server,s", opt::bool_switch()->default_value(false), "Starts the CLI in server mode (ignores input file).")
    ("ip", opt::value<std::string>()->default_value("localhost"), "The ip address of the server when running in server mode.")
    ("port", opt::value<int>()->default_value(37818), "The port number to bind when in server mode.")
#endif
    ("output,o", opt::value<std::string>(), "Output executable file")
    ("platform,p", opt::value<std::string>()->default_value(def_platform), "Target Platform (XLib, Win32, Cocoa)")
    ("workdir,d", opt::value<std::string>()->default_value(def_workdir), "Working Directory")
    ("codegen,k", opt::value<std::string>()->default_value(def_workdir), "Codegen Directory")
    ("mode,m", opt::value<std::string>()->default_value("Debug"), "Game Mode (Run, Release, Debug, Design)")
    ("graphics,g", opt::value<std::string>()->default_value("OpenGL1"), "Graphics System (OpenGL1, OpenGL3, DirectX)")
    ("audio,a", opt::value<std::string>()->default_value("None"), "Audio System (OpenAL, DirectSound, SFML, None)")
    ("widgets,w", opt::value<std::string>()->default_value("None"), "Widget System (Win32, GTK, None)")
    ("network,n", opt::value<std::string>()->default_value("None"), "Networking System (Async, Berkeley, DirectPlay)")
    ("collision,c", opt::value<std::string>()->default_value("None"), "Collision System")
    ("extensions,e", opt::value<std::string>()->default_value("None"), "Extensions (Paths, Timelines, Particles)")
    ("compiler,x", opt::value<std::string>()->default_value(def_compiler), "Compiler.ey Descriptor")
    ("run,r", opt::bool_switch()->default_value(false), "Automatically run the game after it is built")
  ;

  _positional.add("input", 1);

  _handler["info"] = std::bind(&OptionsParser::printInfo, this, std::placeholders::_1);
  _handler["mode"] = std::bind(&OptionsParser::mode, this, std::placeholders::_1);
  _handler["graphics"] = std::bind(&OptionsParser::graphics, this, std::placeholders::_1);
  _handler["audio"] = std::bind(&OptionsParser::audio, this, std::placeholders::_1);
  _handler["collision"] = std::bind(&OptionsParser::collision, this, std::placeholders::_1);
  _handler["widgets"] = std::bind(&OptionsParser::widgets, this, std::placeholders::_1);
  _handler["network"] = std::bind(&OptionsParser::network, this, std::placeholders::_1);
  _handler["platform"] = std::bind(&OptionsParser::platform, this, std::placeholders::_1);
  _handler["extensions"] = std::bind(&OptionsParser::extensions, this, std::placeholders::_1);
  _handler["compiler"] = std::bind(&OptionsParser::compiler, this, std::placeholders::_1);
}

opt::variable_value OptionsParser::GetOption(std::string option)
{
  return _rawArgs[option];
}

bool OptionsParser::HasOption(std::string option)
{
  return _rawArgs.count(option) > 0;
}

int OptionsParser::ReadArgs(int argc, char* argv[])
{
  _readArgsFail = false;

  try
  {
    opt::store(opt::command_line_parser(argc, argv)
                   .options(_desc).positional(_positional).run(),
               _rawArgs);

    if (!_rawArgs.count("info"))
      opt::notify(_rawArgs);
#ifndef CLI_DISABLE_SERVER
    if (!_rawArgs.count("help") && !_rawArgs.count("list") && !_rawArgs.count("info") && !_rawArgs.count("server") && !_rawArgs.count("output")) {
      throw std::logic_error("Option 'help', 'list', 'info', 'server', or option 'output' is required.");
    }
#else
    if (!_rawArgs.count("help") && !_rawArgs.count("list") && !_rawArgs.count("info") && !_rawArgs.count("output")) {
      throw std::logic_error("Option 'help', 'list', 'info', or option 'output' is required.");
    }
#endif
  }
  catch(std::exception& e)
  {
    if (!_rawArgs.count("help"))
      errorStream << "OPTIONS_ERROR: " << e.what() << std::endl << std::endl;

    _readArgsFail = true;

    return OPTIONS_ERROR;
  }

  find_ey("ENIGMAsystem/SHELL/");

  // Platform Compilers
#if CURRENT_PLATFORM_ID == OS_WINDOWS
  find_ey("Compilers/Windows");
#elif CURRENT_PLATFORM_ID ==  OS_MACOSX
  find_ey("Compilers/MacOSX");
#else
  find_ey("Compilers/Linux");
#endif

  return OPTIONS_SUCCESS;
}

int OptionsParser::HandleArgs()
{
  // Exit early on list
  if (_rawArgs.count("list"))
    return OPTIONS_SUCCESS;

  // Exit early on help
  if (_readArgsFail || _rawArgs.count("help"))
  {
    printHelp();
    return OPTIONS_HELP;
  }

  for (auto &&handle : _handler)
  {
    if (_rawArgs.count(handle.first)) {
      int result = handle.second(_rawArgs[handle.first].as<std::string>());
      if (result == OPTIONS_ERROR || result == OPTIONS_HELP)
        return result;
    }
  }

  return OPTIONS_SUCCESS;
}

std::string OptionsParser::APIyaml()
{
  std::string yaml;
  yaml += "%e-yaml\n";
  yaml += "---\n";
  yaml += "treat-literals-as: 0\n";
  yaml += "sample-lots-of-radios: 0\n";
  yaml += "inherit-equivalence-from: 0\n";
  yaml += "eobjs-directory: " + boost::filesystem::absolute(_rawArgs["workdir"].as<std::string>()).string() + "\n";
  yaml += "codegen-directory: " + boost::filesystem::absolute(_rawArgs["codegen"].as<std::string>()).string() + "\n";
  yaml += "sample-checkbox: on\n";
  yaml += "sample-edit: DEADBEEF\n";
  yaml += "sample-combobox: 0\n";
  yaml += "inherit-strings-from: 0\n";
  yaml += "inherit-negatives-as: 0\n";
  yaml += "inherit-escapes-from: 0\n";
  yaml += "inherit-objects: true \n";
  yaml += "inherit-increment-from: 0\n";
  yaml += " \n";
  yaml += "target-audio: " + _rawArgs["audio"].as<std::string>() + "\n";
  yaml += "target-windowing: " + _rawArgs["platform"].as<std::string>() + "\n";
  yaml += "target-compiler: " + _rawArgs["compiler"].as<std::string>() + "\n";
  yaml += "target-graphics: " + _rawArgs["graphics"].as<std::string>() + "\n";
  yaml += "target-widget: " + _rawArgs["widgets"].as<std::string>() + "\n";
  yaml += "target-collision: " + _rawArgs["collision"].as<std::string>() + "\n";
  yaml += "target-networking: " + _rawArgs["network"].as<std::string>() + "\n";
  yaml += "extensions: " + _extensions + "\n";

  return yaml;
}

int OptionsParser::find_ey(const char* dir)
{
  boost::filesystem::path targetDir(dir);
  boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

  BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod))
  {
    if (is_regular_file(i))
    {
      auto ey = i.string().find(".ey");

      list_t apiType(8);
      apiType[0] = "Platform";
      apiType[1] = "Graphics";
      apiType[2] = "Audio";
      apiType[3] = "Collision";
      apiType[4] = "Network";
      apiType[5] = "Widget";
      apiType[6] = "Extensions";
      apiType[7] = "Compilers";

      if (ey != std::string::npos)
      {
        for (auto &&t : apiType)
        {
          auto ey = i.string().find(t);
          if (ey != std::string::npos)
            _api[t].push_back(i.string());
        }
      }
    }
  }

  return OPTIONS_SUCCESS;
}

int OptionsParser::printInfo(const std::string &api)
{
  auto it = _api.find(api);
  if (it != std::end(_api))
  {
    outputStream << std::endl << "Target " << api << ":" << std::endl;

    for (auto&& p : _api[api])
    {
      std::ifstream ifabout(p, std::ios_base::in);

      if (ifabout.is_open())
      {
        ey_data about = parse_eyaml(ifabout, p);

        std::string name = about.get("name");
        std::string desc = about.get("description");
        std::string id = about.get("identifier");
        std::string target = about.get("target-platform");

        // Why is this different in extensions?
        if (id.empty())
          id = about.get("id");

        if (!target.empty() && !name.empty())
        {
          boost::filesystem::path ey(p);
          outputStream << '\t' << name << " (" << ey.stem().string() << "):" << std::endl;
          outputStream << "\t\t Target: " << target << std::endl << std::endl;
        }
        else if (!name.empty() && !desc.empty() && !id.empty())
        {
          outputStream << '\t' << name << " (" << id << "):" << std::endl;
          outputStream << "\t\t" << word_wrap(desc, 80) << std::endl << std::endl;
        }
      }
    }
  }
  else
  {
    errorStream << "OPTIONS_ERROR: Unknown System: \"" << api  << '"'
              << std::endl << std::endl << "Avaliable Systems: " << std::endl;

    for (auto &&a : _api)
      errorStream << a.first << std::endl;

    return OPTIONS_ERROR;
  }

  return OPTIONS_HELP;
}

void OptionsParser::printHelp()
{
  outputStream << "Enigma Command Line Compiler" << std::endl
            << _desc << std::endl;
}

int OptionsParser::help(const std::string &str)
{
  // More Info on Systems (Graphics Audio Etc)
  auto it = _api.find(str);
  if (it != std::end(_api))
  {
    printInfo(str);
  }
  else
    printHelp();

  return OPTIONS_HELP;
}

int OptionsParser::parse(const std::string &str)
{
  try
  {
    fs::path file(str);
    if (fs::is_directory(file))
    {
      errorStream << "OPTIONS_ERROR: " << str << " Is a Directory!" << std::endl;
      return OPTIONS_ERROR;
    }

    bool exists = fs::exists(file);
    if (exists)
    {
      outputStream << "Parsing: " << str <<std::endl;
      // call parser
      return OPTIONS_SUCCESS;
    }
    else
    {
      errorStream << "OPTIONS_ERROR: No Such File " << str << std::endl;
    }

    return OPTIONS_ERROR;
  }
  catch (const fs::filesystem_error& ex)
  {
    errorStream << "OPTIONS_ERROR: " << ex.what() << std::endl;
    return OPTIONS_ERROR;
  }
}

int OptionsParser::mode(const std::string &str)
{
  if (str == "Run" || str == "Debug" || str == "Compile" || str == "Design" || str == "Rebuild")
  {
    return OPTIONS_SUCCESS;
  }
  else
    errorStream << "OPTIONS_ERROR: invalid mode: " << str << std::endl
              << "Available Modes: " << std::endl
              << "Run" << std::endl
              << "Debug" << std::endl
              << "Compile" << std::endl
              << "Design" << std::endl
              << "Rebuild" << std::endl;

  return OPTIONS_ERROR;
}

int OptionsParser::searchCompilers(const std::string &target)
{
  auto it = std::find_if(std::begin(_api["Compilers"]), std::end(_api["Compilers"]), [target](std::string &a)
  {
    boost::filesystem::path ey(a);
    return ey.stem().string() == target;
  });

  if (it != std::end(_api["Compilers"]))
  {
    return OPTIONS_SUCCESS;
  }
  else
    errorStream << "OPTIONS_ERROR: Unknown Compiler Target: " << target << std::endl
              << "Run \"emake --info Compiler\" For a List of Available Targets" << std::endl;

  return OPTIONS_ERROR;

}

int OptionsParser::searchAPI(const std::string &api, const std::string &target)
{
  auto it = std::find_if(std::begin(_api[api]), std::end(_api[api]), [target](std::string &a)
  {
    std::ifstream ifabout(a, std::ios_base::in);
    ey_data about = parse_eyaml(ifabout, a);
    std::string id = about.get("identifier");
    // Why is this different in extensions?
    if (id.empty())
      id = about.get("id");
    return (id == target);
  });

  if (it != std::end(_api[api]))
  {
    //set api
    std::string lower = api;
    boost::algorithm::to_lower(lower);

    if (lower == "extensions")
    {
      _extensions += "Universal_System/Extensions/" + target + ",";
    }

    return OPTIONS_SUCCESS;
  }
  else
    errorStream << "OPTIONS_ERROR: Unknown " << api << " Target: " << target << std::endl
              << "Run \"emake --info " << api << "\" For a List of Available Targets" << std::endl;

  return OPTIONS_ERROR;
}

int OptionsParser::graphics(const std::string &str)
{
  return searchAPI("Graphics", str);
}

int OptionsParser::audio(const std::string &str)
{
  return searchAPI("Audio", str);
}

int OptionsParser::collision(const std::string &str)
{
  return searchAPI("Collision", str);
}

int OptionsParser::widgets(const std::string &str)
{
  return searchAPI("Widget", str);
}

int OptionsParser::network(const std::string &str)
{
  return searchAPI("Network", str);
}

int OptionsParser::platform(const std::string &str)
{
  return searchAPI("Platform", str);
}

int OptionsParser::extensions(const std::string &str)
{
  if (str == "None")
  {
    _extensions = "";
  }
  else
  {
    list_t ext = splitString(str);

    _extensions = "";
    for (auto &&e : ext)
    {
      int valid = searchAPI("Extensions", e);
      if (valid == OPTIONS_ERROR)
        return OPTIONS_ERROR;
    }
  }

  return OPTIONS_SUCCESS;
}

int OptionsParser::compiler(const std::string &str)
{
  return searchCompilers(str);
}
