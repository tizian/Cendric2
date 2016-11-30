-- Dialogue for NPC "npc_jeremy"
loadDialogue = function(DL) 

	if (DL:isConditionFulfilled("npc_jeremy", "who_are_you") == DL:isConditionFulfilled("npc_jeremy", "buddy_guy")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(9) 
	end 

	if (DL:isConditionFulfilled("npc_jeremy", "who_are_you") == DL:isConditionFulfilled("npc_jeremy", "buddy_guy")) then 

		DL:createChoiceNode(1)
		if (DL:isQuestState("stephens_helmet", "started") and not DL:isConditionFulfilled("npc_jeremy", "helmet")) then 
			DL:addChoice(2, "DL_Choice_Helmet") -- Do you know something about the helmet of a certain paladin?
		end
		if (not DL:isConditionFulfilled("npc_jeremy", "who_are_you")) then 
			DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are you guys?
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestState("stephens_helmet", "started") and not DL:isConditionFulfilled("npc_jeremy", "helmet")) then 

			DL:createNPCNode(2, 8, "DL_Jeremy_Helmet") -- Oh, you mean that pathetic paladin that gambled away everything he had? I'll have to disappoint you, we don't have that helmet anymore.
			DL:addNode()


			DL:createNPCNode(8, -2, "DL_Jeremy_Helmet2") -- We lost it somewhere in the sewers of Gandria. Good luck finding it there, between the nasty rats and oozes...
			DL:addQuestDescription("stephens_helmet", 1)
			DL:addConditionProgress("npc_jeremy", "helmet")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jeremy", "who_are_you")) then 

			DL:createNPCNode(3, -2, "DL_Jeremy_WhoAreYou") -- Just some mercenaries. None of your business, buddy.
			DL:addConditionProgress("npc_jeremy", "who_are_you")
			DL:addNode()

		end

	end


	DL:createChoiceNode(9)
	DL:addChoice(10, "DL_Choice_NotYourBuddy") -- I'm not your buddy, guy.
	DL:addChoice(11, "DL_Choice_Okay") -- Okay...
	DL:addNode()


	DL:createNPCNode(10, 12, "DL_Jeremy_NotYourGuy") -- I'm not your guy, friend.
	DL:addNode()


	DL:createChoiceNode(12)
	DL:addChoice(13, "DL_Choice_NotYourFriend") -- I'm not your friend, buddy.
	DL:addChoice(14, "DL_Choice_Okay") -- 
	DL:addNode()


	DL:createNPCNode(13, 4, "DL_Choice_NotYourBuddy") -- 
	DL:addNode()


	DL:createChoiceNode(4)
	DL:addChoice(6, "DL_Jeremy_NotYourGuy") -- 
	DL:addChoice(5, "DL_Choice_Okay") -- 
	DL:addNode()


	DL:createNPCNode(6, -2, "DL_Choice_NotYourFriend") -- 
	DL:addNode()


	DL:createNPCNode(5, -1, "DL_Jeremy_Nothing") -- 
	DL:addConditionProgress("npc_jeremy", "buddy_guy")
	DL:addNode()


	DL:createNPCNode(14, -1, "DL_Jeremy_Nothing") -- 
	DL:addConditionProgress("npc_jeremy", "buddy_guy")
	DL:addNode()


	DL:createNPCNode(11, -1, "DL_Jeremy_Nothing") -- ...
	DL:addConditionProgress("npc_jeremy", "buddy_guy")
	DL:addNode()

end