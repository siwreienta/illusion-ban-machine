#include "server.hpp"
#include <fmt/format.h>
#include <userver/server/handlers/http_handler_base.hpp>

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
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        const auto code = request.GetArg("code");
        return apotheosis::LoadCodes(code);
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

std::string LoadCodes(std::string_view code) {
    if (code.empty()) {
        return "Error: The code itself is missing = banned :)\n";
    }
    return fmt::format(
        "We just received the code '{}', it's completely plagiarized.\n", code
    );
}

void AppendCheckStatus(userver::components::ComponentList &component_list) {
    component_list.Append<CheckStatusController>();
}

void AppendLoadCodes(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodeController>();
}

}  // namespace apotheosis
