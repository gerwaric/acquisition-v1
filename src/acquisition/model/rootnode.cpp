#include <acquisition/model/rootnode.h>

#include <acquisition/model/itemnode.h>

RootNode::RootNode(const QString& name, TreeNode* parent)
    : TreeNode(NodeType::Root, name, parent)
{
}
