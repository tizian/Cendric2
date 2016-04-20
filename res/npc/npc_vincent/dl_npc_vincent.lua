-- Dialogue for NPC "Vincent"
loadDialogue = function(DL)

		if (DL:hasItem("pe_feudalfire",1) and not DL:isConditionFulfilled("npc_vincent","talked")) then 
			DL:createNPCNode(0, 1, "DL_Vincent_Stop") -- Hey you, psst! You just got some Feudal Fire, I see? Are you planning to take that to the Elder Rhendal?
			DL:addConditionProgress("npc_vincent","talked")
			DL:addNode()
			
			DL:createChoiceNode(1)
			DL:addChoice(10, "DL_Choice_YesWhy") -- Yes, why do you ask?
			DL:addChoice(2, "DL_Choice_NoneOfYourBusiness") -- I think that's none of your business.
			DL:addChoice(7, "DL_Choice_NoForMyself") --  No, I bought that for myself.
			DL:addNode()
			
			DL:createNPCNode(2, 3, "DL_Vincent_NoneOfMyBusiness") -- D'you think so? Well, maybe it is... maybe it isn't. Who are you to decide?
			DL:addNode()
			
			DL:createChoiceNode(3)
			DL:addChoice(10, "DL_Choice_WhatDoYouWant") -- (Sigh) What do you want, then?
			DL:addChoice(4, "DL_Choice_Agressive") -- I'm the one who's going to kick your butt if you don't leave me alone.
			DL:addChoice(5, "DL_Choice_GoAway") --  I think I can decide for myself. Just go away.
			DL:addNode()
			
			DL:createNPCNode(4, -1, "DL_Vincent_Disgruntled") -- (Smirks) You know you should't talk to strangers like that. You might get yourself in trouble... 
			DL:addConditionProgress("npc_vincent","disgruntled")
			DL:addNode()
			
			DL:createNPCNode(5, 6, "DL_Vincent_Defected") -- What a pity. You don't even want to hear what I'd like to tell you?
			DL:addNode()
			
			DL:createChoiceNode(6)
			DL:addChoice(10, "DL_Choice_WhatDoYouWant") -- (Sigh) What do you want, then?
			DL:addChoice(4, "DL_Choice_Agressive") -- I'm the one who's going to kick your butt if you don't leave me alone.
			DL:addNode()
			
			if (DL:isQuestState('broken_bridge','started')) then
				DL:createNPCNode(7, 10, "DL_Vincent_YouAreLying") -- Interesting. Very interesting. But you don't need to lie to me. I know what the Elder is up to.
				DL:addNode()
			
				DL:createNPCNode(10, 11, "DL_Vincent_RhendalIsDrinker") -- Hm, It's not the first time he makes someone buy a bottle for him. He told you he's going to use it for "alchemistic purposes", ain't he. But that's a lie, he just likes getting drunk from time to time.
				DL:addNode()
			else
				DL:createNPCNode(7, 10, "DL_Vincent_Interesting") -- Interesting. But I need to tell you something.
				DL:addNode()
			
				DL:createNPCNode(10, 11, "DL_Vincent_NoQuest") -- The Elder loves this schnapps. If you bring it to him anyway, he'll take it gladly, you know. Even an alchemist likes getting drunk from time to time.
				DL:addNode()
			end
			
			DL:createNPCNode(11, 12, "DL_Vincent_Proposal") -- And that's pretty harmful to the people of the village - A drunk alchemist is no good alchemist. What about we give the Elder a little... suprise? (Smiles)
			DL:addNode()
			
			DL:createChoiceNode(12)
			DL:addChoice(13, "DL_Choice_WhatDoYouMean") -- What do you mean with "suprise"?
			DL:addChoice(14, "DL_Choice_Defect") -- No, I'm sorry, I'm not interested.
			DL:addChoice(20, "DL_Choice_Cooperate") --  Sure, sounds interesting.
			DL:addNode()
			
			DL:createNPCNode(14, -1, "DL_Vincent_Defect") -- Well, your decision is always the right decision. See you later, then...
			DL:addNode()
			
			DL:createNPCNode(13, 15, "DL_Vincent_ThePlan") -- I can mix something in the bottle that makes the booze disgusting. That will keep him from further drinking... at least for a while.
			DL:addNode()
			
			DL:createChoiceNode(15)
			DL:addChoice(14, "DL_Choice_Defect") -- No, I'm sorry, I'm not interested.
			DL:addChoice(14, "DL_Choice_DontBelieve") -- I don't believe you at all. I won't help you.
			DL:addChoice(20, "DL_Choice_Cooperate") --  Sure, sounds interesting. Here, take the bottle.
			DL:addNode()
			
			DL:createNPCNode(20, -1, "DL_Vincent_Cooperation") -- Very good. (Opens bottle and puts something in it) That should do it. (Closes bottle) Come back to me after you've done your job. I will pay you.
			DL:addConditionProgress("npc_vincent","cooperated")
			if (DL:isQuestState('broken_bridge','started')) then
				DL:addQuestDescription("broken_bridge", 1)
			end
			DL:removeItem("pe_feudalfire",1)
			DL:addItem("qe_spoiledfeudalfire",1)
			DL:changeQuestState("spoiled_fire", "started")
			DL:addNode()
			
		elseif (not DL:isConditionFulfilled("npc_vincent","talked")) then
			DL:createNPCNode(0, -1, "DL_Vincent_NothingToSay") -- ... 
			DL:addNode()
		elseif (DL:isConditionFulfilled("npc_vincent", "cooperated")) then

			if (DL:isQuestState("spoiled_fire", "started") and not DL:isConditionFulfilled("npc_rhendal", "spoiled_schnapps")) then 
			
				DL:createNPCNode(0, -1, "DL_Vincent_DoYourDuty") -- What are you waiting for? Go and fulfill your duty. 
				DL:addNode()
				
			elseif (DL:isQuestState("spoiled_fire", "started") and DL:isConditionFulfilled("npc_rhendal", "spoiled_schnapps")) then
			
				DL:createNPCNode(0, -1, "DL_Vincent_GoodJob") -- I know that you have carried out my task. Very good. You might be of use. 
				DL:changeQuestState("spoiled_fire", "completed")
				DL:addGold(50)
				DL:addReputationProgress("thief", 10)
				DL:addNode()
				
			else
				-- vincent is going...
				DL:createNPCNode(0, -1, "DL_Vincent_SeeYouLater") -- See you later...
				DL:addNode()
			end
		else 
			-- vincent is going...
			DL:createNPCNode(0, -1, "DL_Vincent_SeeYouLater") -- See you later...
			DL:addNode()
		end
		
		DL:setRoot(0)

    end