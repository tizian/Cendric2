-- Dialogue for NPC "npc_jason"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jason", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_jason", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Jason_Hello") -- Hm? Just because Rana loves to talk does not mean I do. Keep it short.
		DL:addConditionProgress("npc_jason", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (DL:isQuestState("jasons_glaive", "void")) then 
		DL:addChoice(3, "DL_Choice_JasonQuest") -- Can I help you somehow?
	end
	if (DL:isQuestState("jasons_glaive", "started") and DL:isQuestComplete("jasons_glaive")) then 
		DL:addChoice(5, "DL_Choice_GargoyleDust") -- I got some gargoyle dust for you.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("jasons_glaive", "void")) then 

		DL:createNPCNode(3, 4, "DL_Jason_JasonQuest") -- Hrm. Yes, actually, you can. My new weapon broke and it can only be fixed with enough gargoyle dust.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Jason_JasonQuest2") -- Some gargoyles live in the old temple in the North. If you bring me some gargoyle dust, you can have my old glaive in exchange.
		DL:changeQuestState("jasons_glaive", "started")
		DL:addNode()

	end

	if (DL:isQuestState("jasons_glaive", "started") and DL:isQuestComplete("jasons_glaive")) then 

		DL:createNPCNode(5, 6, "DL_Jason_GargoyleDust") -- Oh? What a surprise. I never thought you'd be able to kill them.
		DL:removeItem("mi_gargoyledust", 3)
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Jason_GargoyleDust2") -- Yeah. But promise is promise. Here, take my old glaive. It's still a good weapon.
		DL:changeQuestState("jasons_glaive", "completed")
		DL:addItem("we_jasonsglaive", 1)
		DL:addNode()

	end

end