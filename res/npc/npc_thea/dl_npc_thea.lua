-- Dialogue for NPC "npc_thea"
loadDialogue = function(DL)

	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_thea", "who_are_you")) then 
		DL:addChoice(1, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_thea", "what_are_you_doing")) then
		DL:addChoice(10, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end
	if (not DL:isConditionFulfilled("npc_thea", "mages")) then
		DL:addChoice(30, "DL_Choice_Mages") -- What do you think about mages?
	end
	if (not DL:isConditionFulfilled("npc_thea", "gandria") and DL:isConditionFulfilled("npc_thea", "what_are_you_doing")) then
		DL:addChoice(40, "DL_Choice_Gandria") -- What do you know about the city Gandria?
	end
    if (DL:isQuestState("theas_dream", "started") and DL:isQuestDescriptionUnlocked("theas_dream", 1)) then
		DL:addChoice(50, "DL_Choice_Syrah") -- I found a job for you in Gandria.
	end

	DL:addChoice(-1, "") --  ""
	DL:addNode()
	
	if (not DL:isConditionFulfilled("npc_thea", "who_are_you")) then
		DL:createNPCNode(1, 2, "DL_Thea_IAm") -- My name is Thea... and you?
		DL:addConditionProgress("npc_thea", "who_are_you")
		DL:addNode()
	
		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_Cendric") -- I'm Cendric. Nice to meet you.
		DL:addChoice(4, "DL_Choice_DoesntMatter") -- My name isn't of importance.
		DL:addNode()
	
		DL:createNPCNode(3, -2, "DL_Thea_Nice") -- Nice to meet you, too!
		DL:addNode()
	
		DL:createNPCNode(4, -2, "DL_Thea_Pity") -- Pity, I would have loved to know it.
		DL:addNode()
	end

	if (not DL:isConditionFulfilled("npc_thea", "what_are_you_doing")) then
		DL:createNPCNode(10, 11, "DL_Thea_WhatAmIDoing") -- I had to deliver some goods from the farmers and then decided to stay for a while.
		DL:addNode()
	
		DL:createChoiceNode(11)
		if (not DL:isConditionFulfilled("npc_thea", "traveling_alone")) then
			DL:addChoice(12, "DL_Choice_VeryAlone") -- You are traveling alone?
		end
		if (not DL:isConditionFulfilled("npc_thea", "work_farmers")) then
			DL:addChoice(13, "DL_Choice_WorkForFarmers") -- Do you work for the farmers?
		end
		if (DL:isConditionFulfilled("npc_thea", "work_farmers") or DL:isConditionFulfilled("npc_thea", "traveling_alone")) then
			DL:addChoice(16, "DL_Choice_WhatIsDangerous") -- What is dangerous out here, then?
		end
		DL:addNode()
	
		DL:createNPCNode(12, 11, "DL_Thea_VeryAlone") -- Yes, I always travel on my own. But it has become dangerous lately outside of the city walls of Gandria. I'd rather take someone with me in the future.
		DL:addConditionProgress("npc_thea", "traveling_alone");
		DL:gotoNode(11)
		DL:addNode()
	
		DL:createNPCNode(13, 11, "DL_Thea_WorkForFarmers") -- Yes, I'm a maid at Ivo's farm. But I'd rather find work in the city of Gandria. It's safe there, at least.
		DL:addConditionProgress("npc_thea", "work_farmers");
        DL:changeQuestState("theas_dream","started")
		DL:gotoNode(11)
		DL:addNode()
		
		DL:createNPCNode(16, 17, "DL_Thea_Beast") -- There is a beast lurking around, and it must be pretty hungry, as most of our sheeps are gone.
		DL:addNode()
        
        DL:createNPCNode(17, 18, "DL_Thea_Beast2") -- Nobody has really seen it so far but I swear, I've seen a huge shadow disappear into the North last night.
		DL:addNode()
        
        DL:createNPCNode(18, -2, "DL_Thea_Beast3") -- And in the morning, another sheep was missing.
		if (not DL:isQuestState("monster_problem", "started")) then
			DL:changeQuestState("monster_problem","started")
		end
		DL:addQuestDescription("monster_problem", 1)
		DL:addConditionProgress("npc_thea", "what_are_you_doing")
		DL:addNode()
	end
	
	if (not DL:isConditionFulfilled("npc_thea", "mages")) then
		DL:createNPCNode(30, 31, "DL_Thea_Mages") -- Well, most of them are quite reasonable people. They lead our kingdom, at least.
		DL:addConditionProgress("npc_thea", "mages")
		DL:addNode()
		
		DL:createNPCNode(31, -2, "DL_Thea_Mages2") -- And I really think that we own them much - the farmer Ivo told me that in the past, there were even mages that controlled the weather to help farmers like him.
		DL:addNode()
	end
	
	if (not DL:isConditionFulfilled("npc_thea", "gandria") and DL:isConditionFulfilled("npc_thea", "what_are_you_doing")) then
		DL:createNPCNode(40, -1, "DL_Thea_Gandria") -- It's the capital city of our kingdom. Where the king resides - and all the important people, of course. Ah, I wish I could live there!
		DL:addConditionProgress("npc_thea", "gandria")
		DL:addNode()

	end
    
    if (DL:isQuestState("theas_dream", "started") and DL:isQuestDescriptionUnlocked("theas_dream", 1)) then
		DL:createCendricNode(50, 51, "DL_Cendric_Syrah") -- The alchemist Syrah would take you as an apprentice.
		DL:addNode()
        
        DL:createNPCNode(51, 52, "DL_Thea_Syrah") -- Oh, really? I always wanted to work with herbs and potions. Thank you so much!
		DL:addNode()
        
        DL:createNPCNode(52, 53, "DL_Thea_Syrah2") -- I'll tell Ivo and then travel to Gandria as fast as possible.
		DL:addNode()
        
        DL:createNPCNode(53, -2, "DL_Thea_Syrah3") -- Don't forget to visit me when I'm there!
        DL:addQuestDescription("theas_dream", 2)
        DL:changeQuestState("theas_dream", "completed")
		DL:addNode()
	end
	
	DL:setRoot(0)
	
end	
	