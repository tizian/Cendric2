-- Dialogue for NPC "npc_lloyd4"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_lloyd4", "first")) then 
		DL:setRoot(1) 
	elseif (DL:isQuestState("find_velius", "void")) then 
		DL:setRoot(8) 
	elseif (not DL:isConditionFulfilled("npc_lloyd4", "talked")) then 
		DL:setRoot(15) 
	else 
		DL:setRoot(-1) 
	end 

	if (not DL:isConditionFulfilled("npc_lloyd4", "first")) then 

		DL:createNPCNode(1, 2, "DL_Lloyd_Hello") -- What... what happened here?! What did you...
		DL:addConditionProgress("npc_lloyd4", "first")
		DL:addNode()


		DL:createNPCNode(2, 3, "DL_Lloyd_Hello2") -- (Shocked) You killed them? I told you to interrogate them and to be careful! They were our only suspects!
		DL:addNode()


		DL:createChoiceNode(3)
		DL:addChoice(4, "DL_Choice_IDidntMean") -- I didn't mean to!
		DL:addChoice(5, "DL_Choice_CalmDown") -- Calm down, we'll find a solution.
		DL:addChoice(6, "DL_Choice_TheyAttacked") -- They attacked me! It was self-defence.
		DL:addChoice(7, "DL_Choice_TheyDeserved") -- They deserved it.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Lloyd_IDidntMean") -- (Sighs) Throwing your magic at people kills them apparently. Anyway, we've got to find a solution now.
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Lloyd_CalmDown") -- Do you have a suggestion?
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Lloyd_TheyAttacked") -- Hrr. I'll believe you. But this was the last time I asked you to interrogate someone. Anyway, what should we do now?
		DL:addNode()


		DL:createNPCNode(7, -2, "DL_Lloyd_TheyDeserved") -- Maybe they did. But it's not yours to decide about their fate. Anyway, what should we do now?
		DL:addNode()

	end

	if (DL:isQuestState("find_velius", "void")) then 

		DL:createChoiceNode(8)
		DL:addChoice(9, "DL_Choice_FoundLetter") -- I've found this letter in their pockets. (Give letter)
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Lloyd_FoundLetter") -- Interesting. Let me have a look at it.
		DL:removeItem("do_veliusletter", 1)
		DL:addNode()


		DL:createNPCNode(10, 11, "DL_Lloyd_FoundLetter2") -- It seems to be from their boss. (Mumbles) mages... mark... Who is that "V."?
		DL:addNode()


		DL:createCendricNode(11, 12, "DL_Cendric_tldr") -- I didn't read the letter yet...
		DL:addNode()


		DL:createNPCNode(12, 13, "DL_Lloyd_FoundLetter3") -- Here, take it back. Somebody is abducting mages. Inina is not the only one who's missing.
		DL:addItem("do_veliusletter", 1)
		DL:addNode()


		DL:createNPCNode(13, 14, "DL_Lloyd_FoundLetter4") -- I've heard from other cases. We must do something! 
		DL:addQuestProgress("lloyds_plan", "information_gathered")
		DL:changeQuestState("ininas_abduction", "completed")
		DL:addNode()


		DL:createNPCNode(14, -2, "DL_Lloyd_FoundLetter5") -- The letter's author is a certain "V." and he mentions "ruins". Go and find those while I'm going to find out who that "V." is.
		DL:changeQuestState("find_velius", "started")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lloyd4", "talked")) then 

		DL:createChoiceNode(15)
		if (not DL:isConditionFulfilled("npc_lloyd4", "where_search")) then 
			DL:addChoice(16, "DL_Choice_WhereSearch") -- Where should I search for those ruins?
		end
		if (not DL:isConditionFulfilled("npc_lloyd4", "why_work") and not DL:getGuild() == "cleric") then 
			DL:addChoice(17, "DL_Choice_WhyWork") -- And why exactly should I still work for you?
		end
		DL:addChoice(21, "DL_Choice_Bye") -- I'll see what I can do.
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_lloyd4", "where_search")) then 

			DL:createNPCNode(16, -2, "DL_Lloyd_WhereSearch") -- The only ruins that come to my mind are the ones in the mountains, to the West of the marshlands.
			DL:addConditionProgress("npc_lloyd4", "where_search")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_lloyd4", "why_work") and not DL:getGuild() == "cleric") then 

			DL:createNPCNode(17, 18, "DL_Lloyd_WhyWork") -- Admantris is in danger. Gandria is in danger. And you probably are, too. Why would you stop helping me?
			DL:addNode()


			DL:createChoiceNode(18)
			DL:addChoice(19, "DL_Choice_KeepWorking") -- Alright I'll help you.
			DL:addChoice(20, "DL_Choice_StopWorking") -- You can start looking for another idiot to do your job.
			DL:addNode()


			DL:createNPCNode(19, -2, "DL_Lloyd_KeepWorking") -- (Lloyd nods acknowledging)
			DL:addConditionProgress("npc_lloyd4", "why_work")
			DL:addNode()


			DL:createNPCNode(20, -1, "DL_Lloyd_StopWorking") -- I don't need you.
			DL:changeQuestState("find_velius", "failed")
			DL:addConditionProgress("npc_lloyd4", "why_work")
			DL:addConditionProgress("npc_lloyd4", "talked")
			DL:addNode()

		end


		DL:createNPCNode(21, -2, "DL_Lloyd_Bye") -- Be safe.
		DL:addConditionProgress("npc_lloyd4", "talked")
		DL:addNode()

	end

end