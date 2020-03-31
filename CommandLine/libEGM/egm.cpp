#include "egm.h"
#include <iostream>

namespace {

using std::map;
using std::string;
using buffers::TreeNode;

void Flatten(const TreeNode &tree, string prefix, map<string, const TreeNode*> &out) {
  std::cout << "Flattening " << prefix << std::endl;
  for (const TreeNode &subtree : tree.child()) {
    const string cpath = prefix + "/" + subtree.name();
    if (subtree.type_case() == TreeNode::kFolder) {
      Flatten(subtree, cpath, out);
    } else {
      out[cpath] = &subtree;
    }
  }
}

}

namespace egm {

map<string, const TreeNode*> FlattenTree(const TreeNode &tree) {
  map<string, const TreeNode*> res;
  Flatten(tree, "", res);
  return res;
}

}
