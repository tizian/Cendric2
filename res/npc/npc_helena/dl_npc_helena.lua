-- Dialogue for NPC "npc_helena"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_helena", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_helena", "old_things")) then 
		DL:setRoot(4) 
	else 
		DL:setRoot(7) 
	end 

	if (not DL:isConditionFulfilled("npc_helena", "talked")) then 

		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_TellMe") -- Okay, tell me what you know about me.
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(2, 3, "DL_Helena_TellMe") -- Oh, I don't know much. You've been here like a month ago, but only for a very short time.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Helena_TellMe2") -- You were in quite a hurry, only left your luggage here and then you rushed out again to fetch something from the ship you forgot... And I've never seen you again.
		DL:addConditionProgress("npc_helena", "talked")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_helena", "old_things")) then 

		DL:createChoiceNode(4)
		if (not DL:isConditionFulfilled("npc_helena", "q_ship")) then 
			DL:addChoice(5, "DL_Choice_Ship") -- What ship?
		end
		DL:addChoice(6, "DL_Choice_OldThings") -- Could I have my luggage back then?
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_helena", "q_ship")) then 

			DL:createNPCNode(5, -2, "DL_Helena_Ship") -- I guess you've travelled by ship... You should know this better than me. 
			DL:addConditionProgress("npc_helena", "q_ship")
			DL:addNode()

		end


		DL:createNPCNode(6, 8, "DL_Helena_Luggage") -- Well... To be honest, I didn't reckon that you'd return. So I've sold most of it... But some things are still there.
		DL:addNode()


		DL:createNPCNode(8, 9, "DL_Helena_Luggage2") -- But I've kept the things that I couldn't sell right here at the bar.
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Helena_Luggage3") -- It's your old bag... Useless stuff inside, but it's all I have left. You don't have to worry about the rent of your room, of course.
		DL:addItem("co_travelbag", 1)
		DL:addConditionProgress("npc_helena", "old_things")
		DL:addNode()

	end


	DL:createChoiceNode(7)
	if (not DL:isConditionFulfilled("npc_helena", "sold_things")) then 
		DL:addChoice(11, "DL_Choice_SoldThings") -- Whom did you sell my stuff?
	end
	DL:addChoice(10, "DL_Choice_Trade") -- What can I get here?
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_helena", "sold_things")) then 

		DL:createNPCNode(11, -1, "DL_Helena_SoldThings") -- Oh, I don't remember. But I've only sold some robes and hats anyway.
		DL:addConditionProgress("npc_helena", "sold_things")
		DL:addNode()

	end


	DL:createTradeNode(10, -2)
	DL:addNode()

end