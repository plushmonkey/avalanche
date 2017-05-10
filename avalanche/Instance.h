#ifndef _AVALANCHE_INSTANCE_H_
#define _AVALANCHE_INSTANCE_H_

#include <mclib/core/Client.h>
#include "behavior/Behavior.h"

namespace avalanche {

class Instance {
private:
    mc::protocol::packets::PacketDispatcher m_Dispatcher;
    mc::core::Client m_Client;
    std::unique_ptr<Behavior> m_Behavior;

public:
    Instance();

    Behavior* GetBehavior() { return m_Behavior.get(); }
    void SetBehavior(std::unique_ptr<Behavior> behavior);

    mc::core::Client* GetClient() { return &m_Client; }
};

} // ns avalanche

#endif
