#include "OptionsParser.hpp"

#include "settings-parse/eyaml.h"
#include "OS_Switchboard.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/algorithm/string.hpp>

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
  _desc.add_options()

  ("help,h", "Print help messages")
  ("info,i", opt::value<std::string>(), "Provides a listing of Platforms, APIs and Extensions")
  ("output,o", opt::value<std::string>()->required(), "Output file")
  ("workdir,w", opt::value<std::string>(), "Working Directory")
  ("mode,m", opt::value<std::string>(), "Game Mode (Run, Release, Debug, Design)")
  ("graphics,g", opt::value<std::string>(), "Graphics System (OpenGL1, OpenGL3, DirectX)")
  ("audio,a", opt::value<std::string>(), "Audio System (OpenAL, DirectSound, SFML, None)")
  ("widgets,W", opt::value<std::string>(), "Widget System (GTK, None)")
  ("network,n", opt::value<std::string>(), "Networking System (Async, Berkeley, DirectPlay)")
  ("collision,c", opt::value<std::string>(), "Collision System")
  ("platform,p", opt::value<std::string>(), "Target Platform (XLib, Win32, Cocoa)")
  ("extensions,e", opt::value<std::string>(), "Extensions (Paths, Timelines, Particles)")
  ("compiler,x", opt::value<std::string>(), "Compiler.ey Descriptor");


  _handler["info"] = std::bind(&OptionsParser::printInfo, this, std::placeholders::_1);
  _handler["output"] = std::bind(&OptionsParser::output, this, std::placeholders::_1);
  _handler["workdir"] = std::bind(&OptionsParser::workdir, this, std::placeholders::_1);
  _handler["mode"] = std::bind(&OptionsParser::mode, this, std::placeholders::_1);
  _handler["graphics"] = std::bind(&OptionsParser::graphics, this, std::placeholders::_1);
  _handler["audio"] = std::bind(&OptionsParser::audio, this, std::placeholders::_1);
  _handler["collision"] = std::bind(&OptionsParser::collision, this, std::placeholders::_1);
  _handler["widgets"] = std::bind(&OptionsParser::widgets, this, std::placeholders::_1);
  _handler["network"] = std::bind(&OptionsParser::network, this, std::placeholders::_1);
  _handler["platform"] = std::bind(&OptionsParser::platform, this, std::placeholders::_1);
  _handler["extensions"] = std::bind(&OptionsParser::extensions, this, std::placeholders::_1);
  _handler["compiler"] = std::bind(&OptionsParser::compiler, this, std::placeholders::_1);


  // Platform Defaults
#if CURRENT_PLATFORM_ID == OS_WINDOWS
  _finalArgs["platform"] = "Win32";
  _finalArgs["workdir"] = "%PROGRAMDATA%/ENIGMA/";
#elseif CURRENT_PLATFORM_ID ==  OS_MACOSX
  _finalArgs["platform"] = "Cocoa";
  _finalArgs["workdir"] = "/tmp/ENIGMA/";
#else
  _finalArgs["platform"] = "xlib";
  _finalArgs["workdir"] = "/tmp/ENIGMA/";
#endif

  // Universal Defaults
  _finalArgs["graphics"] = "OpenGL1";
  _finalArgs["audio"] = "None";
  _finalArgs["widget"] = "None";
  _finalArgs["network"] = "None";
  _finalArgs["collision"] = "None";
  _finalArgs["mode"] = "Debug";

  // Compiler Defaults
  _finalArgs["compiler"] = "gcc";

}

std::string OptionsParser::GetOption(std::string option)
{
  return _finalArgs[option];
}

int OptionsParser::ReadArgs(int argc, char* argv[])
{
  _readArgsFail = false;

  try
  {
    opt::store(opt::parse_command_line(argc, argv, _desc), _rawArgs);

    if (!_rawArgs.count("info"))
      opt::notify(_rawArgs);
  }
  catch(opt::error& e)
  {
    std::cerr << "OPTIONS_ERROR: " << e.what() << std::endl << std::endl;

    _readArgsFail = true;

    return OPTIONS_ERROR;
  }

  find_ey("ENIGMAsystem/SHELL/");

  // Platform Compilers
#if CURRENT_PLATFORM_ID == OS_WINDOWS
  find_ey("Compilers/Windows");
#elseif CURRENT_PLATFORM_ID ==  OS_MACOSX
  find_ey("Compilers/MacOSX");
#else
  find_ey("Compilers/Linux");
#endif

  return OPTIONS_SUCCESS;
}

int OptionsParser::HandleArgs()
{
  // Exit early on help
  if (_readArgsFail || _rawArgs.count("help"))
  {
    printHelp();
    return OPTIONS_HELP;
  }


  for (auto &&arg : _rawArgs)
  {
    std::string str = boost::any_cast<std::string>(arg.second.value());
    int result = _handler[arg.first](str);
    if (result == OPTIONS_ERROR || result == OPTIONS_HELP)
      return result;
  }

  return OPTIONS_SUCCESS;
}

