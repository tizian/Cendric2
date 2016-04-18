-- Dialogue for NPC "Vincent"
loadDialogue = function(DL)

		if (DL:isConditionFulfilled("npc_innkeeper","bought_feudal_fire") and not DL:isConditionFulfilled("npc_vincent","talked")) then 
			DL:createNPCNode(0, 1, "DL_Vincent_Stop") -- Hey you, psst! You just got some Feudal Fire, I see? Are you planning to take that to the Elder Rhendal?
			DL:addConditionProgress("npc_vincent","talked")
			DL:addNode()
			
			DL:createChoiceNode(1)
			DL:addChoice(-2, "DL_Choice_YesWhy") -- Yes, why do you ask?
			DL:addChoice(-2, "DL_Choice_NoneOfYourBusiness") -- I think that's none of your business.
			DL:addChoice(-2, "DL_Choice_NoForMyself") --  No, I bought that for myself.
			DL:addNode()
			
		elseif (not DL:isConditionFulfilled("npc_vincent","talked")) then
			DL:createNPCNode(0, -1, "DL_Vincent_NothingToSay") -- ... 
			DL:addNode()
		else
			-- other stuff
			DL:createNPCNode(0, -1, "DL_Vincent_NothingToSay") -- ... 
			DL:addNode()
		end
		
		DL:setRoot(0)

    end