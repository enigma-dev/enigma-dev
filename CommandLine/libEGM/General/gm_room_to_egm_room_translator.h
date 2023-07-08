#ifndef GM_ROOM_TO_EGM_ROOM_TRANSLATOR_H
#define GM_ROOM_TO_EGM_ROOM_TRANSLATOR_H

#include "treenode.pb.h"

class GmRoomToEgmRoomTranslator {
private:
    buffers::TreeNode *treenodeRoot;
    buffers::TreeNode *roomsFolderRef;

    void AddEgmRoomsForAllGmRooms(buffers::TreeNode *root,
                                  buffers::TreeNode *parent = nullptr);
    void UpdateEgmRoom(const google::protobuf::Message *gmRoomMsg,
                       google::protobuf::Message *egmRoomMsg);
    const google::protobuf::FieldDescriptor* GetEGMRoomFieldDescriptor(
            const std::string &gmFieldDescName, google::protobuf::Message *egmRoomMsg);
    void RemoveAllGmRooms();

public:
    GmRoomToEgmRoomTranslator(buffers::TreeNode *root);

    void Translate();
};

#endif // GM_ROOM_TO_EGM_ROOM_TRANSLATOR_H
