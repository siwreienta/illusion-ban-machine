#ifndef HANDLERS_HPP_
#define HANDLERS_HPP_

#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <userver/formats/json.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include "../../algorithm/apotheosis.hpp"
#include "../../database/database.hpp"
#include "../../joern_parse/input-parcer.hpp"
#include <fmt/format.h>

namespace apotheosis {

class LoadCodePageController final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-load-code-page";
    using HttpHandlerBase::HttpHandlerBase;
    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest&,
        userver::server::request::RequestContext&) const override;
};

class LoadCodeController final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-load-code";
    LoadCodeController(
        const userver::components::ComponentConfig &config,
        const userver::components::ComponentContext &context
    )
        : HttpHandlerJsonBase(config, context) {
    }
    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest&,
        const userver::formats::json::Value& json,
        userver::server::request::RequestContext&) const override;
private:
    std::string ProcessPair(const std::string& user1, const std::string& user2,
                            int contest, int task,
                            const std::string& code1, const std::string& code2) const;
};

class ManyCodesCheck final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-get-all-codes";
    ManyCodesCheck(
        const userver::components::ComponentConfig &config,
        const userver::components::ComponentContext &context
    )
        : HttpHandlerJsonBase(config, context) {
    }
    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest&,
        const userver::formats::json::Value&,
        userver::server::request::RequestContext&) const override;
private:
    std::string ProcessBulk() const;
};

class MainPageController final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-main-page";
    using HttpHandlerBase::HttpHandlerBase;
    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest&,
        userver::server::request::RequestContext&) const override;
};

}  // namespace apotheosis
#endif
