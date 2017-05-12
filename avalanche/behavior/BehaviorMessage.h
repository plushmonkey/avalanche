#ifndef _AVALANCHE_BEHAVIOR_MESSAGE_H_
#define _AVALANCHE_BEHAVIOR_MESSAGE_H_

#include "Behavior.h"

#include <vector>
#include <memory>
#include <mclib/core/Client.h>

namespace avalanche {

class BehaviorMessage : public Behavior, mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    std::string m_Message;
    bool m_Finished;

public:
    BehaviorMessage(mc::core::Client* client);

    void OnCreate() override;
    bool OnUpdate() override;
    void OnDestroy() override;

    void OnTick() override;

    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};


} // ns avalanche

#endif
