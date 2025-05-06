#include <model/rootnode.h>

#include <model/itemnode.h>

RootNode::RootNode(const QString& name, TreeNode* parent)
    : TreeNode(NodeType::Root, name, parent)
{
}
