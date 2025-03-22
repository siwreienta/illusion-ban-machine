#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>
#include <string_view>
#include <userver/components/component_list.hpp>

namespace apotheosis {

std::string CheckStatus(std::string_view id);
std::string LoadCodes();

void AppendCheckStatus(userver::components::ComponentList &component_list);
void AppendLoadCodes(userver::components::ComponentList &component_list);

}  // namespace apotheosis
#endif