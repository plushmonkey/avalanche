#include "MultiTokenGenerator.h"

#include <fstream>

namespace avalanche {

const char *MultiTokenGenerator::s_Name = "multi-token";

MultiTokenGenerator::MultiTokenGenerator()
    : m_Index(0)
{

}


bool MultiTokenGenerator::Initialize(const std::string& filename) {
    m_Users.clear();

    std::ifstream in(filename);

    if (!in.is_open())
        throw std::runtime_error(std::string(s_Name) + ": Could not open file " + filename);

    std::string line;
    while (std::getline(in, line)) {
        auto pos = line.find(':');
        if (pos == std::string::npos) continue;

        auto length = pos;

        std::string username = line.substr(0, length);

        auto prev = pos + 1;

        pos = line.find(':', pos + 1);
        if (pos == std::string::npos) continue;

        length = pos - prev;
        std::string accessToken = line.substr(prev, length);

        prev = pos + 1;

        std::string clientToken;
        std::string profileId;

        pos = line.find(':', pos + 1);

        if (pos == std::string::npos) {
            clientToken = line.substr(prev);
        } else {
            length = pos - prev;

            clientToken = line.substr(prev, length);
            profileId = line.substr(pos + 1);
        }

        mc::core::AuthToken token(accessToken, clientToken, profileId);
        m_Users.emplace_back(username, token);
    }

    if (m_Users.empty())
        throw std::runtime_error(std::string(s_Name) + ": No users loaded from " + filename);

    return true;
}

Authenticator MultiTokenGenerator::Generate() {
    if (m_Index > m_Users.size() - 1)
        throw std::out_of_range(std::string(s_Name) + ": Index higher than user count");

    UserToken current = m_Users[m_Index++];

    return Authenticator(current.username, current.token);
}

bool MultiTokenGenerator::ReadJSON(const Json::Value& node) {
    auto&& filenameNode = node["filename"];

    if (!filenameNode.isString())
        return false;

    return Initialize(filenameNode.asString());
}

} // ns avalanche
