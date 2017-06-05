#include "Instance.h"

namespace avalanche {

Instance::Instance()
    : m_Dispatcher(),
      m_Client(&m_Dispatcher, mc::protocol::Version::Minecraft_1_11_2),
      m_Active(true)
{

}

bool Instance::Update() {
    if (!m_Active) return true;

    auto behavior = GetBehavior();

    auto state = GetClient()->GetConnection()->GetSocketState();
    if (state != mc::network::Socket::Status::Connected) {
        SetBehavior(nullptr);

        m_Active = false;
        return false;
    }

    if (behavior)
        behavior->OnUpdate();

    GetClient()->Update();

    return true;
}

void Instance::SetBehavior(std::unique_ptr<Behavior> behavior) {
    if (m_Behavior)
        m_Behavior->OnDestroy();
    m_Behavior = std::move(behavior);
}

} // ns avalanche
