-- Dialogue for NPC "npc_bjarne"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_bjarne", "who_are_you")) then 
		DL:addChoice(2, "DL_Choice_WhoAreYou") -- Wer bist du?
	end
	if (DL:isConditionFulfilled("npc_bjarne", "who_are_you") and not DL:isConditionFulfilled("npc_bjarne", "talk")) then 
		DL:addChoice(9, "DL_Choice_Talk") -- You don't like to talk much?
	end
	if (DL:isQuestState("runas_deal", "started") and not DL:isQuestDescriptionUnlocked("runas_deal", 1) and DL:hasItem("fo_physalis", 10)) then 
		DL:addChoice(3, "DL_Choice_RunasDeal") -- Runa sent me. I got some Physalis for you.
	end
	if (DL:isQuestDescriptionUnlocked("runas_deal", 1) and not DL:isConditionFulfilled("npc_bjarne", "physalis")) then 
		DL:addChoice(6, "DL_Choice_Physalis") -- What do you need that many Physalis for?
	end
	if (DL:isQuestDescriptionUnlocked("runas_deal", 1) and not DL:isConditionFulfilled("npc_bjarne", "mages")) then 
		DL:addChoice(7, "DL_Choice_Mages") -- Why wouldn't Runa trust a mage?
	end
	if (DL:isQuestDescriptionUnlocked("runas_deal", 1) and not DL:isConditionFulfilled("npc_bjarne", "mage")) then 
		DL:addChoice(8, "DL_Choice_Mage") -- Aren't you a mage yourself?
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_bjarne", "who_are_you")) then 

		DL:createNPCNode(2, -2, "DL_Bjarne_WhoAreYou") -- My name's Bjarne. That should be enough.
		DL:addConditionProgress("npc_bjarne", "who_are_you")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_bjarne", "who_are_you") and not DL:isConditionFulfilled("npc_bjarne", "talk")) then 

		DL:createNPCNode(9, -1, "DL_Bjarne_Talk") -- No.
		DL:addConditionProgress("npc_bjarne", "talk")
		DL:addNode()

	end

	if (DL:isQuestState("runas_deal", "started") and not DL:isQuestDescriptionUnlocked("runas_deal", 1) and DL:hasItem("fo_physalis", 10)) then 

		DL:createNPCNode(3, 4, "DL_Bjarne_RunasDeal") -- Hm. I didn't expect her to send someone else.
		DL:removeItem("fo_physalis", 10)
		DL:addNode()


		DL:createNPCNode(4, 5, "DL_Bjarne_RunasDeal2") -- Especially not a mage.
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Bjarne_RunasDeal3") -- But well... here you go. You should be careful with that...
		DL:addQuestDescription("runas_deal", 1)
		DL:addItem("eq_antimagicpendant", 1)
		DL:addNode()

	end

	if (DL:isQuestDescriptionUnlocked("runas_deal", 1) and not DL:isConditionFulfilled("npc_bjarne", "physalis")) then 

		DL:createNPCNode(6, -1, "DL_Bjarne_Physalis") -- That's none of your business.
		DL:addConditionProgress("npc_bjarne", "physalis")
		DL:addNode()

	end

	if (DL:isQuestDescriptionUnlocked("runas_deal", 1) and not DL:isConditionFulfilled("npc_bjarne", "mages")) then 

		DL:createNPCNode(7, -2, "DL_Bjarne_Mages") -- Because she's afraid of them.
		DL:addConditionProgress("npc_bjarne", "mages")
		DL:addNode()

	end

	if (DL:isQuestDescriptionUnlocked("runas_deal", 1) and not DL:isConditionFulfilled("npc_bjarne", "mage")) then 

		DL:createNPCNode(8, -2, "DL_Bjarne_Mage") -- You should know that.
		DL:addConditionProgress("npc_bjarne", "mage")
		DL:addNode()

	end

end