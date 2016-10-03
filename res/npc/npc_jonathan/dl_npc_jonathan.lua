-- Dialogue for NPC "npc_jonathan"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jonathan", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isQuestState("a_new_trace", "completed")) then 
		DL:setRoot(5) 
	elseif (DL:isQuestState("essence_of_fire", "void")) then 
		DL:setRoot(11) 
	else 
		DL:setRoot(17) 
	end 

	if (not DL:isConditionFulfilled("npc_jonathan", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Jonathan_WalkIntoHouse") -- Hey, what are you doing here? You can't just walk into other people's houses.
		DL:addConditionProgress("npc_jonathan", "talked")
		DL:addNode()


		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_DoorWasOpen") -- But... the door was open.
		DL:addChoice(4, "DL_Choice_LookingForYou") -- I was looking for you.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Jonathan_NoExcuNdse") -- Well, that's not really an excuse. Why are you here?
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Jonathan_WhatDoYouWant") -- What do you want from me then?
		DL:addNode()

	end

	if (not DL:isQuestState("a_new_trace", "completed")) then 

		DL:createChoiceNode(5)
		if (DL:hasItem("do_eleletter", 1)) then 
			DL:addChoice(6, "DL_Choice_Letter") -- Someone wants me to meet you. (Show the letter)
		end
		DL:addChoice(-1, "DL_Choice_Leave") -- I was just about to leave.
		DL:addNode()

		if (DL:hasItem("do_eleletter", 1)) then 

			DL:createNPCNode(6, 7, "DL_Jonathan_Letter") -- (Reads the letter carefully) Cyrus! So good to hear from him. So, you're from the academy? How are things going there? And what did Cyrus find out?
			DL:addNode()


			DL:createChoiceNode(7)
			DL:addChoice(8, "DL_Choice_Disappoint") -- I'm sorry to disappoint you...
			DL:addNode()


			DL:createCendricNode(8, 9, "DL_Cendric_TellsTheStory") -- (Cendric tells the story about losing his memory and getting here)
			DL:addNode()


			DL:createNPCNode(9, 10, "DL_Jonathan_Wow") -- Wow... Losing the your whole memory is pretty unusual, even for a mage.
			DL:addNode()


			DL:createNPCNode(10, -2, "DL_Jonathan_Wow2") -- And summoning an iceball is the only spell you remember, right? It's good you came here. We will solve this together.
			DL:changeQuestState("a_new_trace", "completed")
			DL:addNode()

		end

	end

	if (DL:isQuestState("essence_of_fire", "void")) then 

		DL:createChoiceNode(11)
		if (not DL:isConditionFulfilled("npc_jonathan", "academy")) then 
			DL:addChoice(12, "DL_Choice_Academy") -- What academy are you talking about?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "academy") and not DL:isConditionFulfilled("npc_jonathan", "elementalist")) then 
			DL:addChoice(13, "DL_Choice_Elementalists") -- What are elementalists?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "affinity")) then 
			DL:addChoice(14, "DL_Choice_Affinity") -- What does "affinity" mean?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 
			DL:addChoice(15, "DL_Choice_WhatShouldWeDo") -- And what should we do now?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 
			DL:addChoice(18, "DL_Choice_ProveMyself") -- How can I prove myself?
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_jonathan", "academy")) then 

			DL:createNPCNode(12, -2, "DL_Jonathan_Academy") -- The only academy in Admantris where elementalists are educated. It is situated on the other side of the sea. You have obviously studied there.
			DL:addConditionProgress("npc_jonathan", "academy")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "academy") and not DL:isConditionFulfilled("npc_jonathan", "elementalist")) then 

			DL:createNPCNode(13, -2, "DL_Jonathan_Elementalists") -- Mages with an affinity to elemental magic. There are not too much left in Admantris, but I'm one of them... and you are - probably - too.
			DL:addConditionProgress("npc_jonathan", "elementalist")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "affinity")) then 

			DL:createNPCNode(14, -2, "DL_Jonathan_Affinity") -- Each mage is born with a certain affinity to a type of magic. This affinity determines which spells he can learn. We elementalists are lucky - we are able to understand the whole spectrum of magic.
			DL:addConditionProgress("npc_jonathan", "affinity")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 

			DL:createNPCNode(15, 16, "DL_Jonathan_NextStep") -- Hm. I knew that Cyrus was going to send me one of his students to support my research here. But to be honest, I hoped for someone who is a bit more...
			DL:addNode()


			DL:createNPCNode(16, -2, "DL_Jonathan_NextStep2") -- ...qualified. Also, someone who lost his memory is suspicious. Before I let you in on my studies, you should first prove that you're a worthy elementalist.
			DL:addConditionProgress("npc_jonathan", "prove_yourself")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 

			DL:createNPCNode(18, 19, "DL_Jonathan_FireballQuest") -- Every mage is able to learn how to summon an iceball, it's an easy spell. Controlling fire, on the other hand, is considered nearly impossible for mages without an elemental affinity.
			DL:addNode()


			DL:createNPCNode(19, -1, "DL_Jonathan_FireballQuest2") -- Some fire rats from the sewers recently invaded the laboratory in my basement. Find a way to lure them and collect the essence of fire from their lungs. Come back to me when you're done and I'll see if you're able to learn how to summon a fireball.
			DL:addItem("ke_jonathanbasement", 1)
			DL:changeQuestState("essence_of_fire", "started")
			DL:addNode()

		end

	end


	DL:createChoiceNode(17)
	if (DL:isQuestState("essence_of_fire", "started") and DL:isQuestComplete("essence_of_fire")) then 
		DL:addChoice(-1, "DL_Choice_GotFireEssence") -- I got the essences of fire for you.
	end
	DL:addChoice(-1, "DL_Choice_CU") -- See you later.
	DL:addNode()

	if (DL:isQuestState("essence_of_fire", "started") and DL:isQuestComplete("essence_of_fire")) then 
	end

end