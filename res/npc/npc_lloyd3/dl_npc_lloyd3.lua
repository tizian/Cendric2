-- Dialogue for NPC "npc_lloyd3"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_lloyd3", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:isQuestState("ininas_abduction", "void")) then 
		DL:setRoot(4) 
	else 
		DL:setRoot(10) 
	end 

	if (not DL:isConditionFulfilled("npc_lloyd3", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Lloyd_Talk") -- Here you are. I was looking for you.
		DL:addConditionProgress("npc_lloyd3", "talked")
		DL:changeQuestState("further_investigation", "completed")
		DL:addNode()


		DL:createNPCNode(2, 3, "DL_Lloyd_Talk2") -- One of Inina's guards just returned and told me about that secret passage. But - much more urgent now - he returned without her! 
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Lloyd_Talk3") -- Inina was abducted whilst you explored the sewers. You shouldn't have left her alone!
		DL:addNode()

	end

	if (DL:isQuestState("ininas_abduction", "void")) then 

		DL:createChoiceNode(4)
		if (not DL:isConditionFulfilled("npc_lloyd3", "paladin")) then 
			DL:addChoice(5, "DL_Choice_Paladin") -- She wasn't alone, there was a paladin guarding her.
		end
		DL:addChoice(6, "DL_Choice_WhoAbducted") -- Who abducted her?
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_lloyd3", "paladin")) then 

			DL:createNPCNode(5, -2, "DL_Lloyd_Paladin") -- Yes, I'm well aware of this. He was knocked out by her abductors though.
			DL:addConditionProgress("npc_lloyd3", "paladin")
			DL:addNode()

		end


		DL:createNPCNode(6, 7, "DL_Lloyd_WhoAbducted") -- I'd like to know that too. I already talked to the paladin who was accompanying her.
		DL:addNode()


		DL:createNPCNode(7, 8, "DL_Lloyd_WhoAbducted2") -- Before he was knocked out, he could see three hooded people.
		DL:addNode()


		DL:createNPCNode(8, 9, "DL_Lloyd_WhoAbducted3") -- I've recently heard that three mercenaries are currently in the Basilisk Inn - they are our primary suspects. Go and see if they're still there.
		DL:changeQuestState("ininas_abduction", "started")
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Lloyd_WhoAbducted4") -- But be careful when questioning them - we don't want to cause a stir.
		DL:addNode()

	end


	DL:createChoiceNode(10)
	if (not DL:isConditionFulfilled("npc_lloyd3", "why_paladin")) then 
		DL:addChoice(11, "DL_Choice_WhyPaladin") -- Why can't you send her guard? It's his mistake, after all.
	end
	DL:addChoice(-1, "DL_Choice_Okay") -- Yes, Commander Lloyd.
	if (not DL:isConditionFulfilled("npc_lloyd3", "Choice-2")) then 
		DL:addChoice(-1, "DL_Choice_Okay2") -- I'll see what I can do.
	end
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_lloyd3", "why_paladin")) then 

		DL:createNPCNode(11, -2, "DL_Lloyd_WhyPaladin") -- He's still not very well. Furthermore, they would certainly recognise him.
		DL:addConditionProgress("npc_lloyd3", "why_paladin")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lloyd3", "Choice-2")) then 
	end

end