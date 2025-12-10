#include "event_parser.h"

void LegacyEventsToEGM(buffers::resources::Object *obj, const EventData* evdata,
                       const std::map<int, NamedObject> &objs) {
  for (auto &legacy_event : *obj->mutable_legacy_events()) {
    if (!legacy_event.has_type()) {
      std::cerr << "Error: Legacy event missing type!" << std::endl;
    }
    if (legacy_event.has_name()) {
      // Try to get the event as a parameterized event by trying to find it in the events list
      // We need to find an event that has a mapping for this type and accepts string parameters
      const EventDescriptor* found_event = nullptr;
      for (const auto& ev_desc : evdata->events()) {
        // Check if this event has a reverse mapping to our legacy type
        auto legacy_pair = evdata->reverse_get_event(ev_desc);
        if (legacy_pair.mid == legacy_event.type() && ev_desc.IsParameterized()) {
          // Check if it accepts string parameters (not just integers)
          if (ev_desc.ParameterCount() > 0) {
            std::string_view param_kind = ev_desc.ParameterKind(0);
            if (param_kind != "integer") {
              found_event = &ev_desc;
              break;
            }
          }
        }
      }
      
      if (found_event) {
        // Use the string-based get_event to create the event with the name as argument
        std::vector<std::string> args = {std::string(legacy_event.name())};
        auto evd = evdata->get_event(found_event->bare_id(), args);
        if (evd.IsValid()) {
          auto &egm_event = *obj->add_egm_events();
          egm_event.set_id(evd.bare_id());
          egm_event.set_code(legacy_event.code());
          for (const auto& arg : evd.arguments) {
            egm_event.add_arguments(arg.name);
          }
          continue;
        }
      }
      
      // Fallback: try the original approach
      auto evd = evdata->get_event(legacy_event.type(), 0);
      if (evd.IsValid() && evd.IsParameterized()) {
        // Check if first parameter accepts strings
        if (evd.ParameterCount() > 0) {
          std::string_view param_kind = evd.ParameterKind(0);
          // Accept string parameters for any parameter type that isn't "integer"
          if (param_kind != "integer") {
            auto &egm_event = *obj->add_egm_events();
            egm_event.set_id(evd.bare_id());
            egm_event.add_arguments(std::string(legacy_event.name()));
            egm_event.set_code(legacy_event.code());
            continue;
          }
        }
      }
      
      // If we get here, the event cannot accept a string parameter
      std::cerr << "Error: Event " << legacy_event.type()
                << " cannot accept a string parameter." << std::endl;
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
