-- Routine for NPC "Rhendal"

velocity = 50

loadRoutine = function(R, W)

	R:setTilePosition(11,8)
	R:setReloadEnabled(true)
	R:setLooped(false)
	
	if (not W:isConditionFulfilled("npc_rhendal", "talked")) then
		R:setTalkingActive(true)
	elseif (W:isQuestState("silkweed_potion", "completed") and not W:isConditionFulfilled("npc_rhendal", "vision")) then
		R:setTalkingActive(true)
	end
	
end	