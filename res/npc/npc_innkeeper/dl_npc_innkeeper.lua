-- Dialogue for NPC "Innkeeper"
loadDialogue = function(DL)

		DL:createChoiceNode(0)
		DL:addChoice(10, "DL_Choice_Rumors") -- Heard any rumours?
		DL:addChoice(1, "DL_Choice_GiveBeer") -- I'm thirsty, give me a beer. (10 Gold)
		DL:addChoice(2, "DL_Choice_SomethingElse") --  Do you also sell other things than beer? 
		if (not DL:isConditionFulfilled("npc_innkeeper", "asked_for_feudal_fire")) then 
			DL:addChoice(30, "DL_Choice_INeedSchnapps") --  I'm looking for a special schnapps... 
		end
		if (not DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire") and DL:isConditionFulfilled("npc_innkeeper", "wrong_name")) then 
			DL:addChoice(40, "DL_Choice_SellMeAnyway") --  Please, I really need your schnapps. (60 Gold) 
		end
		if (not DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire") and DL:isConditionFulfilled("npc_innkeeper", "wrong_name")) then 
			DL:addChoice(40, "DL_Choice_SellMeSchnapps") --  Give me some Feudal Fire. (20 Gold)   
		end
		DL:addChoice(-1, "") --
		DL:addNode()
		
		DL:setRoot(0)
		
		if (DL:hasItem("gold", 10)) then
			DL:createNPCNode(1, -2, "DL_Innkeeper_HereYouGo") -- Sure, here you go.
			DL:addItem("fo_beer",1)
			DL:removeGold(10)
			DL:addNode()
		else
			DL:createNPCNode(1, -2, "DL_Innkeeper_NotEnoughGold") -- If you want to have a beer, you need enough gold to pay for it.
		end
		
		if (not DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire") and DL:isConditionFulfilled("npc_innkeeper", "wrong_name")) then 
			if (DL:hasItem("gold", 60)) then
				DL:createNPCNode(40, -2, "DL_Innkeeper_TakeItAnyway") -- Well, you seem to have enough money to convince me. Here you go.
				DL:addConditionProgress("npc_innkeeper", "bought_feudal_fire")
				DL:addItem("pe_feudalfire",1)
				DL:removeGold(60)
				DL:addNode()
			else
				DL:createNPCNode(40, -2, "DL_Innkeeper_NoSchnapps") -- I can't sell my schnapps any Tom, Dick or Harry, you know. And especially not to some beggarly guy like you.
			end  
		end
		
		if (not DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire") and DL:isConditionFulfilled("npc_innkeeper", "right_name")) then 
			if (DL:hasItem("gold", 20)) then
				DL:createNPCNode(40, -2, "DL_Innkeeper_HereYouGo") -- Sure, here you go.
				DL:addItem("pe_feudalfire",1)
				DL:addConditionProgress("npc_innkeeper", "bought_feudal_fire")
				DL:removeGold(20)
				DL:addNode()
			else
				DL:createNPCNode(40, -2, "DL_Innkeeper_NotEnoughForSchnapps") -- You don't have enough gold to buy a bottle, I'm sorry.
			end  
		end
		
		DL:createNPCNode(10, -1, "No") -- NO (untranslated)
		DL:addNode()
		
		DL:createTradeNode(2, 0, "DL_Trade_TakeALook") -- Sure, take a look.
		DL:addNode()
		
		if (not DL:isConditionFulfilled("npc_innkeeper", "asked_for_feudal_fire")) then 
			DL:createNPCNode(30, 31, "DL_Innkeeper_WhatsTheName") --  Mhm, my booze is famous, isn't it. So famous, that you surely know its name? 
			DL:addNode()
			
			DL:createChoiceNode(31)
			DL:addChoice(32, "DL_Choice_Name2") -- Fiery Frenzy.
			DL:addChoice(32, "DL_Choice_Name3") -- Floral Force.
			DL:addChoice(32, "DL_Choice_Name4") -- Furry Fox.
			DL:addChoice(32, "DL_Choice_Name5") -- Fierce Farmer.
			DL:addChoice(32, "DL_Choice_Name6") -- Fake Flame.
			DL:addChoice(34, "DL_Choice_NameFeudalFire") -- Feudal Fire.
			DL:addChoice(32, "DL_Choice_Name7") -- Forgiving Frost.
			DL:addChoice(33, "DL_Choice_NameIDontKnow") -- I don't know.
			DL:addNode()
			
			DL:createNPCNode(33, -1, "DL_Innkeeper_ComeBackLater") -- If you don't know the name, you don't need it either. Ask again when you know more.
			DL:addNode()
			
			DL:createNPCNode(32, -2, "DL_Innkeeper_WrongName") -- Are you kidding me? That sounds ridiculous. I sell my schnapps only to people who know its name.
			DL:addConditionProgress("npc_innkeeper", "asked_for_feudal_fire")
			DL:addConditionProgress("npc_innkeeper", "wrong_name")
			DL:addNode()
			
			DL:createNPCNode(34, -2, "DL_Innkeeper_RightName") -- Very well then, you can have some. But be cautious, it doesn't bear its name in vain, it burns when it runs down your throat. Hee hee...
			DL:addConditionProgress("npc_innkeeper", "asked_for_feudal_fire")
			DL:addConditionProgress("npc_innkeeper", "right_name")
			DL:addNode()			
		end
		
    end