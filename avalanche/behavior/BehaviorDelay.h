#ifndef _AVALANCHE_BEHAVIOR_DELAY_H_
#define _AVALANCHE_BEHAVIOR_DELAY_H_

#include "Behavior.h"

#include <vector>
#include <memory>
#include <mclib/core/Client.h>

namespace avalanche {

class BehaviorDelay : public Behavior {
private:
    mc::core::Client* m_Client;
    s64 m_StartTime;
    s32 m_Delay;

public:
    BehaviorDelay(mc::core::Client* client);

    void OnCreate() override;
    bool OnUpdate() override;
    void OnDestroy() override;

    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};


} // ns avalanche

#endif
