-- Dialogue for NPC "npc_inina"
loadDialogue = function(DL)

	if (not DL:isConditionFulfilled("npc_inina", "hi")) then 
		DL:createChoiceNode(0)
		if (not DL:isConditionFulfilled("npc_inina", "hi")) then
			DL:addChoice(2, "DL_Choice_Hey") -- Hey.
		end
		DL:addChoice(-1, "") -- ""
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
			DL:addChoice(11, "DL_Choice_Aura") -- You can see my aura?
		end
		if (not DL:isConditionFulfilled("npc_inina", "farmers")) then
			DL:addChoice(12, "DL_Choice_Farmers") -- Why do you oppress the farmers? They need your help.
		end
		if (DL:isConditionFulfilled("npc_inina", "farmers") and not DL:isConditionFulfilled("npc_inina", "missing_people")) then
			DL:addChoice(20, "DL_Choice_MissingPeople") -- What do you know about the missing people of Gandria?
		end
		if (DL:isConditionFulfilled("npc_inina", "what_doing") and DL:isQuestState("missing_paladin", "void")) then
			DL:addChoice(30, "DL_Choice_MissingPaladin") -- Maybe I could look for the paladin?
		end
		if (not DL:isQuestState("missing_paladin", "void") and not DL:isQuestState("missing_paladin", "completed")) then
			DL:addChoice(40, "DL_Choice_AboutPaladin") -- About the missing paladin...
		end
		if (not DL:isConditionFulfilled("npc_inina", "learned_light")) then
			DL:addChoice(50, "DL_Choice_CanYouTeachMe") -- Can you teach me something?
		end
		
		DL:addChoice(-1, "") -- ""
		DL:addNode()

		DL:setRoot(0)
		
		if (not DL:isConditionFulfilled("npc_inina", "what_doing")) then
	
			DL:createNPCNode(2, 3, "DL_Inina_WhatDoing") -- Do you see this shrine? Its power is weakened. We're here to find out why.
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
			
			DL:createNPCNode(4, -2, "DL_Inina_Shrine") -- It''s a shrine for the Eternal Light. Mages and commoners can come here and find strength and peace in prayer.
			DL:addConditionProgress("npc_inina", "shrine")
			DL:gotoNode(3)
			DL:addNode()
			
			DL:createNPCNode(5, -2, "DL_Inina_PowerWeakened") -- It seems like there is some magic around that is disturbing the divine magic of the shrine. We need to find the cause of this interfering magic.			
			DL:addConditionProgress("npc_inina", "power_weakened")
			DL:gotoNode(3)
			DL:addNode()

			DL:createNPCNode(6, -2, "DL_Inina_WhatDo") -- We''ve sent one of our paladins in the cave behind the shrine, but he has not yet returned.
			DL:addConditionProgress("npc_inina", "what_doing")
			DL:addNode()
	
		end
		
		if (DL:isConditionFulfilled("npc_inina", "what_doing") and DL:isQuestState("missing_paladin", "void")) then
			
			DL:createNPCNode(30, 31, "DL_Inina_MissingPaladin1") -- Yes, maybe you could help us. The name of the paladin I sent into the cave is Marcus.
			DL:addNode()
			
			DL:createNPCNode(31, 32, "DL_Inina_MissingPaladin2") -- He has taken my pendant with him. A special pendant that holds some divine energy and shines in the dark, so you should be able to find him.
			DL:addNode()
			
			DL:createNPCNode(32, -2, "DL_Inina_MissingPaladin3") -- You''d better take something with you that lights your way. I cannot accompany you, as I''m still trying to purify the magic of the shrine.
			DL:changeQuestState("missing_paladin", "started")
			DL:addNode()
		
		end
		
		if (not DL:isConditionFulfilled("npc_inina", "farmers")) then
		
			DL:createNPCNode(12, -2, "DL_Inina_Farmers") -- There were some incidents where people left the city and didn''t return. We have to tackle this problem first before we take care of some missing sheep.
			DL:addConditionProgress("npc_inina", "farmers")
			DL:addNode()
			
		end
		
		if (DL:isConditionFulfilled("npc_inina", "farmers") and not DL:isConditionFulfilled("npc_inina", "missing_people")) then
		
			DL:createNPCNode(20, -2, "DL_Inina_MissingPeople") -- We suspect that these incidents are connected to the strange magical energy we are observing here. But this has yet to be confirmed.
			DL:addQuestDescription("gates_of_gandria", 1)
			DL:addConditionProgress("npc_inina", "missing_people")
			DL:addNode()
			
		end
		
		if (not DL:isConditionFulfilled("npc_inina", "mage")) then
		
			DL:createNPCNode(10, -2, "DL_Inina_Mage") -- (Smiles) The Eternal Light is well-disposed towards you. You have a very special aura.
			DL:addConditionProgress("npc_inina", "mage")
			DL:addNode()
			
		end
		
		if (DL:isConditionFulfilled("npc_inina", "mage") and not DL:isConditionFulfilled("npc_inina", "aura")) then
		
			DL:createNPCNode(11, -2, "DL_Inina_Aura") -- No, I can only feel it. There are very few mages out there that are able to see auras, the clairvoyant. Sadly, I''m not one of them.
			DL:addConditionProgress("npc_inina", "aura")
			DL:addNode()
			
		end
		
		if (not DL:isQuestState("missing_paladin", "void") and not DL:isQuestState("missing_paladin", "completed")) then

			DL:createChoiceNode(40)
			if (not DL:isConditionFulfilled("npc_inina", "where_light")) then
				DL:addChoice(41, "DL_Choice_WhereLight") -- Where can I get something to light my way?
			end
			if (not DL:isConditionFulfilled("npc_inina", "where_search")) then
				DL:addChoice(42, "DL_Choice_WhereSearch") -- Where should I start the search?
			end
			if (not DL:isConditionFulfilled("npc_inina", "whats_reward")) then
				DL:addChoice(43, "DL_Choice_WhatsInForMe") -- What's in for me if I find him?
			end
			if (DL:isQuestComplete("missing_paladin")) then
				DL:addChoice(44, "DL_Choice_FoundPaladin") -- I found the Paladin. He's dead. [Give the pendant]
			end
			DL:addChoice(-2, "DL_Choice_Back") -- [BACK]
			DL:addNode()
			
			DL:createNPCNode(41, -2, "DL_Inina_WhereLight") -- You'll need a torch. I don't have one, but I'm sure that you can buy one at the farm.
			DL:addConditionProgress("npc_inina", "where_light")
			DL:gotoNode(40)
			DL:addNode()
			
			DL:createNPCNode(42, -2, "DL_Inina_WhereSearch") -- He went westward to a cave.
			DL:addConditionProgress("npc_inina", "where_search")
			DL:gotoNode(40)
			DL:addNode()
			
			DL:createNPCNode(43, -2, "DL_Inina_WhatsInForMe") -- Well, as you're a mage, I can teach you the path of the Eternal Light. You'll never need to use a torch again. (Smiles)
			DL:addConditionProgress("npc_inina", "whats_reward")
			DL:gotoNode(40)
			DL:addNode()
			
			DL:createNPCNode(43, 44, "DL_Inina_FoundPaladin") -- Oh no. That's terrible. He was a good man. I'm going to pray for him. Thank you for bringing my pendant back.
			DL:changeQuestState("missing_paladin", "completed")
			DL:removeItem("it_lightpendant", 1)
			DL:addNode()
			
			DL:createNPCNode(44, -2, "DL_Inina_FoundPaladin2") -- We really need to find out what's behind all this.
			DL:addNode()

		end
		
		if (not DL:isConditionFulfilled("npc_inina", "learned_light")) then
		
			if (DL:isQuestState("missing_paladin", "completed")) then
			
				DL:createNPCNode(50, 51, "DL_Inina_LearnLight1") -- Yes, you've proven yourself worthy to learn the way of the Eternal Light.
				DL:addNode()
				
				DL:createNPCNode(51, -1, "DL_Inina_LearnLight2") -- To create a magical light, you'll need a weapon that is capable of focusing divine magic. Then, concentrate the divine energy around you and let it shine. Take this scroll. It will teach you everything you need to know.
				DL:addConditionProgress("npc_inina", "learned_light")
				DL:addItem("sp_light", 1)
				DL:addNode()
			
			else
				
				DL:createNPCNode(50, -2, "DL_Inina_NoTeach") -- We can't tell yet whether your heart is pure. You first have to prove that you are willing to help us.
				DL:addNode()
				
			end
			
			
		end
				
	end
	
end	