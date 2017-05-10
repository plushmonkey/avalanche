#include "BehaviorSequence.h"

#include <json/json.h>
#include <iostream>

namespace avalanche {

const char* BehaviorSequence::s_Name = "sequence";

BehaviorSequence::BehaviorSequence(mc::core::Client* client) 
    : m_Client(client),
      m_Index(0),
      m_Repeat(true)
{

}

void BehaviorSequence::OnCreate() {
    if (m_Children.empty()) return;

    m_Children.front()->OnCreate();
    m_Finished = false;
}

bool BehaviorSequence::OnUpdate() {
    if (m_Finished) return true;

    auto&& child = m_Children[m_Index];

    if (child->OnUpdate()) {
        child->OnDestroy();

        ++m_Index;

        const s32 maxIndex = m_Children.size() - 1;

        if (m_Index > maxIndex) {
            if (m_Repeat)
                m_Index = 0;
            else
                m_Finished = true;
        }

        if (m_Index <= maxIndex)
            m_Children[m_Index]->OnCreate();
    }

    return m_Finished;
}

void BehaviorSequence::OnDestroy()  {
    if (!m_Finished)
        m_Children[m_Index]->OnDestroy();
}

bool BehaviorSequence::ReadJSON(const Json::Value& node) {
    auto&& repeatNode = node["repeat"];

    if (repeatNode.isBool())
        m_Repeat = repeatNode.asBool();

    auto&& childrenNode = node["children"];

    if (!childrenNode.isArray())
        return false;

    for (auto&& iter = childrenNode.begin(); iter != childrenNode.end(); ++iter) {
        if (!iter->isObject()) continue;

        auto&& childNode = *iter;
        auto&& methodNode = childNode["method"];
        if (!methodNode.isString())
            continue;

        std::string method = methodNode.asString();

        auto child = g_BehaviorFactory.Create(method, m_Client);
        if (!child) {
            std::cerr << "Unknown behavior " << method << " in sequence." << std::endl;
            continue;
        }

        if (!child->ReadJSON(childNode)) {
            std::cerr << "Failed to read sequence child " << method << std::endl;
            continue;
        }

        m_Children.push_back(std::move(child));
    }

    return true;
}

} // ns avalanche
