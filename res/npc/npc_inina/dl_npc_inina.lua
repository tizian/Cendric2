-- Dialogue for NPC "npc_inina"
loadDialogue = function(DL)

	if (not DL:isConditionFulfilled("npc_inina", "hi")) then 
		DL:createChoiceNode(0)
		if (not DL:isConditionFulfilled("npc_inina", "hi")) then
			DL:addChoice(2, "DL_Choice_Hey") -- Hey.
		end
		DL:addChoice(-1, "DL_Choice_Bye") -- ""
		DL:addNode()
		
		DL:setRoot(0)
		
		DL:createNPCNode(2, -2, "DL_Inina_Hey") -- Hello, young mage. I'm Inina, High Priestess of the Clerics. How can I help you?
		DL:addConditionProgress("npc_inina", "hi")
		DL:addNode()
		
	else 
	
		DL:createChoiceNode(0)
		if (not DL:isConditionFulfilled("npc_inina", "what_doing")) then
			DL:addChoice(2, "DL_Choice_WhatDoing") -- What are you doing here?
		end
		if (not DL:isConditionFulfilled("npc_inina", "mage")) then
			DL:addChoice(10, "DL_Choice_Mage") -- How do you know I'm a mage?
		end
		if (DL:isConditionFulfilled("npc_inina", "mage") and not DL:isConditionFulfilled("npc_inina", "aura")) then
			DL:addChoice(11, "DL_Choice_Aura") -- Can you "see" my aura?
		end
		if (not DL:isConditionFulfilled("npc_inina", "farmers")) then
			DL:addChoice(12, "DL_Choice_Farmers") -- Why do you oppress the farmers? They need your help.
		end
		DL:addChoice(-1, "DL_Choice_Bye") -- ""
		DL:addNode()

		DL:setRoot(0)
		
		if (not DL:isConditionFulfilled("npc_inina", "what_doing")) then
	
			DL:createNPCNode(2, 3, "DL_Inina_WhatDoing") -- Do you see this shrine? Its power is weakened. We're here to find out why.
			DL:addConditionProgress("npc_inina", "what_doing")
			DL:addNode()

			DL:createChoiceNode(3)
			if (not DL:isConditionFulfilled("npc_inina", "shrine")) then
				DL:addChoice(4, "DL_Choice_Shrine") -- What kind of shrine is this?
			end
			if (not DL:isConditionFulfilled("npc_inina", "power_weakened")) then
				DL:addChoice(5, "DL_Choice_PowerWeakened") -- What could possibly weaken its power?
			end
			if (DL:isConditionFulfilled("npc_inina", "power_weakened")) then
				DL:addChoice(6, "DL_Choice_WhatDo") -- What did you do until now to find the cause?
			end
			DL:addNode()
			
			DL:createNPCNode(4, -2, "DL_Inina_Shrine") -- It's a shrine for the Eternal Light. Mages and commoners can come here and find strength and peace in prayer.
			DL:addConditionProgress("npc_inina", "shrine")
			DL:gotoNode(3)
			DL:addNode()
			
			DL:createNPCNode(5, -2, "DL_Inina_PowerWeakened") -- It seems like there is some magic around that is disturbing the divine magic of the shrine. We need to find the cause of this magic.
			DL:addConditionProgress("npc_inina", "power_weakened")
			DL:gotoNode(3)
			DL:addNode()

			DL:createNPCNode(6, -2, "DL_Inina_WhatDo") -- We've sent one of our paladins in the cave behind the shrine, but he has not returned until now. 
			DL:addConditionProgress("npc_inina", "what_doing")
			DL:addNode()
	
		end
		
		if (not DL:isConditionFulfilled("npc_inina", "farmers")) then
		
			DL:createNPCNode(12, -2, "DL_Inina_Farmers") -- There were some incidents where people left the city and didn't return. We have to tackle this problem first before we take care of some missing sheep.
			DL:addConditionProgress("npc_inina", "farmers")
			DL:addNode()
			
		end
		
		if (not DL:isConditionFulfilled("npc_inina", "mage")) then
		
			DL:createNPCNode(10, -2, "DL_Inina_Mage") -- (Smiles) The Eternal Light is well-disposed towards you. You have a very special aura.
			DL:addConditionProgress("npc_inina", "what_doing")
			DL:addNode()
			
		end
		
		if (DL:isConditionFulfilled("npc_inina", "mage") and not DL:isConditionFulfilled("npc_inina", "aura")) then
		
			DL:createNPCNode(11, -2, "DL_Inina_Aura") -- No, I can only feel it, like most mages do. There are very few mages out there that are able to see auras, the clairvoyant, but I'm not one of them.
			DL:addConditionProgress("npc_inina", "aura")
			DL:addNode()
			
		end
		
		
	end
	
end	