#ifndef _AVALANCHE_LOGIN_MULTITOKENGENERATOR_H_
#define _AVALANCHE_LOGIN_MULTITOKENGENERATOR_H_

#include "AuthGenerator.h"
#include <string>

namespace avalanche {

class MultiTokenGenerator : public AuthGenerator {
private:
    struct UserToken {
        std::string username;
        mc::core::AuthToken token;

        UserToken(const std::string& username, mc::core::AuthToken token) : username(username), token(token) { }
    };

    std::vector<UserToken> m_Users;
    std::size_t m_Index;

public:
    MultiTokenGenerator();

    bool Initialize(const std::string& filename);

    Authenticator Generate() override;

    std::size_t GetIndex() const { return m_Index; }

    bool ReadJSON(const Json::Value& node) override;

    static const char *s_Name;
};

} // ns avalanche

#endif
