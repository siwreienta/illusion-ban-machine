#include "server.hpp"
#include "handlers.hpp"

namespace apotheosis {
void AppendMainPage(userver::components::ComponentList &component_list) {
    component_list.Append<MainPageController>();
}

void AppendLoadCodes(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodeController>();
}

void AppendLoadCodePage(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodePageController>();
}

void AppendManyCheck(userver::components::ComponentList &component_list) {
    component_list.Append<ManyCodesCheck>();
}
}  // namespace apotheosis
