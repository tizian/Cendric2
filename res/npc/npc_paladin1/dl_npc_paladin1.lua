-- Dialogue for NPC "npc_paladin1"
loadDialogue = function(DL)
	
	DL:createNPCNode(0, 1, "DL_Paladin1_Hm") -- (Grumpy) Hmmm?
	DL:addNode()

	DL:setRoot(0)
	
	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_paladin1", "who_are_you")) then
		DL:addChoice(2, "DL_Choice_WhoAreYou") -- Who are you? 
	end
	if (not DL:isConditionFulfilled("npc_paladin1", "armor")) then
		DL:addChoice(10, "DL_Choice_Armor") -- Nice armour you got there...  
	end
	if (not DL:isConditionFulfilled("npc_paladin1", "paladin") and DL:isConditionFulfilled("npc_paladin1", "who_are_you")) then
		DL:addChoice(20, "DL_Choice_Paladin") -- How is it like to be a paladin?  
	end
	DL:addChoice(-1, "DL_Choice_Bye") -- Nevermind...  
	DL:addNode()
	
	DL:createNPCNode(2, -2, "DL_Paladin1_IAm") -- I'm a paladin of the Eternal Light, serving our lord and king Logan the Second.
	DL:addConditionProgress("npc_paladin1", "who_are_you")
	DL:addNode()
	
	DL:createNPCNode(10, -2, "DL_Paladin1_Armor") -- Ye, right? But you'll need to work hard to get something like this, so don't even think about it. 
	DL:addConditionProgress("npc_paladin1", "armor")
	DL:addNode()
	
	DL:createNPCNode(20, -2, "DL_Paladin1_PissedOff") -- How is it like to be a little pain in the neck like you? 
	DL:addConditionProgress("npc_paladin1", "paladin")
	DL:addNode()
end	
	