const char* OptionsParser::APIyaml()
{
  std::string yaml;
  yaml += "%e-yaml\n";
  yaml += "---\n";
  yaml += "treat-literals-as: 0\n";
  yaml += "sample-lots-of-radios: 0\n";
  yaml += "inherit-equivalence-from: 0\n";
  yaml += "make-directory: " + _finalArgs["workdir"] + "\n";
  yaml += "sample-checkbox: on\n";
  yaml += "sample-edit: DEADBEEF\n";
  yaml += "sample-combobox: 0\n";
  yaml += "inherit-strings-from: 0\n";
  yaml += "inherit-negatives-as: 0\n";
  yaml += "inherit-escapes-from: 0\n";
  yaml += "inherit-objects: true \n";
  yaml += "inherit-increment-from: 0\n";
  yaml += " \n";
  yaml += "target-audio: " + _finalArgs["audio"] + "\n";
  yaml += "target-windowing: " + _finalArgs["platform"] + "\n";
  yaml += "target-compiler: " + _finalArgs["compiler"] + "\n";
  yaml += "target-graphics: " + _finalArgs["graphics"] + "\n";
  yaml += "target-widget: " + _finalArgs["widget"] + "\n";
  yaml += "target-collision: " + _finalArgs["collision"] + "\n";
  yaml += "target-networking: " + _finalArgs["network"] + "\n";
  yaml += "extensions: " + _finalArgs["extensions"] + "\n";

  return yaml.c_str();
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
      apiType[0] = "Platforms";
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
    std::cout << std::endl << "Target " << api << ":" << std::endl;

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
          std::cout << '\t' << name << " (" << ey.stem().string() << "):" << std::endl;
          std::cout << "\t\t Target: " << target << std::endl << std::endl;
        }
        else if (!name.empty() && !desc.empty() && !id.empty())
        {
          std::cout << '\t' << name << " (" << id << "):" << std::endl;
          std::cout << "\t\t" << word_wrap(desc, 80) << std::endl << std::endl;
        }
      }
    }
  }
  else
  {
    std::cerr << "OPTIONS_ERROR: Unknown System: \"" << api  << '"'
              << std::endl << std::endl << "Avaliable Systems: " << std::endl;

    for (auto &&a : _api)
      std::cerr << a.first << std::endl;

    return OPTIONS_ERROR;
  }

  return OPTIONS_HELP;
}

void OptionsParser::printHelp()
{
  std::cout << "Enigma Command Line Compiler" << std::endl
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

int OptionsParser::output(const std::string &str)
{
  //set outfile
  _finalArgs["output"] = str;
  return OPTIONS_SUCCESS;
}

int OptionsParser::parse(const std::string &str)
{
  try
  {
    fs::path file(str);
    if (fs::is_directory(file))
    {
      std::cerr << "OPTIONS_ERROR: " << str << " Is a Directory!" << std::endl;
      return OPTIONS_ERROR;
    }

    bool exists = fs::exists(file);
    if (exists)
    {
      std::cout << "Parsing: " << str <<std::endl;
      // call parser
      return OPTIONS_SUCCESS;
    }
    else
    {
      std::cerr << "OPTIONS_ERROR: No Such File " << str << std::endl;
    }

    return OPTIONS_ERROR;
  }
  catch (const fs::filesystem_error& ex)
  {
    std::cerr << "OPTIONS_ERROR: " << ex.what() << std::endl;
    return OPTIONS_ERROR;
  }
}

int OptionsParser::workdir(const std::string &str)
{
  //set workdir
  _finalArgs["workdir"] = str;
  return OPTIONS_SUCCESS;
}

int OptionsParser::mode(const std::string &str)
{
  if (str == "Run" || str == "Debug" || str == "Compile" || str == "Design" || str == "Rebuild")
  {
    _finalArgs["mode"] = str;
    return OPTIONS_SUCCESS;
  }
  else
    std::cerr << "OPTIONS_ERROR: invalid mode: " << str << std::endl
              << "Available Modes: " << std::endl
              << "Run" << std::endl
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
    _finalArgs["compiler"] = target;
    return OPTIONS_SUCCESS;
  }
  else
    std::cerr << "OPTIONS_ERROR: Unknown Compiler Target: " << target << std::endl
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
      _finalArgs["extensions"] += "Universal_System/Extensions/" + target + ",";
    }
    else
      _finalArgs[lower] = target;

    return OPTIONS_SUCCESS;
  }
  else
    std::cerr << "OPTIONS_ERROR: Unknown " << api << " Target: " << target << std::endl
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
  return searchAPI("Platforms", str);
}

int OptionsParser::extensions(const std::string &str)
{
  if (str == "None")
  {
    _finalArgs["extensions"] = "";
  }
  else
  {
    list_t ext = splitString(str);

    _finalArgs["extensions"] = "";
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
