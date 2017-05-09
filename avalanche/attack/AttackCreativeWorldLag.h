#ifndef _AVALANCHE_ATTACK_CREATIVEWORLDLAG_
#define _AVALANCHE_ATTACK_CREATIVEWORLDLAG_

#include "AttackMethod.h"
#include "../PositionProvider.h"

#include <mclib/core/Client.h>
#include <mclib/inventory/Slot.h>

#include <random>
#include <limits>

namespace avalanche {

class AttackCreativeWorldLag : public AttackMethod, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    std::unique_ptr<PositionProvider> m_PositionProvider;
    bool m_SendGamemode;

    static s32 s_SendPerTick;

    mc::inventory::Slot CreateSlotAttack();

public:
    AttackCreativeWorldLag(mc::core::Client* client);
    ~AttackCreativeWorldLag();

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;
};

} // ns avalanche

#endif
