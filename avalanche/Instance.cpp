#include "Instance.h"

namespace avalanche {

Instance::Instance()
    : m_Dispatcher(),
      m_Client(&m_Dispatcher, mc::protocol::Version::Minecraft_1_11_2)
{

}

void Instance::SetBehavior(std::unique_ptr<Behavior> behavior) {
    m_Behavior = std::move(behavior);
}

} // ns avalanche
