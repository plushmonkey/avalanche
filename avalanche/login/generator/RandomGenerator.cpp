#include "RandomGenerator.h"

namespace avalanche {

const char *RandomGenerator::s_Name = "random";

RandomGenerator::RandomGenerator()
    : m_MinSize(3),
      m_MaxSize(16),
      m_Engine(std::random_device()())
{

}

void RandomGenerator::Initialize(s32 minSize, s32 maxSize) {
    m_MinSize = minSize;
    m_MaxSize = maxSize;
}

Authenticator RandomGenerator::Generate() {
    std::string username;

    std::uniform_int_distribution<s32> lengthDist(m_MinSize, m_MaxSize);
    s32 length = lengthDist(m_Engine);

    std::uniform_int_distribution<s32> charDist('a', 'z');

    username.resize(length);
    for (s32 i = 0; i < length; ++i) {
        username[i] = (char)charDist(m_Engine);
    }

    return Authenticator(username, "");
}

bool RandomGenerator::ReadJSON(const Json::Value& node) {
    auto&& minNode = node["min"];
    auto&& maxNode = node["max"];

    if (minNode.isInt())
        m_MinSize = minNode.asInt();

    if (maxNode.isInt())
        m_MaxSize = maxNode.asInt();

    return true;
}

} // ns avalanche
