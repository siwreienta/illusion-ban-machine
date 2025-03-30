#include "server.hpp"
#include <fmt/format.h>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_response_body_stream.hpp>
#include "../../joern_parse/input-parcer.hpp"
#include <functional>

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
        auto& response = request.GetHttpResponse();
        response.SetContentType("text/html");
        return apotheosis::CheckStatus(task_id);
    }
};

class LoadCodeController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-load-code";

    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        auto& response = request.GetHttpResponse();
        response.SetContentType("text/html");
        return fmt::format("<html><body><h1>{}</h1></body></html>", apotheosis::LoadCodes());
    }
};

}  // namespace

std::string CheckStatus(std::string_view id) {
    if (id.empty()) {
        return "<html><body><h1>Error: No solution ID = banned :)</h1></body></html>";
    }
    return fmt::format(
        "<html><body><h1>Everything is fine, your request for a solution {} has been accepted, "
        "just wait a couple of months</h1></body></html>",
        id
    );
}

std::string LoadCodes() {
    try {
        graph_maker::joern_graph_maker joern_parser;
        graph_maker::files_stack file_stack("../joern_parse/test_files");
        joern_parser.make_graph(file_stack);
        // std::cout << joern_parser.get_result_file_path("1") << std::endl;
        joern_parser.clear_directory("../joern_parse/results");
        return "We just received the codes, it's completely plagiarized.";
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
