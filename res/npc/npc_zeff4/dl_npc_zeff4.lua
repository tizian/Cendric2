-- Dialogue for NPC "npc_zeff4"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_zeff4", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_zeff4", "done")) then 
		DL:setRoot(2) 
	else 
		DL:setRoot(6) 
	end 

	if (not DL:isConditionFulfilled("npc_zeff4", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Zeff_Na") -- I knew you would be successful!
		DL:addConditionProgress("npc_zeff4", "talked")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_zeff4", "done")) then 

		DL:createChoiceNode(2)
		DL:addChoice(4, "DL_Choice_WhoAreYou") -- Who are you?
		DL:addChoice(3, "DL_Choice_LookingGood") -- You're looking good.
		DL:addChoice(5, "DL_Choice_CurseBroken") -- The curse is finally broken.
		DL:addNode()


		DL:createNPCNode(4, 7, "DL_Zeff_WhoAreYou") -- Don't you recognize me? I'm Zeff. A little less fur, but still Zeff.
		DL:addConditionProgress("npc_zeff4", "done")
		DL:addNode()


		DL:createNPCNode(7, -1, "DL_Zeff_WhoAreYou2") -- The curse is finally broken. Thank you, young mage.
		DL:addNode()


		DL:createNPCNode(3, 8, "DL_Zeff_LookingGood") -- Yeh, right? A little less fur, but I'm happy to be back.
		DL:addConditionProgress("npc_zeff4", "done")
		DL:addNode()


		DL:createNPCNode(8, -1, "DL_Zeff_WhoAreYou2") -- 
		DL:addNode()


		DL:createNPCNode(5, -1, "DL_Zeff_CurseBroken") -- It truly is. Thank you, young mage. I'll never forget you.
		DL:addConditionProgress("npc_zeff4", "done")
		DL:addNode()

	end


	DL:createNPCNode(6, -1, "DL_Zeff_Thanks") -- Thank you!
	DL:addNode()

end