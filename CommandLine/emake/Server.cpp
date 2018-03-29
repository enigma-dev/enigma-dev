#include "Server.hpp"

#define _WIN32_WINNT 0x0600
#include "server.grpc.pb.h"

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>

#include <memory>

using namespace grpc;
using namespace buffers;

class CompilerServiceImpl final : public Compiler::Service {
  public:
  explicit CompilerServiceImpl(EnigmaPlugin& plugin): plugin(plugin) {}

  Status CompileBuffer(ServerContext* /*context*/, const CompileRequest* request, ServerWriter<CompileReply>* writer) override {
    plugin.BuildGame(const_cast<buffers::Game*>(&request->game()), emode_run, request->name().c_str());
    CompileReply reply;
    reply.set_message("hello");
    reply.set_progress(0);
    writer->Write(reply);
    return Status::OK;
  }

  Status GetResources(ServerContext* /*context*/, const Empty* request, ServerWriter<Resource>* writer) override {
    const char* raw = plugin.FirstResource();
    while (raw != nullptr) {
      Resource resource;

      resource.set_value(raw);
      resource.set_is_function(plugin.ResourceIsFunction());
      resource.set_arg_count_min(plugin.ResourceArgCountMin());
      resource.set_arg_count_max(plugin.ResourceArgCountMax());
      resource.set_overload_count(plugin.ResourceOverloadCount());
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

  private:
  EnigmaPlugin& plugin;
};

int RunServer(const std::string& address, EnigmaPlugin& plugin) {
  CompilerServiceImpl service(plugin);

  ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << address << std::endl;
  server->Wait();
  return 0;
}
