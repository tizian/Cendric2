-- Dialogue for NPC "npc_tristan"
loadDialogue = function(DL)

	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_tristan", "who_are_you")) then
		DL:addChoice(2, "DL_Choice_Farmer") -- Is this your farm?
	end
	if (DL:isConditionFulfilled("npc_tristan", "who_are_you") and not DL:isConditionFulfilled("npc_tristan", "merchant")) then
		DL:addChoice(2, "DL_Choice_Merchant") -- Do you trade with the goods of your farm?
	end 
	if (DL:isQuestState("monster_problem", "completed") and not DL:isConditionFulfilled("npc_tristan", "reward")) then
		DL:addChoice(40, "DL_Choice_GiveReward") -- Your father told me to collect my reward from you.
	end 
	if (DL:isConditionFulfilled("npc_ivo", "sheep_asked") and not DL:isQuestState("monster_problem", "complete")) then
		DL:addChoice(10, "DL_Choice_Sheep") -- About your sheep...
	end
	if (DL:isConditionFulfilled("npc_tristan", "merchant")) then
		DL:addChoice(50, "DL_Choice_Trade") -- Show me your wares. [TRADE]
	end 
	
	DL:addChoice(-1, "DL_Choice_Bye") -- Bye.  
	DL:addNode()
	
	DL:setRoot(0)
	
	DL:createTradeNode(50, -2) 
	DL:addNode()
	
	if (DL:isConditionFulfilled("npc_tristan", "who_are_you") and not DL:isConditionFulfilled("npc_tristan", "merchant")) then
	
		DL:createNPCNode(2, -2, "DL_Tristan_Merchant") -- Yes, you can buy meat, hearbs and also some other things here if you have gold.
		DL:addConditionProgress("npc_tristan", "merchant")
		DL:addNode()
		
	end 
	
	if (not DL:isConditionFulfilled("npc_tristan", "who_are_you")) then
	
		DL:createNPCNode(2, 3, "DL_Tristan_Farmer") -- Not yet. It still belongs to my father, Ivo. But in a few years, it's mine.
		DL:addConditionProgress("npc_tristan", "who_are_you")
		DL:addNode()
		
		DL:createChoiceNode(3)
		DL:addChoice(4, "DL_Choice_SoundsGreat") -- Sounds amazing.
		DL:addChoice(5, "DL_Choice_Shepherd") -- So for now you're not more than a shepherd then.
		DL:addNode()
		
		DL:createNPCNode(4, 6, "DL_Tristan_SoundsGreat") -- Right? I'll be very rich someday.
		DL:addNode()
		
		DL:createNPCNode(5, 6, "DL_Tristan_Shepherd") -- Don't be rude, I guess you're only jelly because you don't have a rich father like me.
		DL:addNode()
		
		DL:createChoiceNode(6)
		DL:addChoice(-2, "DL_Choice_Whatever") -- Whatever...
		DL:addChoice(7, "DL_Choice_Rude") -- I don't need a rich father to be something, wally.
		DL:addNode()
		
		DL:createNPCNode(7, -2, "DL_Tristan_Rude") -- How dare you talk to me like that. This won't win you any friends...
		DL:addConditionProgress("npc_tristan", "rude")
		DL:addNode()

	end
	
	if (DL:isQuestState("monster_problem", "completed") and not DL:isConditionFulfilled("npc_tristan", "reward")) then
		
		DL:createNPCNode(40, 41, "DL_Tristan_Reward") -- So, you took care of the sheep thief? I'm impressed.
		DL:addConditionProgress("npc_tristan", "reward")
		DL:addNode()
		
		if (DL:isConditionFulfilled("npc_tristan", "rude")) then
		
			DL:createNPCNode(41, -2, "DL_Tristan_RudeReward") -- You're at least good for something. Ivo offered a reward of 70 gold to the person who'd get rid of the thief. But I don't like how you talked to me earlier... I guess 50 is enough.
			DL:addGold(50)
			DL:addNode()
			
		else 
		
			DL:createNPCNode(41, -2, "DL_Tristan_NormalReward") -- Here, take this gold. And a torch, you might need it. You were really a huge help.
			DL:addGold(70)
			DL:addItem("we_torch", 1)
			DL:addNode()
		
		end
		
	end 
	
	if (DL:isConditionFulfilled("npc_ivo", "sheep_asked") and not DL:isQuestState("monster_problem", "complete")) then
	
		DL:createChoiceNode(10)
		if (not DL:isConditionFulfilled("npc_tristan", "thief_seen")) then
			DL:addChoice(11, "DL_Choice_ThiefSeen") -- Have you seen anything you could tell me about the thief?
		end
		if (DL:isConditionFulfilled("npc_tristan", "thief_seen") and not DL:isConditionFulfilled("npc_tristan", "bandits")) then
			DL:addChoice(12, "DL_Choice_Bandits") -- So you think the thieves are bandits?
		end
		if (DL:isQuestComplete("monster_problem") and not DL:isConditionFulfilled("npc_tristan", "took_care")) then
			DL:addChoice(13, "DL_Choice_MonsterBanished") -- I took care of the sheep thief.
		end
		DL:addChoice(-2, "DL_Choice_Back") -- [BACK]
		DL:addNode()
		
		DL:createNPCNode(11, -2, "DL_Tristan_ThiefSeen") -- Hm, no. The thief only appeared when it was dark. I don't really think it was a wolf, I would have found some carcass of the stolen sheep.
		DL:addConditionProgress("npc_tristan", "thief_seen")
		DL:gotoNode(10)
		DL:addNode()
		
		DL:createNPCNode(12, -2, "DL_Tristan_Bandits") -- Maybe. If you are looking for them, don't search near the city, they prefer hiding in the woods.
		DL:addConditionProgress("npc_tristan", "bandits")
		DL:gotoNode(10)
		DL:addNode()

		DL:createNPCNode(13, -2, "DL_Tristan_MonsterBanished") -- Great. Go to Ivo to tell him the good news.
		DL:addConditionProgress("npc_tristan", "took_care")
		DL:gotoNode(10)
		DL:addNode()
		
	end
	
end	
	