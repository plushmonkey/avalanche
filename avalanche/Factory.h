#ifndef _AVALANCHE_FACTORY_H_
#define _AVALANCHE_FACTORY_H_

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cctype>

namespace avalanche {

template <typename T, typename... Args>
class Factory {
public:
    using MethodCreator = std::unique_ptr<T>(*)(Args... args);
    using MethodRegistry = std::unordered_map<std::string, MethodCreator>;

private:
    MethodRegistry m_Methods;

public:
    Factory() = default;

    Factory(const MethodRegistry& registry) {
        m_Methods = registry;
    }

    template <typename Method>
    void Register(const std::string& name) {
        auto creator = [](Args... args) -> std::unique_ptr<T> {
            return std::make_unique<Method>(args...);
        };

        m_Methods.insert(std::make_pair(name, creator));
    }

    bool Contains(const std::string& name) const { return m_Methods.find(name) != m_Methods.end(); }

    std::unique_ptr<T> Create(std::string name, Args... args) const {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        auto iter = m_Methods.find(name);
        if (iter == m_Methods.end())
            return nullptr;

        return iter->second(args...);
    }
};

} // ns avalanche

#endif
