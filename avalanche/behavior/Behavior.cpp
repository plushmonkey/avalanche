#include "Behavior.h"

#include "attack/AttackInteract.h"
#include "attack/AttackBookEdit.h"
#include "attack/AttackCreativeWorldLag.h"
#include "BehaviorDelay.h"
#include "BehaviorMessage.h"
#include "BehaviorSequence.h"

namespace avalanche {

BehaviorFactory g_BehaviorFactory = BehaviorFactory::MethodRegistry {
    { BehaviorDelay::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorDelay>(client); } },
    { BehaviorMessage::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorMessage>(client); } },
    { BehaviorSequence::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSequence>(client); } },

    { AttackBookEdit::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackBookEdit>(client); } },
    { AttackCreativeWorldLag::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackCreativeWorldLag>(client); } },
    { AttackInteract::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackInteract>(client); } },
};

} // ns avalanche
