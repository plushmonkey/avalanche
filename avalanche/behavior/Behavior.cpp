#include "Behavior.h"

#include "attack/AttackBookEdit.h"
#include "attack/AttackCreativeWorldLag.h"
#include "attack/AttackInteract.h"
#include "attack/AttackRecipe.h"
#include "BehaviorDelay.h"
#include "BehaviorMessage.h"
#include "BehaviorSequence.h"
#include "BehaviorSneak.h"
#include "BehaviorSwing.h"

namespace avalanche {

BehaviorFactory g_BehaviorFactory = BehaviorFactory::MethodRegistry {
    { BehaviorDelay::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorDelay>(client); } },
    { BehaviorMessage::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorMessage>(client); } },
    { BehaviorSequence::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSequence>(client); } },
    { BehaviorSneak::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSneak>(client); } },
    { BehaviorSwing::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSwing>(client); } },

    { AttackRecipe::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackRecipe>(client); } },
    { AttackBookEdit::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackBookEdit>(client); } },
    { AttackCreativeWorldLag::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackCreativeWorldLag>(client); } },
    { AttackInteract::s_Name, [](mc::core::Client* client) -> std::unique_ptr<Behavior> { return std::make_unique<AttackInteract>(client); } },
};

} // ns avalanche
