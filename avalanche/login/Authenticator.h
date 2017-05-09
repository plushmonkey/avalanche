#ifndef _AVALANCHE_LOGIN_AUTHENTICATOR_H_
#define _AVALANCHE_LOGIN_AUTHENTICATOR_H_

#include <mclib/core/AuthToken.h>
#include "../Instance.h"

namespace avalanche {

class Authenticator {
public:
    enum class Method { Password, Token };

private:
    mc::core::AuthToken m_Token;
    std::string m_Username;
    std::string m_Password;
    Method m_Method;

    bool PasswordAuthentication(Instance& instance, const std::string& host, u16 port);
    bool TokenAuthentication(Instance& instance, const std::string& host, u16 port);

public:
    Authenticator(const std::string& username, const mc::core::AuthToken& token);
    Authenticator(const std::string& username, const std::string& password);

    bool Authenticate(Instance& instance, const std::string& host, u16 port);

    mc::core::AuthToken GetToken() const noexcept { return m_Token; }
    const std::string& GetUsername() const noexcept { return m_Username; }
    const std::string& GetPassword() const noexcept { return m_Password; }
    Method GetMethod() const noexcept { return m_Method; }
};

} // ns avalanche

#endif
