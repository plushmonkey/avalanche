#ifndef _AVALANCHE_ATTACK_BOOKEDIT_
#define _AVALANCHE_ATTACK_BOOKEDIT_

#include "AttackMethod.h"

#include <mclib/core/Client.h>

namespace avalanche {


class AttackBookEdit : public AttackMethod, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    static std::string s_AttackData;
    static s32 s_BookPages;
    static s32 s_SendPerTick;

    void CreateAttack();

public:
    AttackBookEdit(mc::core::Client* client);
    ~AttackBookEdit();

    void OnTick() override;
    bool ReadJSON(const Json::Value& attackNode) override;

    static const char* s_Name;
};

} // ns avalanche

#endif
