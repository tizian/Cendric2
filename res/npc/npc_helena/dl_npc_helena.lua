-- Dialogue for NPC "npc_helena"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_helena", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_helena", "old_things")) then 
		DL:setRoot(4) 
	elseif (DL:isConditionFulfilled("npc_helena", "leeroy_chicken") and DL:isQuestState("leeroy_chicken", "started") and DL:isConditionFulfilled("boss", "BossMercenaries") and not DL:isConditionFulfilled("npc_helena", "cooked_chicken")) then 
		DL:setRoot(16) 
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


		DL:createNPCNode(3, -2, "DL_Helena_TellMe2") -- You were in quite a hurry, only left your luggage here and then you rushed out again to fetch something you forgot from the ship... And I've never seen you again.
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
		DL:changeQuestState("a_new_trace", "started")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_helena", "leeroy_chicken") and DL:isQuestState("leeroy_chicken", "started") and DL:isConditionFulfilled("boss", "BossMercenaries") and not DL:isConditionFulfilled("npc_helena", "cooked_chicken")) then 

		DL:createNPCNode(16, 17, "DL_Helena_KilledMercenaries") -- Well... I didn't expect you to kill them but... It's also a way of getting rid of them. You'll get your chicken, of course...
		DL:addNode()


		DL:createNPCNode(17, -2, "DL_Helena_LeeroyMercenaries2") -- 
		DL:addItem("qe_leeroychicken", 1)
		DL:addConditionProgress("npc_helena", "cooked_chicken")
		DL:addNode()

	end


	DL:createChoiceNode(7)
	if (not DL:isConditionFulfilled("npc_helena", "sold_things")) then 
		DL:addChoice(11, "DL_Choice_SoldThings") -- Whom did you sell my stuff?
	end
	if (not DL:isConditionFulfilled("npc_helena", "leeroy_chicken") and DL:isQuestState("leeroy_chicken", "started") and not DL:isConditionFulfilled("boss", "BossMercenaries")) then 
		DL:addChoice(12, "DL_Choice_LeeroyChicken") -- I've heard of your famous special chicken...
	end
	if (not DL:isConditionFulfilled("npc_helena", "leeroy_chicken") and DL:isQuestState("leeroy_chicken", "started") and DL:isConditionFulfilled("boss", "BossMercenaries") and not DL:isConditionFulfilled("npc_helena", "cooked_chicken")) then 
		DL:addChoice(14, "DL_Choice_LeeroyChicken") -- 
	end
	DL:addChoice(10, "DL_Choice_Trade") -- What can I get here?
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_helena", "sold_things")) then 

		DL:createNPCNode(11, -1, "DL_Helena_SoldThings") -- Oh, I don't remember. But I've only sold some robes and hats anyway.
		DL:addConditionProgress("npc_helena", "sold_things")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_helena", "leeroy_chicken") and DL:isQuestState("leeroy_chicken", "started") and not DL:isConditionFulfilled("boss", "BossMercenaries")) then 

		DL:createNPCNode(12, 13, "DL_Helena_LeeroyChicken") -- And now you want some. Of course. But I can't cook for you at the moment.
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Helena_LeeroyChicken2") -- Those mercenaries just won't leave and they scare off my guests. If you manage to make them leave my inn, I'll cook a chicken for you.
		DL:addQuestDescription("leeroy_chicken", 1)
		DL:addConditionProgress("npc_helena", "leeroy_chicken")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_helena", "leeroy_chicken") and DL:isQuestState("leeroy_chicken", "started") and DL:isConditionFulfilled("boss", "BossMercenaries") and not DL:isConditionFulfilled("npc_helena", "cooked_chicken")) then 

		DL:createNPCNode(14, 15, "DL_Helena_LeeroyMercenaries") -- You're lucky. I'll cook one for you because you helped me to get rid of those nasty mercenaries who scared off my customers.
		DL:addNode()


		DL:createNPCNode(15, -2, "DL_Helena_LeeroyMercenaries2") -- (Helena grills a chicken and it smells delicious) Here, for you. Enjoy your meal.
		DL:addConditionProgress("npc_helena", "cooked_chicken")
		DL:addItem("qe_leeroychicken", 1)
		DL:addNode()

	end


	DL:createTradeNode(10, -2)
	DL:addNode()

end