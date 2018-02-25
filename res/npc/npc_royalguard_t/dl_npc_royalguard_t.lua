-- Dialogue for NPC "npc_royalguard_t"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_royalguard_t", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(3) 
	end 

	if (not DL:isConditionFulfilled("npc_royalguard_t", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Royalguard_Hey") -- Hey you! How dare you walk in here without permission!
		DL:addConditionProgress("npc_royalguard_t", "talked")
		DL:addNode()


		DL:createNPCNode(2, -2, "DL_Royalguard_Hey2") -- I really hope you have a good reason, otherwise I'll have to throw you out immediately.
		DL:addNode()

	end


	DL:createChoiceNode(3)
	if (not DL:isConditionFulfilled("npc_royalguard_t", "velius")) then 
		DL:addChoice(4, "DL_Choice_Velius") -- I'm looking for some mage called "Velius".
	end
	if (not DL:isConditionFulfilled("npc_royalguard_t", "king")) then 
		DL:addChoice(5, "DL_Choice_King") -- I'd like to have an audience with the king.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_royalguard_t", "velius")) then 

		DL:createNPCNode(4, -2, "DL_Royalguard_Velius") -- The royal highmage is very busy. He does not receive any visitors.
		DL:addConditionProgress("npc_royalguard_t", "velius")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_royalguard_t", "king")) then 

		DL:createNPCNode(5, 6, "DL_Royalguard_King") -- And why should our highness King Logan III waste his time talking to you?
		DL:addNode()


		DL:createChoiceNode(6)
		if (not DL:isConditionFulfilled("npc_royalguard_t", "charming")) then 
			DL:addChoice(7, "DL_Choice_Charming") -- I'd love to get to know the king. 
		end
		if (not DL:isConditionFulfilled("npc_royalguard_t", "none_business")) then 
			DL:addChoice(8, "DL_Choice_NoneBusiness") -- That's none of your business. I only talk to the king himself.
		end
		DL:addChoice(9, "DL_Choice_Mark") -- I'm a member. (Show the barely visible mark on your wrist)
		DL:addChoice(-2, "DL_Choice_Back") -- Ehm... Never mind. [BACK]
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_royalguard_t", "charming")) then 

			DL:createNPCNode(7, -2, "DL_Royalguard_Charming") -- Well, I'm pretty sure he doesn't share this intention. Bugger off!
			DL:addConditionProgress("npc_royalguard_t", "charming")
			DL:gotoNode(6)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_royalguard_t", "none_business")) then 

			DL:createNPCNode(8, -2, "DL_Royalguard_NoneBusiness") -- Hm. Everyone could say that. We don't just let anyone to the king. 
			DL:addConditionProgress("npc_royalguard_t", "none_business")
			DL:gotoNode(6)
			DL:addNode()

		end


		DL:createNPCNode(9, 10, "DL_Royalguard_Mark") -- If that's so... (he stares at your mark) ...you can have a very short audience.
		DL:addNode()


		DL:createNPCNode(10, -1, "DL_Royalguard_Mark2") -- Follow me.
		DL:addConditionProgress("default", "king_open")
		DL:addConditionProgress("npc_royalguard_t", "king")
		DL:addNode()

	end

end