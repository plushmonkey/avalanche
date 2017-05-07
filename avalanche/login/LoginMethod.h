#ifndef AVALANCHE_LOGIN_METHOD_H_
#define AVALANCHE_LOGIN_METHOD_H_

#include <unordered_map>
#include <string>
#include <mclib/common/Types.h>

namespace Json {

class Value;

} // ns Json

namespace avalanche {

class Instance;

class LoginMethod {
protected:
    std::string m_Username;
    std::string m_Password;
    std::string m_Host;
    u16 m_Port;

    virtual bool ReadMethodJSON(const Json::Value& node) = 0;
    void ParseServer(const std::string& server);

public:
    LoginMethod();
    virtual ~LoginMethod() { }

    bool ReadOptions(std::unordered_map<std::string, std::string> options);
    bool ReadJSON(const Json::Value& node);

    virtual std::size_t Login(std::vector<Instance>& instances) = 0;
};

} // ns avalanche

#endif
