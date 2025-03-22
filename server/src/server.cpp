#include "server.hpp"
#include <fmt/format.h>
#include <userver/server/handlers/http_handler_base.hpp>
#include "../../joern_parse/input-parcer.hpp"

namespace apotheosis {

namespace {

class CheckStatusController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-check-status";

    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        const auto task_id = request.GetArg("task_id");
        return apotheosis::CheckStatus(task_id);
    }
};

class LoadCodeController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-load-code";

    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &, userver::server::request::RequestContext &)
        const override {
        return apotheosis::LoadCodes();
    }
};

}  // namespace

std::string CheckStatus(std::string_view id) {
    if (id.empty()) {
        return "Error: No solution ID = banned :)\n";
    }
    return fmt::format(
        "Everything is fine, your request for a solution {} has been accepted, "
        "just wait a couple of months.\n",
        id
    );
}

std::string LoadCodes() {
    try {
        graph_maker::joern_graph_maker test1;
        graph_maker::files_stack fs1("../joern_parse/test_files");
        test1.make_graph(fs1);
        std::cout << test1.get_result_file_path("1") << std::endl;
        test1.clear_directory("../joern_parse/results");
        return "We just received the codes, it's completely plagiarized.\n";
    } catch (graph_maker::parcerer_errors &e) {
        return (e.what());
    }
}

void AppendCheckStatus(userver::components::ComponentList &component_list) {
    component_list.Append<CheckStatusController>();
}

void AppendLoadCodes(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodeController>();
}

}  // namespace apotheosis
