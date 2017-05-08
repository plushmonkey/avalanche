#include "LoginSequential.h"

#include "../Instance.h"

#include <json/json.h>
#include <iostream>
#include <mclib/util/Utility.h>

namespace avalanche {

struct login_guard : public mc::core::ConnectionListener {
    mc::core::Connection* connection;
    bool waiting;
    bool result;

    login_guard(mc::core::Connection* connection) : connection(connection), waiting(true), result(false) {
        connection->RegisterListener(this);
    }

    ~login_guard() {
        connection->UnregisterListener(this);
    }

    void OnLogin(bool success) override {
        result = success;
        waiting = false;
    }

    void OnSocketStateChange(mc::network::Socket::Status newStatus) override {
        result = false;
        waiting = false;
    }
};

LoginSequential::LoginSequential()
    : m_Delay(0)
{
    
}

std::size_t LoginSequential::Login(std::vector<Instance>& instances) {
    std::size_t total = 0;

    for (std::size_t i = 0; i < instances.size(); ++i) {
        std::string instanceUsername = m_Username;

        if (i != 0) {
            instanceUsername += std::to_string(i);

            auto time = mc::util::GetTime();
            // Update previous instances while waiting to log in the next one.
            while (mc::util::GetTime() < time + m_Delay) {
                for (std::size_t j = 0; j < i; ++j) {
                    auto connState = instances[j].GetClient()->GetConnection()->GetSocketState();

                    if (connState == mc::network::Socket::Status::Connected)
                        instances[j].GetClient()->Update();
                }
            }
        }

        try {
            std::cout << "Trying to log in to " << m_Host << ":" << m_Port << " with " << instanceUsername << ":" << m_Password << std::endl;

            if (!instances[i].GetClient()->Login(m_Host, m_Port, instanceUsername, m_Password, mc::core::UpdateMethod::Manual)) {
                std::cerr << "Failed to login with instance " << i << std::endl;
                continue;
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            continue;
        }

        login_guard guard(instances[i].GetClient()->GetConnection());

        while (guard.waiting) {
            instances[i].GetClient()->Update();
        }

        if (guard.result) {
            std::cout << "Successfully logged in instance " << i << std::endl;
            ++total;
        } else {
            std::cerr << "Failed to login with instance " << i << std::endl;
        }
    }

    return total;
}

bool LoginSequential::ReadMethodJSON(const Json::Value& node) {
    auto&& delayNode = node["delay"];

    if (delayNode.isInt())
        m_Delay = delayNode.asInt();

    return true;
}

} // ns avalanche
