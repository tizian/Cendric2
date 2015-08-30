#include "GUI/QuickSlotBar.h"
#include "LevelInterface.h"

QuickSlotBar::QuickSlotBar(LevelInterface* _interface)
{
	m_quickSlot1 = new QuickSlot(_interface, _interface->getCore()->getData().quickSlot1, Key::QuickSlot1);
	m_quickSlot1->setPosition(QUICKSLOT_OFFSET);

	m_quickSlot2 = new QuickSlot(_interface, _interface->getCore()->getData().quickSlot2, Key::QuickSlot2);
	m_quickSlot2->setPosition(QUICKSLOT_OFFSET + sf::Vector2f(QuickSlot::SIDE_LENGTH + QUICKSLOT_SPACING, 0.f));
}

QuickSlotBar::~QuickSlotBar()
{
	delete m_quickSlot1;
	delete m_quickSlot2;
}

void QuickSlotBar::show()
{
	m_isVisible = true;
}

void QuickSlotBar::hide()
{
	m_isVisible = false;
}

void QuickSlotBar::render(sf::RenderTarget& target)
{
	if (!m_isVisible) return;
	m_quickSlot1->render(target);
	m_quickSlot2->render(target);
}

void QuickSlotBar::update(const sf::Time& frameTime)
{
	if (!m_isVisible) return;
	m_quickSlot1->update(frameTime);
	m_quickSlot2->update(frameTime);
}

void QuickSlotBar::notifyConsumableDrop(const InventorySlotClone* item)
{
	if (item == nullptr) return;
	if (item->getBoundingBox()->intersects(*(m_quickSlot1->getBoundingBox())))
	{
		m_quickSlot1->setItemID(item->getItemID());
		return;
	}
	if (item->getBoundingBox()->intersects(*(m_quickSlot2->getBoundingBox())))
	{
		m_quickSlot2->setItemID(item->getItemID());
		return;
	}
}

void QuickSlotBar::reload()
{
	m_quickSlot1->reload();
	m_quickSlot2->reload();
}
