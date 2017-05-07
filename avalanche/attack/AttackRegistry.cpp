#include "AttackRegistry.h"

#include <algorithm>
#include <cctype>

namespace avalanche {

AttackRegistry::AttackRegistry(const MethodRegistry& registry) {
    m_AttackMethods = registry;
}

std::unique_ptr<AttackMethod> AttackRegistry::CreateAttack(const std::string& method, mc::core::Client* client) const {
    std::string lcMethod = method;

    std::transform(lcMethod.begin(), lcMethod.end(), lcMethod.begin(), std::tolower);

    auto iter = m_AttackMethods.find(lcMethod);

    if (iter == m_AttackMethods.end())
        return nullptr;

    auto creator = iter->second;

    return creator(client);
}

} // ns avalanche
