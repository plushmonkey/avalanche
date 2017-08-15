#include "BehaviorDisconnect.h"

#include <json/json.h>
#include <mclib/util/Utility.h>

namespace avalanche {

const char* BehaviorDisconnect::s_Name = "disconnect";

BehaviorDisconnect::BehaviorDisconnect(mc::core::Client* client, mc::protocol::Version version)
    : mc::protocol::packets::PacketHandler(client->GetDispatcher()),
      m_Client(client)
{
    
}

void BehaviorDisconnect::OnCreate() {
    m_Finished = false;
    GetDispatcher()->RegisterHandler(mc::protocol::State::Login, mc::protocol::login::LoginSuccess, this);
}

bool BehaviorDisconnect::OnUpdate() {
    return m_Finished;
}

void BehaviorDisconnect::OnDestroy() {
    GetDispatcher()->UnregisterHandler(this);
}

void BehaviorDisconnect::HandlePacket(mc::protocol::packets::in::LoginSuccessPacket* packet) {
    m_Finished = true;
    m_Client->GetConnection()->Disconnect();
}

bool BehaviorDisconnect::ReadJSON(const Json::Value& node) {
    return true;
}

} // ns avalanche
