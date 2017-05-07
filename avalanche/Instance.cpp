#include "Instance.h"

namespace avalanche {

Instance::Instance()
    : m_Dispatcher(),
      m_Client(&m_Dispatcher, mc::protocol::Version::Minecraft_1_11_2)
{

}

void Instance::SetAttackMethod(std::unique_ptr<AttackMethod> attackMethod) {
    m_AttackMethod = std::move(attackMethod);
}

} // ns avalanche
