-- Dialogue for NPC "npc_edmond"
loadDialogue = function(DL)
		if (not DL:isConditionFulfilled("default","tutorial_complete")) then
			if (not DL:isConditionFulfilled("npc_edmond", "talked_to")) then
				DL:createNPCNode(0, 1, "DL_Edmond_Hello") -- Hey you, stop! What are you doing here, so very alone? It's dangerous out here.
				DL:addConditionProgress("npc_edmond", "talked_to")
				DL:addNode()
			
				DL:setRoot(0)
			
				DL:createChoiceNode(1)
				DL:addChoice(2, "DL_Choice_Exploring") -- Dunno... Exploring, I guess?
				DL:addChoice(3, "DL_Choice_WhatIsDangerous") --  What exactly is dangerous?
				DL:addNode()
				
				DL:createNPCNode(3, 5, "DL_Edmond_DangerousIs") -- These woods are not safe for a young man like you on his own. There are creatures out here and, in addition, big rocks have fallen down from the hill in the east that are blocking our way back to the village.
				DL:addNode()
				
				DL:createNPCNode(2, 4, "DL_Edmond_JustExploring") -- So, just exploring? It is unusual that I meet someone out here that I don't know. You are not a citizen of the village, otherwise I would have seen you before.
				DL:addNode()
				
				DL:createChoiceNode(4)
				DL:addChoice(6, "DL_Choice_TheVillage") -- What village?
				DL:addChoice(3, "DL_Choice_WhatIsDangerous") --  What exactly is dangerous?
				DL:addNode()
				
				DL:createChoiceNode(5)
				DL:addChoice(6, "DL_Choice_TheVillage") -- What village?
				DL:addChoice(7, "DL_Choice_WhatCanWeDo") --  What can we do about those rocks?
				DL:addNode()
				
				DL:createNPCNode(6, 8, "DL_Edmond_TheVillage") -- A small village in the south. I'm a huntsman with the task of delivering meat to these people. You must have seen it as you've come here! 
				DL:addNode()
				
				DL:createCendricNode(8, 9, "DL_Cendric_Nope") -- I don't recall seeing it...
				DL:addNode()
				
				DL:createNPCNode(9, 10, "DL_Edmond_WouldYouHelp") -- That seems strange to me. But anyway, we're trapped here as long as those rocks block our way out. Would you mind helping me? 
				DL:addNode()
				
				DL:createChoiceNode(10)
				DL:addChoice(7, "DL_Choice_WhatCanWeDo") --  What can we do about those rocks?
				DL:addChoice(12, "DL_Choice_IWillHelp") -- Sure! Tell me what to do
				DL:addChoice(11, "DL_Choice_IWontHelp") --  No, that's your problem
				DL:addNode()
				
			elseif (not DL:isConditionFulfilled("level_entry","tutorial_started")) then
				
				DL:createChoiceNode(50)
				DL:addChoice(7, "DL_Choice_WhatCanWeDo") --  What can we do about those rocks?
				DL:addChoice(-1, "DL_Choice_CU") --  See you later
				DL:addNode()
				
				DL:setRoot(50)
				
			end
				
			DL:createNPCNode(7, 13, "DL_Edmond_WeCanCrush") -- They are not made of very hard material. It may be enough to beat our way through. Would you come with me?
			DL:addNode()
			
			DL:createChoiceNode(13)
			DL:addChoice(12, "DL_Choice_IWillHelp") -- Sure! Tell me what to do
			DL:addChoice(11, "DL_Choice_IWontHelp") --  No, that's your problem
			DL:addNode()
			
			DL:createNPCNode(11, -1, "DL_Edmond_YouWontHelp") -- It is not only my problem. Come back to me if you've changed your mind.
			DL:addNode()

						
			DL:createNPCNode(12, -1, "DL_Edmond_YouWillHelp") -- That's great! You could crush some rocks with your fists, but having a weapon is way more comfortable. Here, take my walking pole and follow me.
			DL:addConditionProgress("level_entry","tutorial_started")
			DL:addItem("we_walkingpole", 1)
			DL:equipItem("we_walkingpole")
			DL:addHint("Inventory")
			DL:addNode()
				
		else 
		
			DL:createChoiceNode(0)
			
			if (DL:isQuestState("meat_delivery", "void")) then
				DL:addChoice(10, "DL_Choice_GiveMeTheMeat") --  I will take the meat to the village for you, give it to me.
			end
			if (DL:isQuestState("the_rats_nest", "void")) then
				DL:addChoice(20, "DL_Choice_ISearchForTheRats") -- I could search for the rats nest for you.  
			end
			if (DL:isQuestState("the_rats_nest", "started") and not DL:isQuestState("the_rats_nest", "completed")) then
				DL:addChoice(30, "DL_Choice_AboutTheRats") -- About the rats...
			end
			if (not DL:isConditionFulfilled("npc_edmond", "who_am_i")) then
				DL:addChoice(40, "DL_Choice_YouHaveNeverSeenMe") -- So, you said you have never seen me before? 
			end
			
			DL:addChoice(-1, "DL_Choice_CU") --  See you later
			DL:addNode()
		
			if (not DL:isConditionFulfilled("npc_edmond", "talked_after_tutorial")) then 
				DL:createNPCNode(1, 2, "DL_Edmond_WeDidIt") -- We did it, thank you. Now our way to the village is clear. But I don't have enough meat yet, so I can't come with you. 
				DL:addConditionProgress("npc_edmond", "talked_after_tutorial")
				DL:addNode()
				
				DL:createNPCNode(2, 3, "DL_Edmond_CanYouBringMeat") -- Would you mind bringing these five pieces of raw meat to the cook in the village for me? Her name is Ingrid.  
				DL:addNode()
				
				DL:createChoiceNode(3)
				DL:addChoice(4, "DL_Choice_GiveMeTheMeat") -- I will take the meat to the village for you, give it to me.
				DL:addChoice(5, "DL_Choice_IHaveOtherPlans") --  No, sorry, I have other plans.
				DL:addNode()
				
				DL:createNPCNode(5, 6, "DL_Edmond_Pity") -- What a pity. I'm sure Ingrid would have given you some of her delicious food in return.
				DL:addNode()
				
				DL:createNPCNode(4, 6, "DL_Edmond_GivesMeat") -- Okay, here you go. And give Ingrid my regards.
				DL:changeQuestState("meat_delivery", "started")
				DL:addItem("fo_rawmeat", 5)
				DL:addNode()
			
				DL:createNPCNode(6, -2, "DL_Edmond_INeedToGoHunt") -- Well, I need to go and hunt some more meat. But I worry about the giant rats we saw, they are going to scare off my prey.  
				DL:addNode()
				
				DL:setRoot(1)
			else 
				DL:setRoot(0)
			end
			
			DL:createNPCNode(10, -1, "DL_Edmond_GivesMeatSecond") -- Oh, so you've changed your decision. You won't regret it. Here you go and give Ingrid my regards.
			DL:changeQuestState("meat_delivery", "started")
			DL:addItem("fo_rawmeat", 5)
			DL:addNode()
			
			DL:createNPCNode(20, -1, "DL_Edmond_GivesRatQuest") -- Really? If you manage to find the nest and kill every rat in there, you will get a great reward. These rats really give me a headache. Here, take this potion, you'll need it.
			DL:changeQuestState("the_rats_nest", "started")
			DL:addItem("fo_lesserhealingpotion", 1)
			DL:addNode()
			
			DL:createChoiceNode(30)
			DL:addChoice(31, "DL_Choice_WhereIsTheNest") -- Do you have an idea where I could start searching for the nest?  	
			
			if (DL:isQuestComplete("the_rats_nest")) then
				DL:addChoice(32, "DL_Choice_IFoundIt") -- The rats won't bother you any more. 
			end
			DL:addNode()
			
			DL:createNPCNode(31, -1, "DL_Edmond_RatNestLocation") -- It seems like they're coming from the north, maybe you should start searching there.
			DL:addNode()
			
			DL:createNPCNode(32, -1, "DL_Edmond_GivesReward") -- Wow, that's amazing! I didn't believe you could make it. Here, take all the gold I have with me. And you can keep my walking pole, of course. You really did me a favor. 
			DL:changeQuestState("the_rats_nest", "completed")
			DL:addGold(23)
			DL:addNode()
			
			DL:createNPCNode(40, 41, "DL_Edmond_WhoAreYou") -- Yes, that's true. Should I know you?
			DL:addNode()
			
			DL:createChoiceNode(41)
			DL:addChoice(42, "DL_Choice_IDontRememberAnything") -- I don't know. I don't remember anything.	
			DL:addChoice(43, "DL_Choice_YouShouldKnowMe") -- Yes. My name is Cendric, better keep that in mind.
			DL:addNode()
			
			if (DL:isQuestState("the_rats_nest", "completed")) then
				DL:createNPCNode(43, -1, "DL_Edmond_IWillRememberYou") -- I will remember you as the guy who has slain the rats for me. I hope that's enough.
			else
				DL:createNPCNode(43, -1, "DL_Edmond_YouAreStrange") -- Ehrm yes. What a strange name. I've never heard about you before, sorry.
			end
			DL:addNode()
			
			DL:createNPCNode(42, -1, "DL_Edmond_TheWiseWillHelpYou") -- That's strange. Maybe you hit your head or you've drunken too much recently? I think the village elder can help you with your problem. You'll find him in his house in the middle of the village.
			DL:addConditionProgress("npc_edmond", "who_am_i")
			DL:addQuestDescription("who_am_i", 1)
			DL:addNode()
			
		end
    end