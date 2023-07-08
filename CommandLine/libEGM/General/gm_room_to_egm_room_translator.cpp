#include "gm_room_to_egm_room_translator.h"

using namespace google::protobuf;
using namespace buffers;
using namespace buffers::resources;

GmRoomToEgmRoomTranslator::GmRoomToEgmRoomTranslator(TreeNode *root) {
    treenodeRoot = root;
    roomsFolderRef = nullptr;
}

void GmRoomToEgmRoomTranslator::Translate() {
    if (!treenodeRoot)
        return;
    AddEgmRoomsForAllGmRooms(treenodeRoot);
    RemoveAllGmRooms();
}

void GmRoomToEgmRoomTranslator::AddEgmRoomsForAllGmRooms(TreeNode *root,
                                                         TreeNode *parent) {
    if (root->type_case() == TreeNode::TypeCase::kGmRoom) {
        std::cout << "Translating GMRoom '" << root->name() << "' to EGMRoom"
                  << std::endl;
        // add egm_room for current gm_room
        TreeNode *egmRoomRoot = parent->mutable_folder()->add_children();
        egmRoomRoot->set_name(root->name());
        EGMRoom *egmRoom = egmRoomRoot->mutable_egm_room();
        UpdateEgmRoom(&root->gm_room(), egmRoom);

        // save the parent folder pointer to remove all gm_rooms afterwards
        roomsFolderRef = parent;
    }
    else if (root->has_folder()) {
        for (int i=0; i<root->folder().children_size(); ++i) {
            AddEgmRoomsForAllGmRooms(root->mutable_folder()->mutable_children(i), root);
        }
    }
}

void GmRoomToEgmRoomTranslator::UpdateEgmRoom(const google::protobuf::Message *gmRoomMsg,
                                              google::protobuf::Message *egmRoomMsg) {
    using CppType = google::protobuf::FieldDescriptor::CppType;

    const Descriptor *gmDesc = gmRoomMsg->GetDescriptor();
    const Reflection *gmRefl = gmRoomMsg->GetReflection();

    const Reflection *egmRefl = egmRoomMsg->GetReflection();

    for (int i=0; i<gmDesc->field_count(); ++i) {
        const FieldDescriptor *gmFieldDesc = gmDesc->field(i);
        const FieldDescriptor *egmFieldDesc = GetEGMRoomFieldDescriptor(
                    gmFieldDesc->name(), egmRoomMsg);

        if (!egmFieldDesc)
            continue;

        switch (gmFieldDesc->cpp_type()) {
        case CppType::CPPTYPE_MESSAGE:
            if(gmFieldDesc->is_repeated()) {
                int repeatedMessageCount = gmRefl->FieldSize(*gmRoomMsg, gmFieldDesc);
                for (int j=0; j<repeatedMessageCount; ++j) {
                    Message *childMessage = egmRefl->AddMessage(egmRoomMsg, egmFieldDesc);
                    UpdateEgmRoom(&gmRefl->GetRepeatedMessage(*gmRoomMsg, gmFieldDesc, j),
                                  childMessage);
                }
            }
            else {
                Message *childMessage = egmRefl->MutableMessage(egmRoomMsg, egmFieldDesc);
                UpdateEgmRoom(&gmRefl->GetMessage(*gmRoomMsg, gmFieldDesc), childMessage);
            }
            break;
        case CppType::CPPTYPE_INT32:
            egmRefl->SetInt32(egmRoomMsg, egmFieldDesc,
                              gmRefl->GetInt32(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_INT64:
            egmRefl->SetInt64(egmRoomMsg, egmFieldDesc,
                              gmRefl->GetInt64(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_UINT32:
            egmRefl->SetUInt32(egmRoomMsg, egmFieldDesc,
                               gmRefl->GetUInt32(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_UINT64:
            egmRefl->SetUInt64(egmRoomMsg, egmFieldDesc,
                               gmRefl->GetUInt64(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_DOUBLE:
            egmRefl->SetDouble(egmRoomMsg, egmFieldDesc,
                               gmRefl->GetDouble(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_FLOAT:
            egmRefl->SetFloat(egmRoomMsg, egmFieldDesc,
                              gmRefl->GetFloat(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_BOOL:
            egmRefl->SetBool(egmRoomMsg, egmFieldDesc,
                             gmRefl->GetBool(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_ENUM:
            egmRefl->SetEnum(egmRoomMsg, egmFieldDesc,
                             gmRefl->GetEnum(*gmRoomMsg, gmFieldDesc));
            break;
        case CppType::CPPTYPE_STRING:
            egmRefl->SetString(egmRoomMsg, egmFieldDesc,
                               gmRefl->GetString(*gmRoomMsg, gmFieldDesc));
            break;
        default:
            break;
        }
    }
}

const FieldDescriptor* GmRoomToEgmRoomTranslator::GetEGMRoomFieldDescriptor(
        const std::string &gmFieldDescName, google::protobuf::Message *egmRoomMsg) {
    if (!egmRoomMsg)
        return nullptr;

    const google::protobuf::Descriptor *egmDesc = egmRoomMsg->GetDescriptor();
    const google::protobuf::FieldDescriptor *egmFieldDesc = egmDesc->FindFieldByName(
                gmFieldDescName);

    if (egmFieldDesc)
        return egmFieldDesc;
    else {
        // Handle special case where hsnap and vsnap are mapped to tilewidth and
        // tileheight respectively
        if (gmFieldDescName == "hsnap")
            return GetEGMRoomFieldDescriptor("tilewidth", egmRoomMsg);
        else if (gmFieldDescName == "vsnap")
            return GetEGMRoomFieldDescriptor("tileheight", egmRoomMsg);

        std::cout << "[Warning] EGM descriptor not found for name: " << gmFieldDescName
                  << std::endl;
        return nullptr;
    }
}

void GmRoomToEgmRoomTranslator::RemoveAllGmRooms() {
    if (!roomsFolderRef)
        return;

    RepeatedPtrField<buffers::TreeNode> *repeatedChildren = nullptr;
    repeatedChildren = roomsFolderRef->mutable_folder()->mutable_children();
    if (!repeatedChildren)
        return;

    int childrenSize = roomsFolderRef->mutable_folder()->children_size();

    for (int i=0; i<childrenSize/2; ++i)
        repeatedChildren->SwapElements(i, i+childrenSize/2);

    for (int i=0; i<childrenSize/2; ++i)
        repeatedChildren->RemoveLast();
}
