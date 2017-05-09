#include "MultiUserGenerator.h"

#include <fstream>
#include <iostream>

namespace avalanche {

const char *MultiUserGenerator::s_Name = "multi-user";

MultiUserGenerator::MultiUserGenerator() 
    : m_Index(0)
{

}


bool MultiUserGenerator::Initialize(const std::string& filename) {
    m_Users.clear();

    std::ifstream in(filename);

    if (!in.is_open())
        throw std::runtime_error(std::string(s_Name) + ": Could not open file " + filename);

    std::string line;
    while (std::getline(in, line)) {
        auto pos = line.find(':');

        if (pos == std::string::npos)
            continue;

        m_Users.emplace_back(line.substr(0, pos), line.substr(pos + 1));
    }

    if (m_Users.empty())
        throw std::runtime_error(std::string(s_Name) + ": No users loaded from " + filename);

    return true;
}

Authenticator MultiUserGenerator::Generate() {
    if (m_Index > m_Users.size() - 1)
        throw std::out_of_range(std::string(s_Name) + ": Index higher than user count");

    UserPass current = m_Users[m_Index++];

    return Authenticator(current.username, current.password);
}

bool MultiUserGenerator::ReadJSON(const Json::Value& node) {
    auto&& filenameNode = node["filename"];

    if (!filenameNode.isString())
        return false;

    return Initialize(filenameNode.asString());
}

} // ns avalanche
