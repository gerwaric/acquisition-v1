#pragma once

#include <acquisition/poe_api/character.h>
#include <acquisition/model/treenode.h>

class CharacterNode : public TreeNode
{
public:
	CharacterNode(const poe::Character& character, TreeNode* parent);
    ~CharacterNode() override {};
private:
	void addCollection(const QString& name, const std::vector<poe::Item>& items);
	const poe::Character& m_character;
};
