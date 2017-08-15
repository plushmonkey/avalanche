#ifndef _AVALANCHE_ATTACK_RECIPE_
#define _AVALANCHE_ATTACK_RECIPE_

#include "../Behavior.h"

#include <mclib/core/Client.h>

namespace avalanche {

class AttackRecipe : public Behavior, public mc::core::ClientListener, mc::protocol::packets::PacketHandler {
private:
    mc::core::Client* m_Client;
    mc::protocol::Version m_Version;
    s32 m_TransactionIndex;
    bool m_Finished;
    bool m_Transaction;

    static mc::inventory::Slot s_AttackItem;
    static s32 s_BookPages;
    static s32 s_SendPerTick;
    static s32 s_ReturnCount;
    static s32 s_PrepareCount;

    void CreateAttack();

public:
    AttackRecipe(mc::core::Client* client, mc::protocol::Version version);
    ~AttackRecipe();

    void OnCreate() override;
    bool OnUpdate() override { return m_Finished; }
    void OnDestroy() override;

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;

    void HandlePacket(mc::protocol::packets::in::ConfirmTransactionPacket* packet) override;

    static const char* s_Name;
};

} // ns avalanche

#endif
