#ifndef _AVALANCHE_BEHAVIOR_SEQUENCE_
#define _AVALANCHE_BEHAVIOR_SEQUENCE_

#include "Behavior.h"

#include <vector>
#include <memory>
#include <mclib/core/Client.h>

namespace avalanche {

class BehaviorSequence : public Behavior {
private:
    mc::core::Client* m_Client;
    mc::protocol::Version m_Version;
    std::vector<std::unique_ptr<Behavior>> m_Children;
    s32 m_Index;
    bool m_Repeat;
    bool m_Finished;

public:
    BehaviorSequence(mc::core::Client* client, mc::protocol::Version version);

    void OnCreate() override;
    bool OnUpdate() override;
    void OnDestroy() override;

    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};


} // ns avalanche

#endif
