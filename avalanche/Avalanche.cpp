#include "Avalanche.h"
#include "Factory.h"
#include "login/LoginFlood.h"
#include "login/LoginSequential.h"
#include "attack/AttackInteract.h"
#include "attack/AttackBookEdit.h"
#include "attack/AttackCreativeWorldLag.h"

#include <fstream>
#include <memory>
#include <iostream>

namespace avalanche {

using AttackFactory = Factory<AttackMethod, mc::core::Client*>;
static const AttackFactory attackFactory = AttackFactory::MethodRegistry {
    { AttackInteract::s_Name, [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { return std::make_unique<AttackInteract>(client); } },
    { AttackBookEdit::s_Name, [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { return std::make_unique<AttackBookEdit>(client); } },
    { AttackCreativeWorldLag::s_Name, [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { return std::make_unique<AttackCreativeWorldLag>(client); } },
};

using LoginFactory = Factory<LoginMethod>;
static const LoginFactory loginFactory = LoginFactory::MethodRegistry {
    { LoginFlood::s_Name, []() -> std::unique_ptr<LoginMethod> { return std::make_unique<LoginFlood>(); } },
    { LoginSequential::s_Name, []() -> std::unique_ptr<LoginMethod> { return std::make_unique<LoginSequential>(); } },
};

void Avalanche::Run() {
    s32 activeInstances = m_LoginMethod->Login(m_Instances);

    std::vector<bool> instanceStatus(activeInstances, true);

    std::cout << "Successfully logged in " << activeInstances << "/" << m_Instances.size() << " instances." << std::endl;

    while (activeInstances > 0) {
        for (std::size_t i = 0; i < m_Instances.size(); ++i) {
            auto&& instance = m_Instances[i];

            if (!instanceStatus[i]) continue;

            auto state = instance.GetClient()->GetConnection()->GetSocketState();
            if (state != mc::network::Socket::Status::Connected) {
                instanceStatus[i] = false;
                --activeInstances;

                instance.SetAttackMethod(nullptr);

                std::cout << "Instance was kicked from server. " << activeInstances << " instances remaining." << std::endl;
                continue;
            }

            instance.GetClient()->Update();
        }
    }
}

bool Avalanche::Initialize(const OptionMap& options) {
    auto jsonIter = options.find("json");
    Json::Value attackNode;
    std::string attack;
    std::size_t count = 1;

    auto attackIter = options.find("attack");
    if (attackIter != options.end())
        attack = attackIter->second;

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

            m_LoginMethod = loginFactory.Create(methodName);

            if (m_LoginMethod) {
                if (!m_LoginMethod->ReadJSON(loginNode)) {
                    std::cerr << "Failed to read login method JSON." << std::endl;
                    return false;
                }
            }
        }

        if (attack.empty()) {
            attackNode = root["attack"];
            if (attackNode.isObject()) {
                auto&& methodNode = attackNode["method"];
                if (methodNode.isString()) {
                    attack = methodNode.asString();
                }
            }
        }
    }

    if (m_LoginMethod == nullptr) {
        m_LoginMethod = loginFactory.Create(LoginFlood::s_Name);
    }

    if (!m_LoginMethod->ReadOptions(options)) {
        return false;
    }

    auto countIter = options.find("count");
    if (countIter != options.end())
        count = strtol(countIter->second.c_str(), nullptr, 10);

    m_Instances = std::vector<Instance>(count);

    if (attackFactory.Contains(attack))
        std::cout << "attack: " << attack << std::endl;
    else
        std::cout << "attack: none" << std::endl;

    for (auto&& instance : m_Instances) {
        instance.GetClient()->GetConnection()->GetSettings().SetLocale(L"en_GB");

        if (!attack.empty()) {
            std::unique_ptr<AttackMethod> attackMethod = attackFactory.Create(attack, instance.GetClient());

            if (attackMethod && !attackNode.isNull()) {
                attackMethod->ReadJSON(attackNode);
            }

            instance.SetAttackMethod(std::move(attackMethod));
        }
    }

    return true;
}

} // ns avalanche
