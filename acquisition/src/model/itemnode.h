#pragma once

#include <libpoe/item.h>
#include <model/iteminfo.h>
#include <model/treenode.h>

class ItemNode : public TreeNode
{
public:

    enum class Column : int {
        PrettyName = 0,
        BaseType,
        Id
    };

    static constexpr int ColumnCount = 3;

	explicit ItemNode(const poe::Item& item, TreeNode* parent);
    ~ItemNode() override {};
    int columnCount() const override { return ColumnCount; };
    QVariant data(int column) const override;

	const poe::Item& item() const { return m_item; };
    const ItemInfo& itemInfo() const { return *m_item_info; };

private:
	const poe::Item& m_item;
    const std::unique_ptr<ItemInfo> m_item_info;
};
