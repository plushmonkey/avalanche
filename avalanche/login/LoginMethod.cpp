#include "LoginMethod.h"

#include <json/json.h>

namespace avalanche {

LoginMethod::LoginMethod()
    : m_Port(25565)
{

}

void LoginMethod::ParseServer(const std::string& server) {
    m_Host = server;

    auto pos = m_Host.find(':');
    if (pos != std::string::npos) {
        m_Port = atoi(m_Host.substr(pos + 1).c_str());
        m_Host = m_Host.substr(0, pos);
    }
}

bool LoginMethod::ReadJSON(const Json::Value& node) {
    auto&& usernameNode = node["username"];
    auto&& passwordNode = node["password"];
    auto&& serverNode = node["server"];
    auto&& methodNode = node["method"];

    if (!usernameNode.isString() || !serverNode.isString() || !methodNode.isObject())
        return false;

    if (usernameNode.isString())
        m_Username = usernameNode.asString();

    if (passwordNode.isString())
        m_Password = passwordNode.asString();

    if (serverNode.isString())
        ParseServer(serverNode.asString());

    return ReadMethodJSON(methodNode);
}

bool LoginMethod::ReadOptions(std::unordered_map<std::string, std::string> options) {
    auto usernameIter = options.find("username");
    auto passwordIter = options.find("password");
    auto serverIter = options.find("server");
    
    if (usernameIter != options.end())
        m_Username = usernameIter->second;

    if (passwordIter != options.end())
        m_Password = passwordIter->second;

    if (serverIter != options.end())
        ParseServer(serverIter->second);

    return !m_Username.empty() && !m_Password.empty() && !m_Host.empty();
}

} // ns avalanche
