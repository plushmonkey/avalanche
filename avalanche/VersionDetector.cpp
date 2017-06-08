#include "VersionDetector.h"

#include <unordered_map>
#include <iostream>

namespace avalanche {

VersionDetector::VersionDetector(const std::string& host, u16 port) 
    : m_Host(host),
      m_Port(port),
      m_Found(false),
      m_Version(mc::protocol::Version::Minecraft_1_11_2),
      m_Connection(nullptr)
{

}

void VersionDetector::OnPingResponse(const Json::Value& node) {
    static const std::unordered_map<s32, mc::protocol::Version> mapping = {
        { 210, mc::protocol::Version::Minecraft_1_10_2 },
        { 315, mc::protocol::Version::Minecraft_1_11_0 },
        { 316, mc::protocol::Version::Minecraft_1_11_2 },
        { 335, mc::protocol::Version::Minecraft_1_12_0 },
    };

    auto&& versionNode = node["version"];
    if (versionNode.isObject()) {
        auto&& protocolNode = versionNode["protocol"];
        if (protocolNode.isInt()) {
            s32 protocol = protocolNode.asInt();

            auto iter = mapping.lower_bound(protocol);
            if (iter != mapping.end()) {
                m_Version = iter->second;
                m_Found = true;
                m_Connection->Disconnect();
            }
        }
    }
}

mc::protocol::Version VersionDetector::GetVersion() {
    if (m_Found) return m_Version;

    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::core::Client m_Client(&dispatcher);

    m_Connection = m_Client.GetConnection();

    m_Client.GetConnection()->RegisterListener(this);
    m_Client.Ping(m_Host, m_Port, mc::core::UpdateMethod::Block);
    m_Client.GetConnection()->UnregisterListener(this);

    return m_Version;
}

} // ns avalanche
