#pragma once

#include <libpoe/stashtab.h>
#include <model/treenode.h>

class StashNode : public TreeNode
{
public:
	explicit StashNode(const poe::StashTab& stash, TreeNode* parent);
    ~StashNode() override {};
    QVariant data(int column) const override;
private:
	const poe::StashTab& m_stash;
};
