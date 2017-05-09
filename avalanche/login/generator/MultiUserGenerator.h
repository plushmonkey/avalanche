#ifndef _AVALANCHE_LOGIN_MULTIUSERGENERATOR_H_
#define _AVALANCHE_LOGIN_MULTIUSERGENERATOR_H_

#include "AuthGenerator.h"
#include <string>

namespace avalanche {

class MultiUserGenerator : public AuthGenerator {
private:
    struct UserPass {
        std::string username;
        std::string password;

        UserPass(const std::string& username, const std::string& password) : username(username), password(password) { }
    };

    std::vector<UserPass> m_Users;
    std::size_t m_Index;

public:
    MultiUserGenerator();

    bool Initialize(const std::string& filename);

    Authenticator Generate() override;

    std::size_t GetIndex() const { return m_Index; }

    bool ReadJSON(const Json::Value& node) override;

    static const char *s_Name;
};

} // ns avalanche

#endif
