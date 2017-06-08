#include "Instance.h"

namespace avalanche {

Instance::Instance(mc::protocol::Version version)
    : m_Dispatcher(),
      m_Client(&m_Dispatcher, version),
      m_Version(version),
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
