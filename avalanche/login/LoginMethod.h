#ifndef AVALANCHE_LOGIN_METHOD_H_
#define AVALANCHE_LOGIN_METHOD_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <mclib/common/Types.h>

#include "generator/AuthGenerator.h"

namespace Json {

class Value;

} // ns Json

namespace avalanche {

class Instance;

class LoginMethod {
protected:
    std::unique_ptr<AuthGenerator> m_Generator;
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

using LoginFactory = Factory<LoginMethod>;
extern LoginFactory g_LoginFactory;

} // ns avalanche

#endif
