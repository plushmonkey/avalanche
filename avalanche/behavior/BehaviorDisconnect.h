#ifndef _AVALANCHE_BEHAVIOR_DISCONNECT_H_
#define _AVALANCHE_BEHAVIOR_DISCONNECT_H_

#include "Behavior.h"

#include <vector>
#include <memory>
#include <mclib/core/Client.h>

namespace avalanche {

class BehaviorDisconnect : public Behavior, mc::protocol::packets::PacketHandler {
private:
    mc::core::Client* m_Client;
    bool m_Finished;

public:
    BehaviorDisconnect(mc::core::Client* client);

    void OnCreate() override;
    bool OnUpdate() override;
    void OnDestroy() override;

    bool ReadJSON(const Json::Value& attackNode) override;
    void HandlePacket(mc::protocol::packets::in::LoginSuccessPacket* packet) override;

    static const char* s_Name;
};


} // ns avalanche

#endif
