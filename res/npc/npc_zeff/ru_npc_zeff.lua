-- Routine for NPC "Zeff"
loadRoutine = function(R, W)

	if (W:isConditionFulfilled("boss","BossZeff") or W:isConditionFulfilled("npc_zeff", "zeff_gone")) then
		R:setDisposed()
		return
	end
	
	R:setTilePosition(22,20)
	R:setReloadEnabled(true)
	R:setLooped(false)
	
	if (W:isConditionFulfilled("npc_zeff", "zeff_goes") and not W:isConditionFulfilled("npc_zeff", "zeff_gone")) then
		W:spawnNPC("npc_zeff2", 1100, 1050)
		W:addConditionProgress("npc_zeff", "zeff_gone")
		R:setDisposed()
	end
end	