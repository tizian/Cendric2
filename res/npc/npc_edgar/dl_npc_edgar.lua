-- Dialogue for NPC "npc_edgar"

requiredGold = 300

loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_edgar", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_edgar", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Edgar_WhatDoWeHave") -- Well, well look at that. Another filthy thief, caught by our observer spells. I hope you enjoy your stay. He he he.
		DL:addConditionProgress("npc_edgar", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_edgar", "my_things")) then 
		DL:addChoice(3, "DL_Choice_WhereAreMyThings") -- Where are my belongings?
	end
	if (not DL:isConditionFulfilled("npc_edgar", "how_getout")) then 
		DL:addChoice(5, "DL_Choice_HowGetout") -- How can I get released?
	end
	if (not DL:isConditionFulfilled("npc_edgar", "hungry")) then 
		DL:addChoice(6, "DL_Choice_ImHungry") -- I'm hungry.
	end
	if (DL:isConditionFulfilled("npc_edgar", "hungry") and not DL:isConditionFulfilled("npc_edgar", "hungry_again")) then 
		DL:addChoice(7, "DL_Choice_ImStillHungry") -- I'm still hungry.
	end
	if (DL:isConditionFulfilled("npc_edgar", "how_getout") and not DL:isConditionFulfilled("npc_edgar", "pickaxe")) then 
		DL:addChoice(8, "DL_Choice_HowGold") -- How can I earn the gold to redeem myself?
	end
	if (DL:getItemAmount("gold") < requiredGold and DL:hasItem("mi_ironore", 1)) then 
		DL:addItemChoice(9, "DL_Choice_IronOre", "mi_ironore", DL:getItemAmount("mi_ironore")) -- I got some iron ore for you.
	end
	if (DL:isConditionFulfilled("npc_edgar", "pickaxe") and (DL:getItemAmount("gold") < requiredGold)) then 
		DL:addItemChoice(10, "DL_Choice_IHaveGold", "gold", requiredGold) -- Please let me out.
	end
	if (DL:isConditionFulfilled("npc_edgar", "pickaxe") and (DL:getItemAmount("gold") >= requiredGold)) then 
		DL:addItemChoice(11, "DL_Choice_IHaveGold", "gold", requiredGold) -- 
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_edgar", "my_things")) then 

		DL:createNPCNode(3, 4, "DL_Edgar_YourThings") -- Did you really think we'd leave a mage his weapon? The only thing you'll need here is a pickaxe anyway.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Edgar_YourThings2") -- But your old belongings are stored safely. You'll get them back on the day you're released.
		DL:addConditionProgress("npc_edgar", "my_things")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_edgar", "how_getout")) then 

		DL:createNPCNode(5, -2, "DL_Edgar_HowGetout") -- You will have to work in the mines until you can redeem yourself. Gandria won't pay a single coin for you filthy criminals.
		DL:addConditionProgress("npc_edgar", "how_getout")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_edgar", "hungry")) then 

		DL:createNPCNode(6, -2, "DL_Edgar_WaterAndBread") -- Sure, here you go. You better get used to it, you won't get anything else for a long time.
		DL:addConditionProgress("npc_edgar", "hungry")
		DL:addItem("fo_bread", 2)
		DL:addItem("fo_water", 1)
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_edgar", "hungry") and not DL:isConditionFulfilled("npc_edgar", "hungry_again")) then 

		DL:createNPCNode(7, -1, "DL_Edgar_HungryAgain") -- You already got your ration. Now get out of my sight!
		DL:addConditionProgress("npc_edgar", "hungry_again")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_edgar", "how_getout") and not DL:isConditionFulfilled("npc_edgar", "pickaxe")) then 

		DL:createNPCNode(8, -2, "DL_Edgar_Pickaxe") -- Here, take this pickaxe and mine iron ore in the mines. You can then sell it to me until you have enough gold to be released.
		DL:addConditionProgress("npc_edgar", "pickaxe")
		DL:addItem("we_pickaxe", 1)
		DL:equipItem("we_pickaxe")
		DL:addNode()

	end

	if (DL:getItemAmount("gold") < requiredGold and DL:hasItem("mi_ironore", 1)) then 

		DL:createNPCNode(9, -2, "DL_Edgar_Ironore") -- Thank you. Here is your pay.
		DL:removeItem("mi_ironore", DL:getItemAmount("mi_ironore"))
		DL:addGold(math.min(requiredGold - DL:getItemAmount("gold"), DL:getItemAmount("mi_ironore") * 10))
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_edgar", "pickaxe") and (DL:getItemAmount("gold") < requiredGold)) then 

		DL:createNPCNode(10, -2, "DL_Edgar_NotEnoughGold") -- You don't have enough gold. Go and work a bit more.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_edgar", "pickaxe") and (DL:getItemAmount("gold") >= requiredGold)) then 

		DL:createNPCNode(11, 12, "DL_Edgar_YoureReleased") -- Okay, that's enough.
		DL:removeGold(requiredGold)
		DL:addNode()
		
		DL:createNPCNode(12, 13, "DL_Edgar_YoureReleased2") -- You're free to go. I hope I won't see you here again.
		DL:addNode()
		
		DL:createNPCNode(13, 14, "DL_Edgar_YoureReleased3") -- You can fetch your old belongings from the chest in the barracks.
		DL:addNode()
		
		DL:createNPCNode(14, -1, "") -- 
		DL:startMap("res/map/gandriabarracks/gandriabarracks.tmx", 265, 110)
		DL:addNode()

	end

end