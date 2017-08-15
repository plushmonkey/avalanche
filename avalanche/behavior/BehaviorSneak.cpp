#include "BehaviorSneak.h"

#include <json/json.h>
#include <mclib/util/Utility.h>
#include <limits>

namespace avalanche {

const char* BehaviorSneak::s_Name = "sneak";

BehaviorSneak::BehaviorSneak(mc::core::Client* client, mc::protocol::Version version)
    : m_Client(client),
      m_Start(true),
      m_Stop(false),
      m_Finished(false)
{

}

void BehaviorSneak::OnCreate() {
    m_Finished = false;
    m_Client->RegisterListener(this);
}

bool BehaviorSneak::OnUpdate() {
    return m_Finished;
}

void BehaviorSneak::OnDestroy() {
    m_Client->UnregisterListener(this);
}

void BehaviorSneak::OnTick() {
    if (!m_Client->GetWorld()->GetChunk(mc::ToVector3i(m_Client->GetPlayerController()->GetPosition())))
        return;

    using namespace mc::protocol::packets;

    auto eid = m_Client->GetEntityManager()->GetPlayerEntity()->GetEntityId();

    if (m_Start) {
        out::EntityActionPacket packet(eid, out::EntityActionPacket::Action::StartSneak);
        m_Client->GetConnection()->SendPacket(&packet);
    }

    if (m_Stop) {
        out::EntityActionPacket packet(eid, out::EntityActionPacket::Action::StopSneak);
        m_Client->GetConnection()->SendPacket(&packet);
    }

    m_Finished = true;
}

bool BehaviorSneak::ReadJSON(const Json::Value& node) {
    auto&& startNode = node["start"];
    auto&& stopNode = node["stop"];

    if (startNode.isBool())
        m_Start = startNode.asBool();

    if (stopNode.isBool())
        m_Stop = stopNode.asBool();

    return true;
}

} // ns avalanche
