#ifndef _AVALANCHE_LOGIN_RANDOMGENERATOR_
#define _AVALANCHE_LOGIN_RANDOMGENERATOR_

#include "AuthGenerator.h"
#include <random>

namespace avalanche {

class RandomGenerator : public AuthGenerator {
private:
    std::mt19937 m_Engine;
    s32 m_MinSize;
    s32 m_MaxSize;

public:
    RandomGenerator();

    void Initialize(s32 minSize, s32 maxSize);

    Authenticator Generate() override;

    bool ReadJSON(const Json::Value& node) override;

    static const char *s_Name;
};

} // ns avalanche

#endif
