-- Dialogue for NPC "npc_jonathan2"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jonathan2", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_jonathan2", "matter")) then 
		DL:setRoot(2) 
	elseif (not DL:isConditionFulfilled("default", "chapter4")) then 
		DL:setRoot(9) 
	else 
		DL:setRoot(21) 
	end 

	if (not DL:isConditionFulfilled("npc_jonathan2", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Jonathan_Hello") -- Ah, finally. We need to talk.
		DL:addConditionProgress("npc_jonathan2", "talked")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_jonathan2", "matter")) then 

		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_Whatup") -- What's the matter?
		if (not DL:isConditionFulfilled("npc_jonathan2", "house")) then 
			DL:addChoice(4, "DL_Choice_House") -- I thought you didn't want to leave your house?
		end
		DL:addNode()


		DL:createNPCNode(3, 5, "DL_Jonathan_Whatup") -- There's been a raid on my house, oh, it was terrible.
		DL:addConditionProgress("npc_jonathan2", "matter")
		DL:addNode()


		DL:createNPCNode(5, 6, "DL_Jonathan_Whatup2") -- I first thought the clerics were responsible, because they told me they worked for the king.
		DL:addNode()


		DL:createNPCNode(6, 7, "DL_Jonathan_Whatup3") -- But the clerics would not wear purple robes... Anyway, they searched through everything and stole our map.
		DL:addNode()


		DL:createNPCNode(7, 8, "DL_Jonathan_Whatup4") -- Finally, they tried to attack me - but I fought back and got away while their burning robes distracted them.
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Jonathan_Whatup5") -- I was afraid they'd come after you too. I'm glad they didn't get you yet.
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_jonathan2", "house")) then 

			DL:createNPCNode(4, -2, "DL_Jonathan_House") -- Yeah that's true. But I had to.
			DL:addConditionProgress("npc_jonathan2", "house")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("default", "chapter4")) then 

		DL:createChoiceNode(9)
		if (DL:isConditionFulfilled("npc_jonathan2", "why_after_me")) then 
			DL:addChoice(10, "DL_Choice_WhyAfterMe") -- Why would they be after me?
		end
		if (not DL:isConditionFulfilled("npc_jonathan2", "describe")) then 
			DL:addChoice(11, "DL_Choice_Describe") -- Can you desribe them?
		end
		if (not DL:isConditionFulfilled("npc_jonathan2", "why_map")) then 
			DL:addChoice(13, "DL_Choice_WhyMap") -- Why did they steal the map?
		end
		DL:addChoice(12, "DL_Choice_Map") -- What should we do now without the map?
		DL:addNode()

		if (DL:isConditionFulfilled("npc_jonathan2", "why_after_me")) then 

			DL:createNPCNode(10, -2, "DL_Jonathan_WhyAfterMe") -- They were clearly looking for elementalists. They needed someone to help them follow the instructions on the map.
			DL:addConditionProgress("npc_jonathan2", "why_after_me")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan2", "describe")) then 

			DL:createNPCNode(11, -2, "DL_Jonathan_Describe") -- There were two of them, mages in long, purple robes. They wielded some kind of purple magic, something I've never seen before.
			DL:addConditionProgress("npc_jonathan2", "describe")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan2", "why_map")) then 

			DL:createNPCNode(13, -2, "DL_Jonathan_WhyMap") -- Maybe they knew more about it than we do... 
			DL:addConditionProgress("npc_jonathan2", "why_map")
			DL:addNode()

		end


		DL:createNPCNode(12, 14, "DL_Jonathan_Map") -- I still know where it lead. That's why I wanted to meet you here.
		DL:addNode()


		DL:createNPCNode(14, 15, "DL_Jonathan_Map2") -- It lead to the Highlands of Admantris. And the key to get there was elemental magic.
		DL:addNode()


		DL:createCendricNode(15, 16, "DL_Cendric_Elemental") -- I've mastered spells from all elements.
		DL:addNode()


		DL:createNPCNode(16, 17, "DL_Jonathan_Element") -- That's awesome. You should be able to unveil the secret of the old elementalists then.
		DL:changeQuestState("element_master", "completed")
		DL:addNode()


		DL:createNPCNode(17, 18, "DL_Jonathan_Element2") -- Follow the path to the highlands and find the temple of the ancients. 
		DL:addNode()


		DL:createNPCNode(18, 19, "DL_Jonathan_Element3") -- Find the secret before those other mages do it.
		DL:changeQuestState("element_secret", "started")
		DL:addNode()


		DL:createNPCNode(19, 20, "DL_Jonathan_Element4") -- I'll do my best to keep them from following you. Hurry!
		DL:addNode()


		DL:createNPCNode(20, -1, "") -- 
		DL:startCutscene("chapter_4")
		DL:addConditionProgress("default", "chapter4")
		DL:addNode()

	end


	DL:createNPCNode(21, -1, "DL_Jonathan_Go") -- What are you waiting for? Hurry!
	DL:addNode()

end