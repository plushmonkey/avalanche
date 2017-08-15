#include "Behavior.h"

#include "attack/AttackBookEdit.h"
#include "attack/AttackCreativeWorldLag.h"
#include "attack/AttackInteract.h"
#include "attack/AttackRecipe.h"
#include "BehaviorDelay.h"
#include "BehaviorDisconnect.h"
#include "BehaviorMessage.h"
#include "BehaviorSequence.h"
#include "BehaviorSneak.h"
#include "BehaviorSwing.h"

namespace avalanche {

BehaviorFactory g_BehaviorFactory = BehaviorFactory::MethodRegistry {
    { BehaviorDelay::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorDelay>(client, version); } },
    { BehaviorDisconnect::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorDisconnect>(client, version); } },
    { BehaviorMessage::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorMessage>(client, version); } },
    { BehaviorSequence::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSequence>(client, version); } },
    { BehaviorSneak::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSneak>(client, version); } },
    { BehaviorSwing::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<BehaviorSwing>(client, version); } },

    { AttackRecipe::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<AttackRecipe>(client, version); } },
    { AttackBookEdit::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<AttackBookEdit>(client, version); } },
    { AttackCreativeWorldLag::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<AttackCreativeWorldLag>(client, version); } },
    { AttackInteract::s_Name, [](mc::core::Client* client, mc::protocol::Version version) -> std::unique_ptr<Behavior> { return std::make_unique<AttackInteract>(client, version); } },
};

} // ns avalanche
