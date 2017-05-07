#include "AttackInteract.h"

#include <mclib/util/Utility.h>

namespace avalanche {

AttackInteract::AttackInteract(mc::core::Client* client)
    : m_Client(client),
    m_SendPerTick(100)
{
    client->RegisterListener(this);
}

AttackInteract::~AttackInteract() {
    m_Client->UnregisterListener(this);
}

void AttackInteract::OnTick()  {
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

    for (s32 i = 0; i < m_SendPerTick; ++i) {
        PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::StartedDigging;
        PlayerDiggingPacket packet(status, target + Vector3i(0, 1, 0), mc::Face::West);

        m_Client->GetConnection()->SendPacket(&packet);

        status = PlayerDiggingPacket::Status::FinishedDigging;
        packet = PlayerDiggingPacket(status, target + Vector3i(0, 1, 0), mc::Face::West);

        m_Client->GetConnection()->SendPacket(&packet);
    }
}

bool AttackInteract::ReadJSON(const Json::Value& attackNode) {
    auto&& methodNode = attackNode["method"];
    if (!methodNode.isString() || methodNode.asString() != "interact")
        return false;

    auto&& sendPerTickNode = attackNode["send-per-tick"];
    if (!sendPerTickNode.isNull()) {
        m_SendPerTick = sendPerTickNode.asInt();
    }

    return true;
}

} // ns avalanche
