#include "EnigmaPlugin.hpp"
#include "Main.hpp"

#include "OS_Switchboard.h"
#include "enigma.h"

EnigmaPlugin::EnigmaPlugin()
{
}

int EnigmaPlugin::Load() {
  return PLUGIN_SUCCESS;
}

const char* EnigmaPlugin::Init(CallBack *ecb, const char* enigmaRoot) {
  return libInit_path(ecb, enigmaRoot);
}

syntax_error* EnigmaPlugin::SetDefinitions(const char* def, const char* yaml) {
  return definitionsModified(def, yaml);
}

syntax_error* EnigmaPlugin::SetDefinitions(const char* yaml) {
  return definitionsModified("", yaml);
}

syntax_error* EnigmaPlugin::SyntaxCheck(int count, const char** names, const char* code) {
  return syntaxCheck(count, names, code);
}

void EnigmaPlugin::HandleGameLaunch() {
  ide_handles_game_launch();
}

void EnigmaPlugin::LogMakeToConsole() {
  log_make_to_console();
}

int EnigmaPlugin::BuildGame(deprecated::JavaStruct::EnigmaStruct* data,
                            GameMode mode, const char* fpath) {
  return compileEGMf(data, fpath, mode);
}

int EnigmaPlugin::BuildGame(const buffers::Game& data, GameMode mode, const char* fpath)
{
  buffers::Project proj;
  proj.mutable_game()->CopyFrom(data);
  return compileProto(&proj, fpath, mode);
}

const char* EnigmaPlugin::NextResource() {
  return next_available_resource();
}

const char* EnigmaPlugin::FirstResource() {
  return first_available_resource();
}

bool EnigmaPlugin::ResourceIsFunction() {
  return resource_isFunction();
}

int EnigmaPlugin::ResourceArgCountMin() {
  return resource_argCountMin();
}

int EnigmaPlugin::ResourceArgCountMax() {
  return resource_argCountMax();
}

int EnigmaPlugin::ResourceOverloadCount() {
  return resource_overloadCount();
}

const char* EnigmaPlugin::ResourceParameters(int i) {
  return resource_parameters(i);
}

int EnigmaPlugin::ResourceIsTypeName() {
  return resource_isTypeName();
}

int EnigmaPlugin::ResourceIsGlobal() {
  return resource_isGlobal();
}

bool EnigmaPlugin::ResourcesAtEnd() {
  return resources_atEnd();
}

void EnigmaPlugin::PrintBuiltins(std::string& fName) {
  std::vector<std::string> types;
  std::vector<std::string> globals;
  std::map<std::string, std::string> functions;

  const char* currentResource = FirstResource();
  while (!ResourcesAtEnd()) {

    if (ResourceIsFunction()) {
      //for (int i = 0; i < plugin_ResourceOverloadCount(); i++) // FIXME: JDI can't print overloads
        functions[currentResource] = ResourceParameters(0);
    }

    if (ResourceIsGlobal())
      globals.push_back(currentResource);

    if (ResourceIsTypeName())
      types.push_back(currentResource);

    currentResource = NextResource();
  }

  std::sort(types.begin(), types.end());

  std::ostream out(std::cout.rdbuf());
  std::filebuf fb;

  if (!fName.empty()) {
    std::cout << "Writing builtins..." << std::endl;
    fb.open(fName.c_str(), std::ios::out);
    out.rdbuf(&fb);
  }

  out << "[Types]" << std::endl;
  for (const std::string& t : types)
    out << t << std::endl;

  std::sort(globals.begin(), globals.end());

  out << "[Globals]" << std::endl;
  for (const std::string& g : globals)
    out << g << std::endl;

  out << "[Functions]" << std::endl;
  for (const auto& f : functions)
    out << f.second << std::endl;

  if (!fName.empty()) {
    fb.close();
    std::cout << "Done writing builtins" << std::endl;
  }
}
