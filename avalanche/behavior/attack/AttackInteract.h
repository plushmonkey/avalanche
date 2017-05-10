#ifndef _AVALANCHE_ATTACK_INTERACT_
#define _AVALANCHE_ATTACK_INTERACT_

#include "../Behavior.h"

#include <mclib/core/Client.h>

namespace avalanche {

class AttackInteract : public Behavior, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    s32 m_SendPerTick;
    bool m_Finished;

public:
    AttackInteract(mc::core::Client* client);
    ~AttackInteract();

    void OnCreate() override;
    bool OnUpdate() override { return m_Finished; }
    void OnDestroy() override;

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};

} // ns avalanche

#endif

