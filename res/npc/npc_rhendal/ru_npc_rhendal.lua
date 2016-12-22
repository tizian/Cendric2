-- Routine for NPC "Rhendal"

velocity = 50

loadRoutine = function(R, W)

	R:setTilePosition(11,8)
	R:setReloadEnabled(true)
	R:setLooped(false)
	
	if (not W:isConditionFulfilled("npc_rhendal", "talked")) then
		R:setTalkingActive(true)
	elseif (W:isQuestState("silkweed_potion", "started") and W:isConditionFulfilled("npc_rhendal", "potion_drunk")) then
		R:setTalkingActive(true)
	end
	
end	