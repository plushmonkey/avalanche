#ifndef _AVALANCHE_BEHAVIOR_SNEAK_H_
#define _AVALANCHE_BEHAVIOR_SNEAK_H_

#include "Behavior.h"

#include <vector>
#include <memory>
#include <mclib/core/Client.h>

namespace avalanche {

class BehaviorSneak : public Behavior, mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    bool m_Start;
    bool m_Stop;
    bool m_Finished;

public:
    BehaviorSneak(mc::core::Client* client, mc::protocol::Version version);

    void OnCreate() override;
    bool OnUpdate() override;
    void OnDestroy() override;

    void OnTick() override;

    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};


} // ns avalanche

#endif
