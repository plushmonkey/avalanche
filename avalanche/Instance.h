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
    mc::protocol::Version m_Version;
    bool m_Active;

public:
    Instance(mc::protocol::Version version);

    Behavior* GetBehavior() { return m_Behavior.get(); }
    void SetBehavior(std::unique_ptr<Behavior> behavior);

    mc::core::Client* GetClient() { return &m_Client; }

    // Returns false if the instance is transitioning from active to inactive.
    bool Update();
};

} // ns avalanche

#endif
