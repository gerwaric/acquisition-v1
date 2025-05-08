#include <acquisition/model/characternode.h>

#include <acquisition/model/itemnode.h>
#include <acquisition/model/rootnode.h>
#include <acquisition/common/logging.h>

CharacterNode::CharacterNode(const poe::Character& character, TreeNode* parent)
    : TreeNode(NodeType::Character, character.name, parent)
    , m_character(character)
{
	LOG_TRACE("Creating CharacterNode for {} under {}", character.name, parent->name());

    if (character.equipment) {
        addCollection("Equipment", character.equipment.value());
    };
    if (character.inventory) {
        addCollection("Inventory", character.inventory.value());
    };
    if (character.rucksack) {
        addCollection("Rucksack", character.rucksack.value());
    };
    if (character.jewels) {
        addCollection("Jewels", character.jewels.value());
    };
}

void CharacterNode::addCollection(const QString& name, const std::vector<poe::Item>& items)
{
    addChild<RootNode>(name).addChildren<ItemNode>(items);
}
