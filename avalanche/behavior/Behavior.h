#ifndef _AVALANCHE_BEHAVIOR_H_
#define _AVALANCHE_BEHAVIOR_H_

#include "../Factory.h"

namespace Json {

class Value;

} // ns Json

namespace mc {
namespace core {

class Client;

} // ns core
} // ns mc

namespace avalanche {

class Behavior {
public:
    virtual ~Behavior() { }

    virtual void OnCreate() { }
    // Returns true when ready to be stopped
    virtual bool OnUpdate() { return false; }
    virtual void OnDestroy() { }

    virtual bool ReadJSON(const Json::Value& node) = 0;
};

using BehaviorFactory = Factory<Behavior, mc::core::Client*>;
extern BehaviorFactory g_BehaviorFactory;

} // ns avalanche

#endif
