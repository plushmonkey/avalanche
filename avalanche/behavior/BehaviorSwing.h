#ifndef _AVALANCHE_BEHAVIOR_SWING_H_
#define _AVALANCHE_BEHAVIOR_SWING_H_

#include "Behavior.h"

#include <vector>
#include <memory>
#include <mclib/core/Client.h>

namespace avalanche {

class BehaviorSwing : public Behavior, mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    mc::Hand m_Hand;
    bool m_Attack;
    bool m_Finished;

public:
    BehaviorSwing(mc::core::Client* client);

    void OnCreate() override;
    bool OnUpdate() override;
    void OnDestroy() override;

    void OnTick() override;

    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};


} // ns avalanche

#endif
