-- Routine for NPC "Zeff"

loadRoutine = function(R)

	R:setTilePosition(22,20)
	R:setLooped(false)
	if (R:isConditionFulfilled("boss","BossZeff")) then
		R:setDisposed()
	end
	
end	