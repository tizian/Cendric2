-- Routine for NPC "Rhendal"

velocity = 50

loadRoutine = function(R)

	R:setTilePosition(11,8)
	R:setLooped(false)
	
	if (not R:isConditionFulfilled("npc_rhendal", "talked")) then
		R:setTalkingActive(true)
	elseif (R:isQuestState("silkweed_potion", "started") and R:isConditionFulfilled("npc_rhendal", "potion_drunk")) then
		R:setTalkingActive(true)
	end
	
end	