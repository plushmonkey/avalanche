#ifndef _AVALANCHE_INSTANCE_H_
#define _AVALANCHE_INSTANCE_H_

#include <mclib/core/Client.h>
#include "attack/AttackMethod.h"

namespace avalanche {

class Instance {
private:
    mc::protocol::packets::PacketDispatcher m_Dispatcher;
    mc::core::Client m_Client;
    std::unique_ptr<AttackMethod> m_AttackMethod;

public:
    Instance();

    void SetAttackMethod(std::unique_ptr<AttackMethod> attackMethod);

    mc::core::Client* GetClient() { return &m_Client; }
};

} // ns avalanche

#endif
