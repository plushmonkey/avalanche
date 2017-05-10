#ifndef _AVALANCHE_ATTACK_CREATIVEWORLDLAG_
#define _AVALANCHE_ATTACK_CREATIVEWORLDLAG_

#include "../Behavior.h"
#include "../../PositionProvider.h"

#include <mclib/core/Client.h>
#include <mclib/inventory/Slot.h>

#include <random>
#include <limits>

namespace avalanche {

class AttackCreativeWorldLag : public Behavior, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    std::unique_ptr<PositionProvider> m_PositionProvider;
    bool m_Finished;

    static s32 s_SendPerTick;

    mc::inventory::Slot CreateSlotAttack();

public:
    AttackCreativeWorldLag(mc::core::Client* client);
    ~AttackCreativeWorldLag();

    void OnCreate() override;
    bool OnUpdate() override { return m_Finished; }
    void OnDestroy() override;

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};

} // ns avalanche

#endif
