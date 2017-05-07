#ifndef _AVALANCHE_ATTACK_METHOD_
#define _AVALANCHE_ATTACK_METHOD_

namespace Json {

class Value;

} // ns Json

namespace avalanche {

class AttackMethod {
public:
    virtual ~AttackMethod() { }

    virtual bool ReadJSON(const Json::Value& attackNode) = 0;
};


} // ns avalanche

#endif
