-- Dialogue for NPC "npc_lua"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_lua", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(5) 
	end 

	if (not DL:isConditionFulfilled("npc_lua", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Lua_Hello") -- Oh, hello. It's very uncommon to see a mage with such a special aura. You're an elementalist, right?
		DL:addConditionProgress("npc_lua", "talked")
		DL:addNode()


		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_Elementalist") -- Yes, you're right. How do you know?
		DL:addChoice(4, "DL_Choice_Flirt") -- It's also uncommon to meet such a beautiful lady here.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Lua_Elementalist") -- I'm a clairvoyant. (Smiles mysteriously)
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Lua_Flirt") -- (Blushes) Oh, thank you. What a pity, my skills as a clairvoyant only show me your aura, not your thoughts.
		DL:addConditionProgress("npc_lua", "flirt")
		DL:addReputationProgress("necromancer", 5)
		DL:addNode()

	end


	DL:createChoiceNode(5)
	if (not DL:isConditionFulfilled("npc_lua", "clairvoyant")) then 
		DL:addChoice(6, "DL_Choice_Clairvoyant") -- What does it mean to be a clairvoyant?
	end
	if (DL:isConditionFulfilled("npc_lua", "clairvoyant") and not DL:isConditionFulfilled("npc_lua", "my_aura")) then 
		DL:addChoice(7, "DL_Choice_MyAura") -- What colour does my aura have then?
	end
	if (not DL:isConditionFulfilled("npc_lua", "no_necromancer")) then 
		DL:addChoice(8, "DL_Choice_NoNecromancer") -- You don't look like a necromancer...
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_lua", "clairvoyant")) then 

		DL:createNPCNode(6, -2, "DL_Lua_Clairvoyant") -- Most mages only feel the power of someone's aura. People like me can see their colour - the mage's affinity to a type of magic.
		DL:addConditionProgress("npc_lua", "clairvoyant")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_lua", "clairvoyant") and not DL:isConditionFulfilled("npc_lua", "my_aura")) then 

		DL:createNPCNode(7, -2, "DL_Lua_MyAura") -- For me, it appears in a fiery red. Exceptionally beautiful to look at.
		DL:addConditionProgress("npc_lua", "my_aura")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lua", "no_necromancer")) then 

		DL:createNPCNode(8, 9, "DL_Lua_NoNecromancer") -- Just because I don't wear their robes? Well, to be honest, I just came here to read.
		DL:addConditionProgress("npc_lua", "no_necromancer")
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Lua_NoNecromancer2") -- I love this library. So much knowledge that would be burned by the clerics if they got the chance...
		DL:addNode()

	end

end