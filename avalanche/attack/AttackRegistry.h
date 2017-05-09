#ifndef _AVALANCHE_ATTACK_REGISTRY_
#define _AVALANCHE_ATTACK_REGISTRY_

#include "AttackMethod.h"

#include <unordered_map>
#include <memory>

namespace mc {
namespace core {

class Client;

} // ns core
} // ns mc

namespace avalanche {

class AttackRegistry {
public:
    using AttackMethodCreator = std::unique_ptr<AttackMethod>(*)(mc::core::Client* client);
    using MethodRegistry = std::unordered_map<std::string, AttackMethodCreator>;

private:
    MethodRegistry m_AttackMethods;

public:
    AttackRegistry() = default;
    AttackRegistry(const MethodRegistry& registry);

    template <typename Method>
    void RegisterMethod(const std::string& name) {
        auto creator = [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { 
            return std::make_unique<Method>(client); 
        };

        m_AttackMethods.insert(std::make_pair(name, creator));
    }

    bool IsValidAttack(const std::string& method) const { return m_AttackMethods.find(method) != m_AttackMethods.end(); }
    std::unique_ptr<AttackMethod> CreateAttack(const std::string& method, mc::core::Client* client) const;
};

} // ns avalanche

#endif
