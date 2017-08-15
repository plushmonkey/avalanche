#include "BehaviorMessage.h"

#include <json/json.h>
#include <mclib/util/Utility.h>

namespace avalanche {

const char* BehaviorMessage::s_Name = "message";

BehaviorMessage::BehaviorMessage(mc::core::Client* client, mc::protocol::Version version)
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
    if (!m_Client->GetWorld()->GetChunk(mc::ToVector3i(m_Client->GetPlayerController()->GetPosition())))
        return;

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
