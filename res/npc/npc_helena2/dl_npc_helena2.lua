-- Dialogue for NPC "npc_helena2"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_helena2", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_helena2", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Helena_Hello") -- Hey, hey you! You're back? I thought you'll never return and pay for your room.
		DL:addConditionProgress("npc_helena2", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_helena2", "who_are_you")) then 
		DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_helena2", "room")) then 
		DL:addChoice(4, "DL_Choice") -- What room?
	end
	if (DL:isConditionFulfilled("npc_helena2", "who_are_you")) then 
		DL:addChoice(5, "DL_Choice_HowYouKnow") -- You know me?
	end
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_helena2", "who_are_you")) then 

		DL:createNPCNode(3, -2, "DL_Helena_Helena") -- I'm Helena, the innkeeper of this tavern. We've met before!
		DL:addConditionProgress("npc_helena2", "who_are_you")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_helena2", "room")) then 

		DL:createNPCNode(4, -2, "DL_Helena_Room") -- Your room in this inn, of course?
		DL:addConditionProgress("npc_helena2", "room")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_helena2", "who_are_you")) then 

		DL:createNPCNode(5, 6, "DL_Helena_ComeIn") -- Yes, I do! But it's been a while. I thought you're already long gone.
		DL:addNode()


		DL:createNPCNode(6, -1, "DL_NPC_ComeIn2") -- So, why don't you come in and I tell you everything inside.
		DL:addNode()

	end

end