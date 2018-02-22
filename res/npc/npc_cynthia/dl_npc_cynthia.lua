-- Dialogue for NPC "npc_cynthia"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_cynthia", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(10) 
	end 

	if (not DL:isConditionFulfilled("npc_cynthia", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Cynthia_ChooseSide") -- Whose side are you on? Mages or commoners?
		DL:addNode()


		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_Commoners") -- Commoners.
		DL:addChoice(4, "DL_Choice_Mages") -- Mages.
		if (not DL:isConditionFulfilled("npc_cynthia", "side")) then 
			DL:addChoice(5, "DL_Choice_WhySide") -- Why do I have to choose a side?
		end
		DL:addNode()


		DL:createNPCNode(3, 6, "DL_Cynthia_Commoners") -- You're right. Magic is the root of all evil.
		DL:addConditionProgress("npc_cynthia", "talked")
		DL:addNode()


		DL:createNPCNode(6, 7, "DL_Cynthia_Commoners2") -- Mages could kill you with a single word. Is that fair?
		DL:addNode()


		DL:createNPCNode(7, -1, "DL_Cynthia_Commoners3") -- Here, take this pendant, it will protect you against magic. Even against your own.
		DL:addItem("eq_antimagicpendant", 1)
		DL:addNode()


		DL:createNPCNode(4, 8, "DL_Cynthia_Mages") -- Yes, that's the only correct choice. We don't need the commoners anymore.
		DL:addConditionProgress("npc_cynthia", "talked")
		DL:addNode()


		DL:createNPCNode(8, 9, "DL_Cynthia_Mages2") -- Everything they can do, we can do even better.
		DL:addNode()


		DL:createNPCNode(9, -1, "DL_Cynthia_Mages3") -- Here, take this pendant. It will enhance your magic.
		DL:addItem("eq_magicpendant", 1)
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_cynthia", "side")) then 

			DL:createNPCNode(5, -2, "DL_Cynthia_WhySide") -- Because one day, there will only be one. So...
			DL:addConditionProgress("npc_cynthia", "side")
			DL:addNode()

		end

	end


	DL:createNPCNode(10, -1, "DL_Cynthia_Busy") -- I really got other things to do. Leave me alone.
	DL:addNode()

end