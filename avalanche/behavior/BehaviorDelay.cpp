#include "BehaviorDelay.h"

#include <json/json.h>
#include <mclib/util/Utility.h>

namespace avalanche {

const char* BehaviorDelay::s_Name = "delay";

BehaviorDelay::BehaviorDelay(mc::core::Client* client, mc::protocol::Version version)
    : m_Client(client),
      m_Delay(0)
{

}

void BehaviorDelay::OnCreate() {
    m_StartTime = mc::util::GetTime();
}

bool BehaviorDelay::OnUpdate() {
    return mc::util::GetTime() >= m_StartTime + m_Delay;
}

void BehaviorDelay::OnDestroy() {
    
}

bool BehaviorDelay::ReadJSON(const Json::Value& node) {
    auto&& delayNode = node["delay"];

    if (delayNode.isInt())
        m_Delay = delayNode.asInt();
    else
        return false;

    return true;
}

} // ns avalanche
