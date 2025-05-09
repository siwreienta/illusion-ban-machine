#include <fmt/format.h>
#include <filesystem>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/assert.hpp>
#include <userver/utils/daemon_run.hpp>
#include "../../database/database.hpp"
#include "../../joern_parse/input-parcer.hpp"
#include "server.hpp"

int main(int argc, char *argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>();

    apotheosis::AppendCheckStatus(component_list);
    apotheosis::AppendLoadCodes(component_list);
    apotheosis::AppendMainPage(component_list);
    return userver::utils::DaemonMain(argc, argv, component_list);
}