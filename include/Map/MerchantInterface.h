#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "GUI/MerchantWindow.h"
#include "Structs/MerchantData.h"
#include "Screens/WorldScreen.h"

// the merchant interface, acting as a interface between dialogue and merchant actions
class MerchantInterface final {
public:
	MerchantInterface(WorldScreen* screen, std::string merchantID);
	~MerchantInterface();

	const MerchantData& getMerchantData() const;
	const std::string& getMerchantID() const;
	CharacterCore* getCore() const;

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void sellItem(const Item* item);
	void buyItem(const Item* item);
	// saves the data to the core and exits
	void completeTrade();
	bool isCancelled();
	int getReputation(const Item* item) const;
	bool isReputationReached(const Item* item) const;

private:
	bool m_isCancelled = false;
	WorldScreen* m_screen = nullptr;
	Inventory* m_inventory = nullptr;
	CharacterCore* m_core = nullptr;
	std::string m_merchantID;
	MerchantData m_data;

	MerchantWindow* m_merchantWindow = nullptr;
};