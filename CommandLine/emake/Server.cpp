#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 // at least windows vista required for grpc
#endif

#include "Server.hpp"
#include "eyaml/eyaml.h"

#include "server.grpc.pb.h"

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>

#include <filesystem>
#include <memory>
#include <future>
#include <vector>

namespace fs = std::filesystem;

using namespace grpc;
using namespace buffers;

class CompilerServiceImpl final : public Compiler::Service {
  public:
  explicit CompilerServiceImpl(EnigmaPlugin& plugin, OptionsParser& options, CallBack &ecb):
    plugin(plugin), options(options), ecb(ecb) {}

  Status CompileBuffer(ServerContext* /*context*/, const CompileRequest* request, ServerWriter<CompileReply>* writer) override {
    // use lambda capture to contain compile logic
    auto fnc = [&] {
      plugin.BuildGame(request->game(), emode_run, request->name().c_str());
    };
    // asynchronously launch the compile request
    std::future<void> future = std::async(fnc);

    // provide compile feedback to the client
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      CompileReply reply;

      ecb.ProcessOutput();

      reply.mutable_progress()->CopyFrom(ecb.GetProgress());

      bool end = false;
      const LogMessage msg = ecb.GetFirstLogMessage(end);
      if (end) continue;
      reply.add_message()->CopyFrom(msg);
      while (true) {
        const LogMessage nxt = ecb.GetNextLogMessage(end);
        if (end) { ecb.ClearLogMessages(); break; }
        reply.add_message()->CopyFrom(nxt);
      }

      writer->Write(reply);
    }

    return Status::OK;
  }

  Status GetResources(ServerContext* /*context*/, const Empty* /*request*/, ServerWriter<Resource>* writer) override {
    const char* raw = plugin.FirstResource();
    while (!plugin.ResourcesAtEnd()) {
      Resource resource;

      resource.set_name(raw);
      resource.set_is_function(plugin.ResourceIsFunction());
      if (resource.is_function()) {
        resource.set_arg_count_min(plugin.ResourceArgCountMin());
        resource.set_arg_count_max(plugin.ResourceArgCountMax());
        resource.set_overload_count(plugin.ResourceOverloadCount());
      }
      resource.set_is_type_name(plugin.ResourceIsTypeName());
      resource.set_is_global(plugin.ResourceIsGlobal());

      for (int i = 0; i < resource.overload_count(); ++i) {
        resource.add_parameters(plugin.ResourceParameters(i));
      }

      if (!plugin.ResourcesAtEnd()) {
        resource.set_is_end(false);
        raw = plugin.NextResource();
      } else {
        resource.set_is_end(true);
      }

      writer->Write(resource);
    }

    return Status::OK;
  }

  Status GetSystems(ServerContext* /*context*/, const Empty* /*request*/, ServerWriter<SystemType>* writer) override {
    auto _api = this->options.GetAPI();

    for (auto systems : _api) {
      SystemType system;

      system.set_name(systems.first);

      for (auto&& subsystem : systems.second) {
        SystemInfo* subInfo = system.add_subsystems();

        std::ifstream ifabout(subsystem, std::ios_base::in);
        if (!ifabout.is_open()) continue;

        ey_data about = parse_eyaml(ifabout, subsystem);

        std::string name = about.get("name");
        std::string id = about.get("identifier");
        std::string desc = about.get("description");
        std::string author = about.get("author");
        std::string target = about.get("target-platform");

        if (id.empty())
          id = about.get("id"); // allow alias
        if (id.empty()) {
          // compilers use filename minus ext as id
          fs::path ey(subsystem);
          id = ey.stem().string();
        }

        // allow author alias used by compiler descriptors
        if (author.empty())
          author = about.get("maintainer");

        subInfo->set_name(name);
        subInfo->set_id(id);
        subInfo->set_description(desc);
        subInfo->set_author(author);
        subInfo->set_target(target);
      }

      writer->Write(system);
    }

    return Status::OK;
  }

  SyntaxError GetSyntaxError(syntax_error* err) {
    SyntaxError error;
    error.set_message(err->err_str);
    error.set_line(err->line);
    error.set_position(err->position);
    error.set_absolute_index(err->absolute_index);
    return error;
  }

  Status SetDefinitions(ServerContext* /*context*/, const SetDefinitionsRequest* request, SyntaxError* reply) override {
    syntax_error* err = plugin.SetDefinitions(request->code().c_str(), request->yaml().c_str());
    reply->CopyFrom(GetSyntaxError(err));
    return Status::OK;
  }

  Status SetCurrentConfig(ServerContext* /*context*/, const SetCurrentConfigRequest* request, Empty* /*reply*/) override {
    std::string yaml = this->options.APIyaml(&request->settings());
    /*syntax_error* err = */plugin.SetDefinitions("", yaml.c_str());
    return Status::OK;
  }

  Status SyntaxCheck(ServerContext* /*context*/, const SyntaxCheckRequest* request, SyntaxError* reply) override {
    std::vector<const char*> script_names;
    script_names.reserve(request->script_names().size());
    for (const std::string &str : request->script_names()) script_names.push_back(str.c_str());
    syntax_error* err = plugin.SyntaxCheck(request->script_count(), script_names.data(), request->code().c_str());
    reply->CopyFrom(GetSyntaxError(err));
    return Status::OK;
  }

  private:
  EnigmaPlugin& plugin;
  OptionsParser& options;
  CallBack &ecb;
};

int RunServer(const std::string& address, EnigmaPlugin& plugin, OptionsParser &options, CallBack &ecb) {
  CompilerServiceImpl service(plugin, options, ecb);

  ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << address << std::endl;
  server->Wait();
  return 0;
}
