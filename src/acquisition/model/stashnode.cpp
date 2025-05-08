#include <acquisition/model/stashnode.h>

#include <acquisition/model/itemnode.h>
#include <acquisition/common/logging.h>

StashNode::StashNode(const poe::StashTab& stash, TreeNode* parent)
    : TreeNode(NodeType::Stash, stash.name, parent)
    , m_stash(stash)
{
	LOG_TRACE("Creating StashNode for {} under {}", stash.name, parent->name());

    if (stash.children) {
        addChildren<StashNode>(stash.children.value());
    };
    if (stash.items) {
        addChildren<ItemNode>(stash.items.value());
    };
}

QVariant StashNode::data(int column) const
{
    if (column > 0) {
        return "";
    } else if (m_stash.index) {
        return QString("#%1, %2").arg(QString::number(m_stash.index.value()), m_stash.name);
    } else {
        return m_stash.name;
    };
}
