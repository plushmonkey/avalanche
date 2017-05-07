#include "getopt.h"

#include <mclib/core/Client.h>
#include <mclib/util/Utility.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <cstdio>
#include <cstdlib>

void DisplayUsage();

class AttackMethod {
public:
    virtual ~AttackMethod() { }
};

class AttackInteraction : public AttackMethod, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;

public:
    AttackInteraction(mc::core::Client* client) : m_Client(client) {
        client->RegisterListener(this);
    }

    ~AttackInteraction() {
        m_Client->UnregisterListener(this);
    }

    void OnTick() override {
        using mc::Vector3i;

        const Vector3i offset(0, 2, 0);

        auto controller = m_Client->GetPlayerController();
        auto world = m_Client->GetWorld();

        mc::Vector3i target = mc::ToVector3i(controller->GetPosition()) + offset;

        if (target == offset) return;
        if (!world->GetChunk(target)) return;

        controller->LookAt(mc::ToVector3d(target));

        mc::block::BlockPtr block = world->GetBlock(target + Vector3i(0, 1, 0)).GetBlock();

        if (block == nullptr) {
            return;
        }

        using namespace mc::protocol::packets::out;

        static const s32 DigsPerTick = 100;

        for (s32 i = 0; i < DigsPerTick; ++i) {
            PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::StartedDigging;
            PlayerDiggingPacket packet(status, target + Vector3i(0, 1, 0), mc::Face::West);

            m_Client->GetConnection()->SendPacket(&packet);

            status = PlayerDiggingPacket::Status::FinishedDigging;
            packet = PlayerDiggingPacket(status, target + Vector3i(0, 1, 0), mc::Face::West);

            m_Client->GetConnection()->SendPacket(&packet);
        }
    }

};

using AttackMethodCreator = std::unique_ptr<AttackMethod> (*)(mc::core::Client* client);

static const std::unordered_map<std::string, AttackMethodCreator> attackMethods = {
    { "interact", [](mc::core::Client* client) -> std::unique_ptr<AttackMethod> { return std::make_unique<AttackInteraction>(client); } }
};

struct BotInstance {
    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::protocol::Version version;
    mc::core::Client client;
    std::unique_ptr<AttackMethod> attack;
    bool active;
    
    BotInstance() 
        : dispatcher(),
          version(mc::protocol::Version::Minecraft_1_11_2),
          client(&dispatcher, version),
          active(false)
    {

    }
};

int main(int argc, char* argv[]) {
    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks();

    std::string username;
    std::string password;
    std::string host;
    u16 port = 25565;
    s32 count = 1;
    std::string attack;
    AttackMethodCreator attackCreator = nullptr;

    while (1) {
        static struct option long_options[] = {
            { "username", required_argument, 0, 0 },
            { "password", required_argument, 0, 0 },
            { "server", required_argument, 0, 0 },
            { "count", required_argument, 0, 0 },
            { "attack", required_argument, 0, 0 },
        };

        int option_index = 0;
        int c;

        c = getopt_long(argc, argv, ":", long_options, &option_index);

        if (c == -1) 
            break;

        if (c == 0) {
            std::string option_name = long_options[option_index].name;
            
            if (!optarg) 
                continue;

            if (option_name == "username") {
                username = optarg;
            } else if (option_name == "password") {
                password = optarg;
            } else if (option_name == "server") {
                host = optarg;

                auto pos = host.find(':');
                if (pos != std::string::npos) {
                    port = atoi(host.substr(pos + 1).c_str());
                    host = host.substr(0, pos);
                }
            } else if (option_name == "count") {
                count = atoi(optarg);
            } else if (option_name == "attack") {
                auto iter = attackMethods.find(optarg);
                if (iter == attackMethods.end()) {
                    std::cerr << "Unknown attack method: " << optarg << std::endl;
                    continue;
                }
                attackCreator = iter->second;
                attack = optarg;
            }
        }
    }

    if (username.empty() || host.empty()) {
        DisplayUsage();
        return 1;
    }

    std::cout << "username: " << username << std::endl;
    std::cout << "password: " << password << std::endl;
    std::cout << "host: " << host << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "count: " << count << std::endl;
    std::cout << "attack: " << (attack.empty() ? "none" : attack) << std::endl;

    std::vector<BotInstance> instances(count);
    s32 activeInstances = 0;

    for (std::size_t i = 0; i < instances.size(); ++i) {
        std::string instanceUsername = username;
        if (i != 0)
            instanceUsername += std::to_string(i);

        try {
            if (!instances[i].client.Login(host, port, instanceUsername, password, mc::core::UpdateMethod::Manual)) {
                std::cerr << "Failed to login with instance " << i << std::endl;
                continue;
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            continue;
        }

        if (attackCreator) {
            instances[i].attack = attackCreator(&instances[i].client);
        }

        instances[i].active = true;
        ++activeInstances;
    }

    std::cout << "Successfully logged in " << activeInstances << "/" << count << " instances." << std::endl;

    while (activeInstances > 0) {
        for (auto&& instance : instances) {
            if (!instance.active) continue;

            auto state = instance.client.GetConnection()->GetSocketState();
            if (state != mc::network::Socket::Status::Connected) {
                instance.active = false;
                --activeInstances;
                if (instance.attack) {
                    instance.attack = nullptr;
                }

                std::cout << "Instance was kicked from server. " << activeInstances << " instances remaining." << std::endl;
                continue;
            }

            instance.client.Update();
        }
    }

    return 0;
}

void DisplayUsage() {
    std::cout << "./avalanche --username [username] --password [password] --server [server:port]" << std::endl;
}
