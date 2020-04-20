#include "gmx.h"
#include "gmk.h"
#include "yyp.h"
#include "egm.h"
#include "filesystem.h"

#include "strings_util.h"

#include <iostream>
#include <string>

struct NamedObject {
  std::string_view name;
  buffers::resources::Object* obj;
  NamedObject(): obj(nullptr) {}
  NamedObject(buffers::TreeNode *node):
    name(node->name()), obj(node->mutable_object()) {}
};

// TODO: move this into the readers instead of gm2egm main
void LegacyEventsToEGM(buffers::resources::Object *obj, const EventData &evdata,
                       const std::map<int, NamedObject> &objs) {
  for (auto &legacy_event : *obj->mutable_legacy_events()) {
    if (!legacy_event.has_type()) {
      std::cerr << "Error: Legacy event missing type!" << std::endl;
    }
    if (legacy_event.has_name()) {
      auto evd = evdata.get_event(legacy_event.type(), 0);
      if (evd.IsValid()) {
        auto &egm_event = *obj->add_egm_events();
        egm_event.set_id(evd.bare_id());
        egm_event.add_arguments(legacy_event.name());
      } else {
        std::cerr << "Error: Event " << legacy_event.type()
                  << " cannot accept a string parameter." << std::endl;
      }
    } else if (legacy_event.has_number()) {
      auto evd = evdata.get_event(legacy_event.type(), legacy_event.number());
      if (evd.IsValid()) {
        auto &egm_event = *obj->add_egm_events();
        egm_event.set_id(evd.bare_id());
        for (size_t i = 0; i < evd.arguments.size(); ++i) {
          if (evd.ParameterKind(i) == "object") {
            auto it = objs.find(legacy_event.number());
            if (it != objs.end()) {
              egm_event.add_arguments(std::string{it->second.name});
              continue;
            } else {
              std::cerr << "Event " << legacy_event.type()
                        << " references unknown object " << legacy_event.number()
                        << " (arg is " << evd.arguments[i].name << ")";
            }
          }
          // Default behavior: leave as-is (possibly integer in string).
          egm_event.add_arguments(evd.arguments[i].name);
        }
      }
    } else {
      std::cerr << "Error: Event missing sub-ID (type=" << legacy_event.type()
                << ")." << std::endl;
    }
  }
}
// void LegacyEventsToEGM(buffers::TreeNode *node, const EventData &evdata) {
//   if (node->has_object()) LegacyEventsToEGM(node->mutable_object(), evdata);
//   for (buffers::TreeNode &c : *node->mutable_child())
//     LegacyEventsToEGM(&c, evdata);
// }
void ListObjects(buffers::TreeNode *node, std::map<int, NamedObject> *out) {
  if (node->has_object()) (*out)[node->object().id()] = node;
  for (buffers::TreeNode &c : *node->mutable_child())
    ListObjects(&c, out);
}
void LegacyEventsToEGM(buffers::Project *project, const EventData &evdata) {
  std::map<int, NamedObject> objs;
  ListObjects(project->mutable_game()->mutable_root(), &objs);
  for (const auto &entry : objs) {
    LegacyEventsToEGM(entry.second.obj, evdata, objs);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: gm2egm <input> <output>" << std::endl;
    return -1;
  }
  
  const std::string outDir = argv[2];
  if (FolderExists(outDir)) {
    std::cerr << '"' << outDir << '"' << " already exists; would you like to overwrite it? (Y/N)" << std::endl;
    char c;
    std::cin >> c;
    if (c == 'y' || c == 'Y')
      DeleteFolder(outDir);
    else {
      std::cerr << "Aborting." << std::endl;
      return -5;
    }
  }

  std::string input_file = argv[1];
  std::string ext;
  size_t dot = input_file.find_last_of('.');
  if (dot != std::string::npos) ext = ToLower(input_file.substr(dot + 1));

  std::unique_ptr<buffers::Project> project;

  if (ext == "gm81" || ext == "gmk" || ext == "gm6" || ext == "gmd") {
    project = gmk::LoadGMK(input_file);
  } else if (ext == "gmx") {
    project = gmx::LoadGMX(input_file);
  } else if (ext == "yyp") {
    project = yyp::LoadYYP(input_file);
  } else {
    std::cerr << "Error: Unkown extenstion \"" << ext << "\"." << std::endl; 
    return -2;
  }

  if (project == nullptr) {
    std::cerr << "Error: Failure opening file \"" << input_file << "\"" << std::endl;
    return -3;
  }

  EventData event_data(ParseEventFile("events.ey"));
  LegacyEventsToEGM(project.get(), event_data);

  egm::EGM egm(event_data);
  if (!egm.WriteEGM(outDir, project.get())) {
    std::cerr << "Error: Failure writting \"" << argv[2] << std::endl;
    return -4;
  }

  return 0;
}
