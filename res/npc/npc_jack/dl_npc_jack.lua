-- Dialogue for NPC "npc_jack"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jack", "who_are_you")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_jack", "elysia")) then 
		DL:setRoot(3) 
	else 
		DL:setRoot(9) 
	end 

	if (not DL:isConditionFulfilled("npc_jack", "who_are_you")) then 

		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_WhoAreYou") -- Who are you?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(2, 4, "DL_Jack_WhoAreYou") -- Hello, kid. I'm Jack. Usually I'd sit in my lighthouse instead of this bawdy house.
		DL:addConditionProgress("npc_jack", "who_are_you")
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Jack_WhoAreYou2") -- But I can't go back as long as that thing flies around there.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_jack", "elysia")) then 

		DL:createChoiceNode(3)
		DL:addChoice(5, "DL_Choice_Elysia") -- What is this "thing" you're talking about?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(5, 7, "DL_Jack_Elysia") -- It looks like a huge bird. Sounds like one too, its shrieks are unbearable.
		DL:addConditionProgress("npc_jack", "elysia")
		DL:addNode()


		DL:createNPCNode(7, 8, "DL_Jack_Elysia2") -- I tried to shoo it away using my frying pan, but it just snatched it away from me and devoured it.
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Jack_Elysia3") -- It was a really good frying pan, what a shame...
		DL:addNode()

	end


	DL:createChoiceNode(9)
	if (not DL:isConditionFulfilled("npc_jack", "why_not_clerics")) then 
		DL:addChoice(6, "DL_Choice_WhyNotClerics") -- Why didn't you ask the paladins for help?
	end
	if (DL:isQuestState("kill_elysia", "void")) then 
		DL:addChoice(11, "DL_Choice_HelpJack") -- I could take care of that problem.
	end
	if (DL:isQuestState("kill_elysia", "started") and DL:isQuestComplete("kill_elysia")) then 
		DL:addChoice(14, "DL_Choice_ElysiaKilled") -- I've found your frying pan.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_jack", "why_not_clerics")) then 

		DL:createNPCNode(6, 10, "DL_Jack_WhyNotClerics") -- Oh, I did! But that sneaky chicken wouldn't show up when they looked for it.
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Jack_WhyNotClerics2") -- Now they just won't believe me anymore. But I really don't want to end up as bird food, I just hope it will leave my lighthouse soon.
		DL:addConditionProgress("npc_jack", "why_not_clerics")
		DL:addNode()

	end

	if (DL:isQuestState("kill_elysia", "void")) then 

		DL:createNPCNode(11, 12, "DL_Jack_HelpJack") -- Really? You'd do me a big favour!
		DL:addNode()


		DL:createNPCNode(12, 13, "DL_Jack_HelpJack2") -- Here, take the key to my lighthouse. I locked it to prevent people getting eaten.
		DL:addItem("ke_lighthouse", 1)
		DL:addQuestDescription("element_master", 2)
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Jack_HelpJack3") -- Oh, and while you're on it, it would be awesome if you could get me my frying pan back.
		DL:changeQuestState("kill_elysia", "started")
		DL:addNode()

	end

	if (DL:isQuestState("kill_elysia", "started") and DL:isQuestComplete("kill_elysia")) then 

		DL:createNPCNode(14, 15, "DL_Jack_ElysiaKilled") -- You really did it! That's my good old pan.
		DL:removeItem("qe_jackpan", 1)
		DL:addNode()


		DL:createNPCNode(15, 16, "DL_Jack_ElysiaKilled2") -- Now that that chicken's gone, I can finally go back to my lighthouse.
		DL:changeQuestState("kill_elysia", "completed")
		DL:addNode()


		DL:createNPCNode(16, -1, "DL_Jack_ElysiaKilled3") -- Here, take the gold I was going to spend here, I don't need it anymore.
		DL:addGold(50)
		DL:addConditionProgress("npc_jack","jack_back")
		DL:addNode()

	end

end