-- Dialogue for NPC "npc_citizen1"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	DL:addChoice(3, "DL_Choice_Where") -- Excuse me, where can I find...
	DL:addChoice(2, "DL_Choice_About") -- Could you tell me something about...
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createChoiceNode(3)
	DL:addChoice(6, "DL_Choice_WhereBlacksmith") -- ... a blacksmith?
	DL:addChoice(9, "DL_Choice_WhereAlchemist") -- ... an alchemist?
	if (DL:isConditionFulfilled("npc_zeff", "zeff_gone")) then 
		DL:addChoice(8, "DL_Choice_WhereLeatherworker") -- ... a leatherworker?
	end
	if (not DL:isConditionFulfilled("npc_zeff", "zeff_gone")) then 
		DL:addChoice(11, "DL_Choice_WhereLeatherworker") -- 
	end
	DL:addChoice(4, "DL_Choice_WhereTavern") -- ... the tavern?
	DL:addChoice(5, "DL_Choice_WhereBarracks") -- ... the barracks?
	DL:addChoice(7, "DL_Choice_WhereCathedral") -- ... the cathedral?
	DL:addChoice(10, "DL_Choice_WhereBrothel") -- ... the brothel?
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(6, -2, "DL_Citizen_WhereBlacksmith") -- Our best blacksmith is Harek. You can find him in the artisans' district, in the southwest part of Gandria.
	DL:gotoNode(3)
	DL:addNode()


	DL:createNPCNode(9, -2, "DL_Citizen_WhereAlchemist") -- You're looking for Syrah. Her house is near the artisans' district, in the southern part of Gandria.
	DL:gotoNode(3)
	DL:addNode()

	if (DL:isConditionFulfilled("npc_zeff", "zeff_gone")) then 

		DL:createNPCNode(8, -2, "DL_Citizen_ZeffHere") -- The leatherworker Zeff has his workshop in the artisans' district. Although being transformed into a wolf-human, he's still knows his craft.
		DL:gotoNode(3)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_zeff", "zeff_gone")) then 

		DL:createNPCNode(11, -2, "DL_Citizen_ZeffGone") -- We used to have a leatherworker in our city, but he recently disappeared. Nobody knows what happened to him.
		DL:gotoNode(3)
		DL:addNode()

	end


	DL:createNPCNode(4, -2, "DL_Citizen_WhereTavern") -- The "Basilisk Inn" is in between the artisans' district and the harbour, near the market square.
	DL:gotoNode(3)
	DL:addNode()


	DL:createNPCNode(5, -2, "DL_Citizen_WhereBarracks") -- The barracks are near the gates of the inner city wall, on the other side of the river.
	DL:gotoNode(3)
	DL:addNode()


	DL:createNPCNode(7, -2, "DL_Citizen_WhereCathedral") -- The cathedral is in the northern part of Gandria, on the other side of the river.
	DL:gotoNode(3)
	DL:addNode()


	DL:createNPCNode(10, -2, "DL_Citizen_WhereBrothel") -- Ehm... The brothel is in the harbour district.
	DL:gotoNode(3)
	DL:addNode()


	DL:createChoiceNode(2)
	DL:addChoice(14, "DL_Choice_AboutGandria") -- ... Gandria?
	DL:addChoice(13, "DL_Choice_AboutKing") -- ... the king?
	DL:addChoice(12, "DL_Choice_AboutMages") -- ... mages?
	DL:addChoice(15, "DL_Choice_AboutNecromany") -- ... necromancy?
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(14, -2, "DL_Citizen_AboutGandria") -- Gandria is not only the capital of Admantris, it also plays an important economic role as a port. 
	DL:gotoNode(2)
	DL:addNode()


	DL:createNPCNode(13, -2, "DL_Citizen_AboutKing") -- We don't know much about our king, Logan the Third. He has only recently ascended the throne.
	DL:gotoNode(2)
	DL:addNode()


	DL:createNPCNode(12, 16, "DL_Citizen_AboutMages") -- Mages rule our city. Most of the people in the militia know magic and the king has always been a mage as far as I can remember.
	DL:addNode()


	DL:createNPCNode(16, -2, "DL_Citizen_AboutMages2") -- It's hard being a commoner in a world ruled by magic.
	DL:gotoNode(2)
	DL:addNode()


	DL:createNPCNode(15, -2, "DL_Citizen_AboutNecromany") -- Hush! We don't talk about that.
	DL:gotoNode(2)
	DL:addNode()

end