#ifndef _AVALANCHE_ATTACK_BOOKEDIT_
#define _AVALANCHE_ATTACK_BOOKEDIT_

#include "AttackMethod.h"

#include <mclib/core/Client.h>

namespace avalanche {


class AttackBookEdit : public AttackMethod, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    bool m_LoggedIn;
    static std::string s_AttackData;
    static s32 s_BookPages;
    static s32 s_SendPerTick;

    void CreateAttack();

public:
    AttackBookEdit(mc::core::Client* client);
    ~AttackBookEdit();

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;
};

} // ns avalanche

#endif
