#ifndef _AVALANCHE_LOGIN_AUTHGENERATOR_
#define _AVALANCHE_LOGIN_AUTHGENERATOR_

#include "../../Instance.h"
#include "../Authenticator.h"

namespace Json {

class Value;

} // ns Json

namespace avalanche {

/*
 * This is the base class for authentication generators.
 * These are used to generate username/password combinations or session tokens for logging in.
 */
class AuthGenerator {
public:
    virtual ~AuthGenerator() { }

    virtual Authenticator Generate() = 0;

    virtual bool ReadJSON(const Json::Value& node) = 0;
};

} // ns avalanche

#endif
