-- Dialogue for NPC "npc_graham"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_graham", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_graham", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Graham_Hey") -- Oh, hello! You really came from that creepy cave? You know that it's forbidden to go there?
		DL:addConditionProgress("npc_graham", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_graham", "who_are_you")) then 
		DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_graham", "forbidden_cave")) then 
		DL:addChoice(4, "DL_Choice_ForbiddenCave") -- Why is it forbitten to go into that cave?
	end
	if (DL:isConditionFulfilled("npc_graham", "who_are_you") and not DL:isConditionFulfilled("npc_graham", "king")) then 
		DL:addChoice(12, "DL_Choice_King") -- A royal bard? So do you know the king?
	end
	if (not DL:isConditionFulfilled("npc_graham", "velius") and DL:isQuestDescriptionUnlocked("find_velius",3)) then 
		DL:addChoice(15, "DL_Choice_Velius") -- Do you know someone called "Velius"?
	end
	if (DL:isQuestState("grahams_dulcimer", "void")) then 
		DL:addChoice(5, "DL_Choice_WhatDoing") -- What are you doing here?
	end
	if (DL:isQuestState("grahams_dulcimer", "started") and DL:isQuestComplete("grahams_dulcimer")) then 
		DL:addChoice(6, "DL_Choice_GotDulcimer") -- I found your instrument. On a tree.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_graham", "who_are_you")) then 

		DL:createNPCNode(3, -2, "DL_Graham_WhoAreYou") -- I'm Graham Teawenth, royal bard. Nice to meet you!
		DL:addConditionProgress("npc_graham", "who_are_you")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_graham", "forbidden_cave")) then 

		DL:createNPCNode(4, 16, "DL_Graham_ForbiddenCave") -- It was an order from the king himself. Apparently, it's dangerous.
		DL:addConditionProgress("npc_graham", "forbidden_cave")
		DL:addNode()


		DL:createNPCNode(16, -2, "DL_Graham_ForbiddenCave2") -- I don't know why they don't just barricade it.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_graham", "who_are_you") and not DL:isConditionFulfilled("npc_graham", "king")) then 

		DL:createNPCNode(12, 13, "DL_Graham_King") -- Logan the Second? Yes, yes I know him. But he was recently replaced by his weird son.
		DL:addConditionProgress("npc_graham", "king")
		DL:addNode()


		DL:createNPCNode(13, 14, "DL_Graham_King2") -- The old king loved to listen to my songs. But the prince... well, he's not interested in music.
		DL:addNode()


		DL:createNPCNode(14, -2, "DL_Graham_King3") -- He doesn't seem interested in anything. Really weird kid. So I'm just sitting here in the gardens and play for the court.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_graham", "velius") and DL:isQuestDescriptionUnlocked("find_velius",3)) then 

		DL:createNPCNode(15, -2, "DL_Graham_Velius") -- Hmm... Yep, that's one of the royal mages. I haven't seen him in a while. He's a pretty reserved kind of guy.
		DL:addConditionProgress("npc_graham", "velius")
		DL:addNode()

	end

	if (DL:isQuestState("grahams_dulcimer", "void")) then 

		DL:createNPCNode(5, 9, "DL_Graham_WhatDoing") -- I'm looking for my instrument, a dulcimer. 
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Graham_WhatDoing2") -- Some big raven stole it and carried it into the castle gardens.
		DL:addNode()


		DL:createNPCNode(10, 11, "DL_Graham_WhatDoing3") -- But what's a royal bard without his instrument? *sniff
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Graham_WhatDoing4") -- So, if you happen to find it and bring it back to me, I'll give you lots of gold!
		DL:changeQuestState("grahams_dulcimer", "started")
		DL:addNode()

	end

	if (DL:isQuestState("grahams_dulcimer", "started") and DL:isQuestComplete("grahams_dulcimer")) then 

		DL:createNPCNode(6, 7, "DL_Graham_GotDulcimer") -- Oooooh! My dulcimer! (Graham takes it and strokes it tenderly)
		DL:removeItem("qe_dulcimer", 1)
		DL:addNode()


		DL:createNPCNode(7, 8, "DL_Graham_GotDulcimer2") -- Thank you so much, I really appreciate your help. Here, take this gold.
		DL:addGold(40)
		DL:changeQuestState("grahams_dulcimer", "completed")
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Graham_GotDulcimer3") -- Now I can finally play it again!
		DL:addNode()

	end

end