-- Routine for NPC "Zeff"
loadRoutine = function(R, W)

	if (W:isConditionFulfilled("boss","BossZeff") or W:isConditionFulfilled("npc_zeff", "zeff_goes")) then
		R:setDisposed()
	end
	
	R:setTilePosition(22,20)
	R:setLooped(false)
	
	if (W:isConditionFulfilled("npc_zeff", "zeff_goes") and not W:isConditionFulfilled("npc_zeff", "zeff_gone")) then
		W:spawnNPC("npc_zeff2", 1100, 1050, 0, 1000)
		W:addConditionProgress("npc_zeff", "zeff_gone")
	end
end	