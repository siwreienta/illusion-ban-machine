#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>
#include <string_view>
#include <userver/components/component_list.hpp>

namespace apotheosis {

void AppendLoadCodes(userver::components::ComponentList &component_list);
void AppendMainPage(userver::components::ComponentList &component_list);
void AppendLoadCodePage(userver::components::ComponentList &component_list);

}  // namespace apotheosis
#endif