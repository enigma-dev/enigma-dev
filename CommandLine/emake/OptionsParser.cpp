#include "OptionsParser.hpp"
#include "Main.hpp"

#include "eyaml/eyaml.h"
#include "strings_util.h"
#include "ProtoYaml/proto-yaml.h"
#include "OS_Switchboard.h"

#include "apifilesystem/filesystem.hpp"
namespace fs = ghc::filesystem;

#include <iostream>
#include <cctype> // std::ispace

using namespace buffers::resources;

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

inline std::string repeatedStringToDelimit(const google::protobuf::RepeatedPtrField<std::string> f) {
  std::string ret;
  for (auto s : f) {
    ret += s + ",";
  }
  
  if (!ret.empty()) ret.pop_back();
  return ret;
}

OptionsParser::OptionsParser() : _desc("Options")
{
  // Platform Defaults
  std::string def_platform, def_workdir, def_compiler;
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    def_platform = "Win32";
    def_workdir = std::string(getenv("LOCALAPPDATA")) + "/ENIGMA/";
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
  
  fs::path defaultConfig = fs::current_path()/"emake-defaults.yaml";
  if (fs::exists(defaultConfig)) _loadedSettings = egm::ReadYamlFileAs<Settings>(defaultConfig.u8string());
  const API& defAPI = _loadedSettings.api();
  const Compiler& defComp = _loadedSettings.compiler();
  
  _desc.add_options()
    ("help,h", "Print help messages")
    ("list,l", "List available types, globals & functions")
    ("info,i", opt::value<std::string>(), "Provides a listing of Platforms, APIs and Extensions")
    ("input",   opt::value<std::string>()->default_value(""), "Input game file; currently, only test harness single-object games (*.sog) are supported. The --input string is optional.")
    ("config",   opt::value<std::string>()->default_value(""), "Configuration file for emake. Note: All passeed options will override any option set in the config")
    ("quiet,q", opt::bool_switch()->default_value(false), "Suppresses output to std::out and std::err streams.")
    ("server,s", opt::bool_switch()->default_value(false), "Starts the CLI in server mode (ignores input file).")
    ("ip", opt::value<std::string>()->default_value("localhost"), "The ip address of the server when running in server mode.")
    ("port", opt::value<int>()->default_value(37818), "The port number to bind when in server mode.")
    ("output,o", opt::value<std::string>(), "Output executable file")
    ("platform,p", opt::value<std::string>()->default_value(defAPI.has_target_platform() ? defAPI.target_platform() : def_platform), "Target Platform (Win32, xlib, Cocoa, SDL, None)")
    ("workdir,d", opt::value<std::string>()->default_value(defComp.has_eobjs_directory() ? defComp.eobjs_directory() : def_workdir), "Working Directory")
    ("codegen,k", opt::value<std::string>()->default_value(defComp.has_codegen_directory() ? defComp.codegen_directory() : def_workdir), "Codegen Directory")
    ("mode,m", opt::value<std::string>()->default_value("Debug"), "Game Mode (Run, Compile, Debug, Design)")
    ("graphics,g", opt::value<std::string>()->default_value(defAPI.has_target_graphics() ? defAPI.target_graphics() : "OpenGL3"), "Graphics System (Direct3D9, Direct3D11, OpenGL1, OpenGL3, OpenGLES2, OpenGLES3, None)")
    ("audio,a", opt::value<std::string>()->default_value(defAPI.has_target_audio() ? defAPI.target_audio() : "None"), "Audio System (DirectSound, OpenAL, XAudio2, None)")
    ("widgets,w", opt::value<std::string>()->default_value(defAPI.has_target_widgets() ? defAPI.target_widgets() : "None"), "Widget System (Win32, xlib, Cocoa, GTK+, None)")
    ("network,n", opt::value<std::string>()->default_value(defAPI.has_target_network() ? defAPI.target_network() : "None"), "Networking System (DirectPlay, Asynchronous, BerkeleySockets, None)")
    ("collision,c", opt::value<std::string>()->default_value(defAPI.has_target_collision() ? defAPI.target_collision() : "None"), "Collision System (Precise, BBox, None)")
    ("extensions,e", opt::value<std::string>()->default_value(defAPI.extensions_size() > 0 ? repeatedStringToDelimit(defAPI.extensions()) : "None"), "Extensions (Alarms, Paths, Timelines, Particles, MotionPlanning, ttf, libpng, IniFilesystem, RegistrySpoof, Asynchronous, StudioPhysics, VirtualKeys, XRandR, XTEST, FileDropper, None)")
    ("compiler,x", opt::value<std::string>()->default_value(defAPI.has_target_compiler() ? defAPI.target_compiler() : def_compiler), "Compiler.ey Descriptor")
    ("enigma-root", opt::value<std::string>()->default_value(fs::current_path().string()), "Path to ENIGMA's sources")
    ("codegen-only", opt::bool_switch()->default_value(false), "Only generate code and exit")
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
               
    _enigmaRoot = fs::absolute(_rawArgs["enigma-root"].as<std::string>()).string();
    
    if (!_enigmaRoot.empty() && _enigmaRoot.back() != '/') _enigmaRoot += "/";

    if (!_rawArgs.count("info"))
      opt::notify(_rawArgs);
      
    if (!_rawArgs.count("help") && !_rawArgs.count("list") && !_rawArgs.count("info") && !_rawArgs.count("server") && !_rawArgs.count("output")) {
      throw std::logic_error("Option 'help', 'list', 'info', 'server', or option 'output' is required.");
    }
  }
  catch(std::exception& e)
  {
    if (!_rawArgs.count("help"))
      std::cerr << "OPTIONS_ERROR: " << e.what() << std::endl << std::endl;

    _readArgsFail = true;

    return OPTIONS_ERROR;
  }
  
  std::string config = _rawArgs["config"].as<std::string>();
  if (!config.empty()) {
    if (fs::exists(config)) _loadedSettings = egm::ReadYamlFileAs<Settings>(config);
    else { 
      std::cerr << "Error: Configuration file: \"" << config << "\" does not exists" << std::endl;
      return OPTIONS_ERROR;
    }
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

std::string OptionsParser::APIyaml(const buffers::resources::Settings* currentConfig)
{
  std::string audio = _rawArgs["audio"].as<std::string>();
  std::string platform = _rawArgs["platform"].as<std::string>();
  std::string compiler = _rawArgs["compiler"].as<std::string>();
  std::string graphics = _rawArgs["graphics"].as<std::string>();
  std::string widgets = _rawArgs["widgets"].as<std::string>();
  std::string collision = _rawArgs["collision"].as<std::string>();
  std::string network = _rawArgs["network"].as<std::string>();
  
  std::string eobjs_directory = fs::absolute(_rawArgs["workdir"].as<std::string>()).string();
  std::string codegen_directory = fs::absolute(_rawArgs["codegen"].as<std::string>()).string(); 
  
  // Only override passed arguments if passed a configuration
  if (currentConfig != nullptr) {
    const auto &api = currentConfig->api();
    if (api.has_target_audio()) audio = api.target_audio();
    if (api.has_target_platform()) platform = api.target_platform();
    if (api.has_target_compiler()) compiler = api.target_compiler();
    if (api.has_target_graphics()) graphics = api.target_graphics();
    if (api.has_target_widgets()) widgets = api.target_widgets();
    if (api.has_target_collision()) collision = api.target_collision();
    if (api.has_target_network()) network = api.target_network();
    
    const auto &compilerSettings = currentConfig->compiler();
    if (compilerSettings.has_eobjs_directory()) eobjs_directory = compilerSettings.eobjs_directory();
    if (compilerSettings.has_codegen_directory()) codegen_directory = compilerSettings.codegen_directory();
  }

  // No emake flags to worry about for these
  if (currentConfig == nullptr) currentConfig = &_loadedSettings;
  
  const auto &compilerSettings = currentConfig->compiler();
  int inherit_strings = compilerSettings.has_inherit_strings() ? compilerSettings.inherit_strings() : 0;
  int inherit_escapes = compilerSettings.inherit_escapes() ? compilerSettings.inherit_escapes() : 0;
  int inherit_increment = compilerSettings.has_inherit_increment() ? compilerSettings.inherit_increment() : 0;
  int inherit_equivalence = compilerSettings.inherit_equivalence() ? compilerSettings.inherit_equivalence() : 0;
  int inherit_literals = compilerSettings.has_inherit_literals() ? compilerSettings.inherit_literals() : 0;
  int inherit_negatives = compilerSettings.has_inherit_negatives() ? compilerSettings.inherit_negatives() : 0;
  bool inherit_objects = compilerSettings.has_inherit_objects() ? compilerSettings.inherit_objects() : 0;
  bool automatic_semicolons = compilerSettings.has_automatic_semicolons() ? compilerSettings.automatic_semicolons() : 0;

  std::string yaml;
  yaml += "%e-yaml\n";
  yaml += "---\n";
  yaml += "treat-literals-as: " + std::to_string(inherit_literals) + "\n";
  yaml += "sample-lots-of-radios: 0\n";
  yaml += "inherit-equivalence-from: " + std::to_string(inherit_equivalence) + "\n";
  yaml += "eobjs-directory: " + eobjs_directory + "\n";
  yaml += "codegen-directory: " + codegen_directory + "\n";
  yaml += "sample-checkbox: on\n";
  yaml += "sample-edit: DEADBEEF\n";
  yaml += "sample-combobox: 0\n";
  yaml += "inherit-strings-from: " + std::to_string(inherit_strings) + "\n";
  yaml += "inherit-negatives-as: " + std::to_string(inherit_negatives) + "\n";
  yaml += "inherit-escapes-from: " + std::to_string(inherit_escapes) + "\n";
  yaml += "inherit-increment-from: " + std::to_string(inherit_increment) + "\n";
  yaml += "inherit-objects: " + std::string(inherit_objects ? "true" : "false") + "\n";
  yaml += "automatic-semicolons: " + std::string(automatic_semicolons ? "true" : "false") + "\n";
  yaml += " \n";
  yaml += "target-audio: " + audio + "\n";
  yaml += "target-windowing: " + platform + "\n";
  yaml += "target-compiler: " + compiler + "\n";
  yaml += "target-graphics: " + graphics + "\n";
  yaml += "target-widget: " + widgets + "\n";
  yaml += "target-collision: " + collision + "\n";
  yaml += "target-networking: " + network + "\n";
  yaml += "extensions: " + _extensions + "\n";
  yaml += std::string("codegen-only: ") + (_rawArgs["codegen-only"].as<bool>() ? "true" : "false") + "\n";
  yaml += "enigma-root: " + _enigmaRoot + "\n";

  return yaml;
}

int OptionsParser::find_ey(const char* dir)
{
  fs::path targetDir(_enigmaRoot);
  targetDir /= dir;
  fs::recursive_directory_iterator iter(targetDir);

  for(auto& p : iter)
  {
    fs::path i = p.path();
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

const APIMap& OptionsParser::GetAPI() const
{
  return _api;
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

        if (id.empty())
          id = about.get("id"); // allow alias
        if (id.empty()) {
          // compilers use filename minus ext as id
          fs::path ey(p);
          id = ey.stem().string();
        }

        if (!name.empty() && !id.empty()) {
          outputStream << '\t' << name << " (" << id << "):" << std::endl;
          std::cout    << '\t' << name << " (" << id << "):" << std::endl;
        }

        if (!target.empty())
          outputStream << "\t\t Target: " << target << std::endl << std::endl;
        else if (!desc.empty())
          outputStream << "\t\t" << word_wrap(desc, 80) << std::endl << std::endl;
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
  outputStream << "Enigma Command Line Compiler" << std::endl << _desc << std::endl;
  std::cout    << "Enigma Command Line Compiler" << std::endl << _desc << std::endl;
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
      std::cerr << "OPTIONS_ERROR: " << str << " Is a Directory!" << std::endl;
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

int OptionsParser::mode(const std::string &str)
{
  if (str == "Run" || str == "Debug" || str == "Compile" || str == "Design" || str == "Rebuild")
  {
    return OPTIONS_SUCCESS;
  }
  else
    std::cerr << "OPTIONS_ERROR: invalid mode: " << str << std::endl
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
    fs::path ey(a);
    return ey.stem().string() == target;
  });

  if (it != std::end(_api["Compilers"]))
  {
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
    if (id.empty())
      id = about.get("id"); // allow alias
    return (id == target);
  });

  if (it != std::end(_api[api]))
  {
    //set api
    std::string lower = ToLower(api);

    if (lower == "extensions")
    {
      _extensions += "Universal_System/Extensions/" + target + ",";
    }

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
