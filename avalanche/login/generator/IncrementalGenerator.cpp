#include "IncrementalGenerator.h"

namespace avalanche {

const char *IncrementalGenerator::s_Name = "increment";

IncrementalGenerator::IncrementalGenerator() 
    : m_BaseName(""),
      m_Password(""),
      m_Index(0)
{

}

void IncrementalGenerator::Initialize(const std::string& baseName, const std::string& password, s32 startIndex) {
    m_BaseName = baseName;
    m_Password = password;
    m_Index = startIndex;
}

std::string IncrementalGenerator::GenerateNext() {
    s32 currentIndex = m_Index++;

    return m_BaseName + ((currentIndex > 0) ? std::to_string(currentIndex) : "");
}

Authenticator IncrementalGenerator::Generate() {
    std::string username = GenerateNext();

    return Authenticator(username, m_Password);
}

bool IncrementalGenerator::ReadJSON(const Json::Value& node) {
    auto&& usernameNode = node["username"];
    auto&& passwordNode = node["password"];
    auto&& startNode = node["start"];

    if (usernameNode.isString())
        m_BaseName = usernameNode.asString();

    if (passwordNode.isString())
        m_Password = passwordNode.asString();

    if (startNode.isInt())
        m_Index = startNode.asInt();

    return true;
}

} // ns avalanche
