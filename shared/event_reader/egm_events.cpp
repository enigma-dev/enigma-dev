#include "event_parser.h"

void LegacyEventsToEGM(buffers::resources::Object *obj, const EventData* evdata,
                       const std::map<int, NamedObject> &objs) {
  for (auto &legacy_event : *obj->mutable_legacy_events()) {
    if (!legacy_event.has_type()) {
      std::cerr << "Error: Legacy event missing type!" << std::endl;
    }
    if (legacy_event.has_name()) {
      auto evd = evdata->get_event(legacy_event.type(), 0);
      if (evd.IsValid()) {
        auto &egm_event = *obj->add_egm_events();
        egm_event.set_id(evd.bare_id());
        egm_event.add_arguments(legacy_event.name());
        egm_event.set_code(legacy_event.code());
      } else {
        std::cerr << "Error: Event " << legacy_event.type()
                  << " cannot accept a string parameter." << std::endl;
      }
    } else if (legacy_event.has_number()) {
      auto evd = evdata->get_event(legacy_event.type(), legacy_event.number());
      if (evd.IsValid()) {
        auto &egm_event = *obj->add_egm_events();
        egm_event.set_id(evd.bare_id());
        egm_event.set_code(legacy_event.code());
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
  std::cout << "Converted " << obj->legacy_events_size() << " legacy events into " << obj->egm_events_size() << " EGM events.\n";
  
  // Shouldn't need legacy events anymore
  obj->clear_legacy_events();
}

// void LegacyEventsToEGM(buffers::TreeNode *node, const EventData &evdata) {
//   if (node->has_object()) LegacyEventsToEGM(node->mutable_object(), evdata);
//   for (buffers::TreeNode &c : *node->mutable_child())
//     LegacyEventsToEGM(&c, evdata);
// }

void ListObjects(buffers::TreeNode *node, std::map<int, NamedObject> *out) {
  if (node->has_object()) (*out)[node->object().id()] = NamedObject(node->name(), node->mutable_object());
  if (node->has_folder()) {
    for (buffers::TreeNode &c : *node->mutable_folder()->mutable_children())
      ListObjects(&c, out);
  }
}

void LegacyEventsToEGM(buffers::Project *project, const EventData* evdata) {
  std::map<int, NamedObject> objs;
  ListObjects(project->mutable_game()->mutable_root(), &objs);
  for (const auto &entry : objs) {
    LegacyEventsToEGM(entry.second.obj, evdata, objs);
  }
}
