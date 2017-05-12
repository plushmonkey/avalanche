#include "BehaviorSwing.h"

#include <json/json.h>
#include <mclib/util/Utility.h>
#include <limits>

namespace avalanche {

const char* BehaviorSwing::s_Name = "swing";

mc::Vector3d OrientationToVector(double yaw, double pitch) {
    return mc::Vector3d(
        -std::cos(pitch) * std::sin(yaw),
        -std::sin(pitch),
        std::cos(pitch) * std::cos(yaw)
    );
}

BehaviorSwing::BehaviorSwing(mc::core::Client* client)
    : m_Client(client),
      m_Hand(mc::Hand::Main),
      m_Attack(false),
      m_Finished(false)
{

}

void BehaviorSwing::OnCreate() {
    m_Finished = false;
    m_Client->RegisterListener(this);
}

bool BehaviorSwing::OnUpdate() {
    return m_Finished;
}

void BehaviorSwing::OnDestroy() {
    m_Client->UnregisterListener(this);
}

void BehaviorSwing::OnTick() {
    if (!m_Client->GetWorld()->GetChunk(mc::ToVector3i(m_Client->GetPlayerController()->GetPosition())))
        return;

    using namespace mc::protocol::packets;

    if (m_Attack) {
        auto controller = m_Client->GetPlayerController();
        float pitch = controller->GetPitch();
        float yaw = controller->GetYaw();

        mc::Vector3d direction = mc::Vector3Normalize(OrientationToVector(yaw, pitch));

        mc::Ray ray(controller->GetPosition() + mc::Vector3d(0, 1.6, 0), direction);

        auto entityMan = m_Client->GetEntityManager();

        static const mc::AABB playerBoundingBox(mc::Vector3d(-0.3, 0, -0.3), mc::Vector3d(0.3, 1.8, 0.3));

        mc::entity::EntityPtr closest;
        double closestDist = std::numeric_limits<double>::max();

        for (auto iter = entityMan->begin(); iter != entityMan->end(); ++iter) {
            auto entity = iter->second;

            if (entity == entityMan->GetPlayerEntity()) continue;

            if (entity) {
                auto&& meta = entity->GetMetadata();
                auto health = meta.GetIndex<mc::entity::EntityMetadata::FloatType>(7);

                if (!health) continue;
                if (health->value <= 0.0f) continue;

                mc::AABB entityBounds = playerBoundingBox + entity->GetPosition();

                double length;
                if (entityBounds.Intersects(ray, &length)) {
                    if (length < closestDist) {
                        closest = entity;
                        closestDist = length;
                    }
                }
            }
        }

        if (closestDist <= 4.0) {
            out::UseEntityPacket attackPacket(closest->GetEntityId(), out::UseEntityPacket::Action::Attack);

            m_Client->GetConnection()->SendPacket(&attackPacket);
        }
    }

    out::AnimationPacket packet(m_Hand);
    m_Client->GetConnection()->SendPacket(&packet);

    m_Finished = true;
}

bool BehaviorSwing::ReadJSON(const Json::Value& node) {
    auto&& attackNode = node["attack"];
    auto&& handNode = node["hand"];

    if (attackNode.isBool())
        m_Attack = attackNode.asBool();

    if (handNode.isString()) {
        std::string hand = handNode.asString();

        std::transform(hand.begin(), hand.end(), hand.begin(), ::tolower);

        if (hand == "off")
            m_Hand = mc::Hand::Off;
    }

    return true;
}

} // ns avalanche
