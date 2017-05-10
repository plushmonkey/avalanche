#include "BehaviorMessage.h"

#include <json/json.h>

namespace avalanche {

const char* BehaviorMessage::s_Name = "message";

BehaviorMessage::BehaviorMessage(mc::core::Client* client)
    : m_Client(client)
{

}

void BehaviorMessage::OnCreate() {
    m_Finished = false;
    m_Client->RegisterListener(this);
}

bool BehaviorMessage::OnUpdate() {
    return m_Finished;
}

void BehaviorMessage::OnDestroy() {
    m_Client->UnregisterListener(this);
}

void BehaviorMessage::OnTick() {
    if (m_Finished) return;

    mc::protocol::packets::out::ChatPacket packet(m_Message);
    m_Client->GetConnection()->SendPacket(&packet);

    m_Finished = true;
}

bool BehaviorMessage::ReadJSON(const Json::Value& node) {
    auto&& messageNode = node["message"];

    if (messageNode.isString())
        m_Message = messageNode.asString();
    else
        return false;

    return true;
}

} // ns avalanche
