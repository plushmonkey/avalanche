#ifndef _AVALANCHE_LOGIN_AUTHINCREMENT_
#define _AVALANCHE_LOGIN_AUTHINCREMENT_

#include "AuthGenerator.h"
#include <string>

namespace avalanche {

class IncrementalGenerator : public AuthGenerator {
private:
    std::string m_BaseName;
    std::string m_Password;
    s32 m_Index;

    std::string GenerateNext();

public:
    IncrementalGenerator();

    void Initialize(const std::string& baseName, const std::string& password, s32 startIndex = 0);

    Authenticator Generate() override;

    const std::string& GetBaseName() const noexcept { return m_BaseName; }
    const std::string& GetPassword() const noexcept { return m_Password; }
    s32 GetIndex() const { return m_Index; }

    bool ReadJSON(const Json::Value& node) override;

    static const char *s_Name;
};

} // ns avalanche

#endif
