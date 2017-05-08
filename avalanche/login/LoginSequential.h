#ifndef AVALANCHE_LOGIN_SEQUENTIAL_H_
#define AVALANCHE_LOGIN_SEQUENTIAL_H_

#include "LoginMethod.h"

namespace avalanche {

class LoginSequential : public LoginMethod {
private:
    s32 m_Delay;

    bool ReadMethodJSON(const Json::Value& node) override;

public:
    LoginSequential();

    std::size_t Login(std::vector<Instance>& instances);
};

} // ns avalanche

#endif
