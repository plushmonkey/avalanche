#ifndef _AVALANCHE_ATTACK_INTERACT_
#define _AVALANCHE_ATTACK_INTERACT_

#include "AttackMethod.h"

#include <mclib/core/Client.h>

namespace avalanche {

class AttackInteract : public AttackMethod, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    s32 m_SendPerTick;

public:
    AttackInteract(mc::core::Client* client);
    ~AttackInteract();

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;
};

} // ns avalanche

#endif

