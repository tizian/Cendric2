-- Dialogue for NPC "Zeff"
loadDialogue = function(DL)
	
	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_zeff", "who_are_you")) then 
		DL:addChoice(1, "DL_Choice_WhoAreYou") -- Who are you?
		DL:addChoice(2, "DL_Choice_WhatAreYou") -- What are you?
	end
	if (not DL:isConditionFulfilled("npc_zeff", "what_are_you_doing")) then
		DL:addChoice(20, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end
	if (DL:isConditionFulfilled("npc_zeff", "curse") and not DL:isConditionFulfilled("npc_zeff", "curse_talked")) then
		DL:addChoice(-1, "DL_Choice_Curse") -- What's this curse you are talking about?
	end
	if (DL:isConditionFulfilled("npc_zeff", "leatherworker")) then
		DL:addChoice(50, "DL_Choice_Leatherworking") -- Could you craft something for me?
	end

	DL:addChoice(-1, "DL_Choice_CU") --  See you later.
	DL:addNode()
	
	DL:setRoot(0)
	
	if (not DL:isConditionFulfilled("npc_zeff", "what_are_you_doing")) then 
	
		DL:createNPCNode(20, -2, "DL_Zeff_SittingHere") -- I'm sitting here at the fire, hoping not to devour anyone before that curse devours myself.
		DL:addConditionProgress("npc_zeff", "what_are_you_doing")
		DL:addConditionProgress("npc_zeff", "curse")
		DL:addNode()
	
	end

	if (not DL:isConditionFulfilled("npc_zeff", "who_are_you")) then 

		DL:createNPCNode(1, 3, "DL_Zeff_IAmWho") -- I'm Zeff.
		DL:addNode()
		
		DL:createNPCNode(2, 3, "DL_Zeff_IAmWhat") -- (Growls quietly) I see, my outer appearance is striking. 
		DL:addNode()
		
		DL:createNPCNode(3, 4, "DL_Zeff_IAmSuprised") -- I'm suprised you have the courage to talk to me. Most people I meet flee in terror when they see my face.
		DL:addConditionProgress("npc_zeff", "who_are_you")
		DL:addNode()
		
		DL:createChoiceNode(4)
		DL:addChoice(5, "DL_Choice_YouLookNice") -- I don't think you look terrifying.
		DL:addChoice(6, "DL_Choice_YouLookTerrible") -- That's not suprising. You truly look scary.
		DL:addChoice(7, "DL_Choice_IAmNotAfraid") -- I'm not afraid of you.
		DL:addNode()
		
		DL:createNPCNode(5, 10, "DL_Zeff_ThankYouLooks") -- Thank you. You're the first person to say something nice since I was exiled from Gandria.
		DL:addNode()
		
		DL:createNPCNode(6, 10, "DL_Zeff_IAmScary") -- (Hangs his head) Yes I do. That's what the people of Gandria also thought, when they decided to exile me from the city.
		DL:addNode()
		
		DL:createNPCNode(7, 10, "DL_Zeff_YouShouldBe") -- Maybe you should be. The people of Gandria didn't exile me without any reason from the city.
		DL:addNode()
		
		DL:createChoiceNode(10)
		DL:addChoice(13, "DL_Choice_WhyExile") -- Why were you sent into exile?
		DL:addChoice(11, "DL_Choice_Werewolf") -- Are you a werewolf?
		DL:addNode()
		
		DL:createNPCNode(11, 12, "DL_Zeff_Werewolf") -- You've never met a werewolf, have you? A werewolf wouldn't be able to talk to you.
		DL:addNode()
		
		DL:createCendricNode(12, 13, "DL_Choice_WhyExile") -- Why were you sent into exile?
		DL:addNode()
		
		DL:createNPCNode(13, -2, "DL_Zeff_Exile") -- They were regarding me as a threat. And I can't even hold it against them - this curse is a loose cannon.
		DL:addConditionProgress("npc_zeff", "curse")
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_zeff", "leatherworker")) then	
		
		DL:createNPCNode(50, 51, "DL_Zeff_Craft") -- Sure, if you got a fur for me?
		DL:addNode()
		
		DL:createChoiceNode(51)
		if (DL:hasItem("mi_fur_firerat", 1)) then
			DL:addChoice(52, "DL_Choice_FireRatFur") -- Scarf of the Fire Rat (1 Fur of a Fire Rat)
		end
		DL:addChoice(-2, "DL_Choice_NothingToCraft") -- I'll come back later. [Back]
		DL:addNode()
		
		DL:createNPCNode(52, -2, "DL_Zeff_FireRatFur") -- The fur of a fire rat, how nice. It has the ability to protect you against fire. 
		DL:addItem("eq_fireratscarf", 1)
		DL:removeItem("mi_firerat_fur", 1)
		DL:gotoNode(51)
		DL:addNode()
		
	end

end	
	