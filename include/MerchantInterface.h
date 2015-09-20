#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "GUI/MerchantWindow.h"
#include "Structs/MerchantData.h"
#include "Screens/GameScreen.h"
#include "GUI/Button.h"

// the merchant interface, acting as a interface between dialogue and merchant actions
class MerchantInterface
{
public:
	MerchantInterface(GameScreen* screen, std::string merchantID);
	~MerchantInterface();

	const MerchantData& getMerchantData() const;
	const std::string& getMerchantID() const;
	CharacterCore* getCore() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void sellItem(const Item& item);
	void buyItem(const Item& item);
	// saves the data to the core and exits
	void completeTrade();
	bool isCancelled();

private:
	bool m_isCancelled = false;
	GameScreen* m_screen = nullptr;
	Inventory* m_inventory = nullptr;
	CharacterCore* m_core = nullptr;
	std::string m_merchantID;
	MerchantData m_data;

	MerchantWindow* m_merchantWindow = nullptr;
};