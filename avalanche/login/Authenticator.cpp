#include "Authenticator.h"

namespace avalanche {

Authenticator::Authenticator(const std::string& username, const mc::core::AuthToken& token)
    : m_Username(username),
      m_Token(token),
      m_Method(Method::Token)
{

}

Authenticator::Authenticator(const std::string& username, const std::string& password)
    : m_Username(username),
      m_Password(password),
      m_Method(Method::Password)
{

}

bool Authenticator::PasswordAuthentication(Instance& instance, const std::string& host, u16 port) {
    return instance.GetClient()->Login(host, port, m_Username, m_Password, mc::core::UpdateMethod::Manual);
}

bool Authenticator::TokenAuthentication(Instance& instance, const std::string& host, u16 port) {
    return instance.GetClient()->Login(host, port, m_Username, m_Token, mc::core::UpdateMethod::Manual);
}

bool Authenticator::Authenticate(Instance& instance, const std::string& host, u16 port) {
    if (m_Method == Method::Password)
        return PasswordAuthentication(instance, host, port);
    return TokenAuthentication(instance, host, port);
}

} // ns avalanche
