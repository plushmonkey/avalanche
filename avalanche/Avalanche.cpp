#include "Avalanche.h"
#include "Factory.h"
#include "login/LoginFlood.h"
#include "VersionDetector.h"

#include <fstream>
#include <memory>
#include <iostream>

namespace avalanche {

void Avalanche::Run() {
    s32 activeInstances = m_LoginMethod->Login(m_Instances);

    std::cout << "Successfully logged in " << activeInstances << "/" << m_Instances.size() << " instances." << std::endl;

    while (activeInstances > 0) {
        for (auto&& instance : m_Instances) {
            if (!instance->Update()) {
                --activeInstances;
                std::cout << "Instance was kicked from server. " << activeInstances << " instances remaining." << std::endl;
            }
        }
    }
}

bool Avalanche::Initialize(const OptionMap& options) {
    auto jsonIter = options.find("json");
    Json::Value behaviorNode;
    std::string behaviorMethod;
    std::size_t count = 1;

    auto behaviorIter = options.find("behavior");
    if (behaviorIter != options.end())
        behaviorMethod = behaviorIter->second;

    if (jsonIter != options.end()) {
        std::string jsonFile = jsonIter->second;

        Json::Reader reader;
        Json::Value root;

        std::ifstream in(jsonFile);

        if (!in.is_open())
            throw std::runtime_error("Failed to read JSON file " + jsonFile);

        if (!reader.parse(in, root))
            throw std::runtime_error("Failed to parse JSON file " + jsonFile);

        auto&& countNode = root["count"];
        if (countNode.isInt())
            count = countNode.asInt();

        auto&& loginNode = root["login"];
        if (loginNode.isObject()) {
            auto&& methodNode = loginNode["method"];
            std::string methodName = LoginFlood::s_Name;

            if (methodNode.isObject()) {
                if (methodNode["name"].isString())
                    methodName = methodNode["name"].asString();
            }

            m_LoginMethod = g_LoginFactory.Create(methodName);

            if (m_LoginMethod) {
                if (!m_LoginMethod->ReadJSON(loginNode)) {
                    std::cerr << "Failed to read login method JSON." << std::endl;
                    return false;
                }
            }
        }

        if (behaviorMethod.empty()) {
            behaviorNode = root["behavior"];
            if (behaviorNode.isObject()) {
                auto&& methodNode = behaviorNode["method"];
                if (methodNode.isString()) {
                    behaviorMethod = methodNode.asString();
                }
            }
        }
    }

    if (m_LoginMethod == nullptr) {
        m_LoginMethod = g_LoginFactory.Create(LoginFlood::s_Name);
    }

    if (!m_LoginMethod->ReadOptions(options)) {
        return false;
    }

    auto countIter = options.find("count");
    if (countIter != options.end())
        count = strtol(countIter->second.c_str(), nullptr, 10);

    VersionDetector versionDetector(m_LoginMethod->GetHost(), m_LoginMethod->GetPort());
    auto version = versionDetector.GetVersion();

    m_Instances.reserve(count);

    for (std::size_t i = 0; i < count; ++i)
        m_Instances.emplace_back(std::make_unique<Instance>(version));

    if (g_BehaviorFactory.Contains(behaviorMethod))
        std::cout << "behavior: " << behaviorMethod << std::endl;
    else
        std::cout << "behavior: none" << std::endl;

    for (auto&& instance : m_Instances) {
        if (!behaviorMethod.empty()) {
            std::unique_ptr<Behavior> behavior = g_BehaviorFactory.Create(behaviorMethod, instance->GetClient());

            if (behavior && !behaviorNode.isNull()) {
                behavior->ReadJSON(behaviorNode);
            }

            if (behavior)
                behavior->OnCreate();

            instance->SetBehavior(std::move(behavior));
        }
    }

    return true;
}

} // ns avalanche
