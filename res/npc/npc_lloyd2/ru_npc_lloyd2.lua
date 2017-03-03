-- Routine for NPC "Lloyd" in Gandria

velocity = 60

loadRoutine = function(R, W)

	R:setTilePosition(69,45.5)
	
	if (W:isConditionFulfilled("boss", "BossElysia") and not W:isConditionFulfilled("npc_lloyd2", "talked")) then
		R:setReloadEnabled(true)
		R:setLooped(false)
		R:setTalkingActive(true)
		R:setFacingUp()
		return
	end
	
	R:setTalkingEnabled(false)
	R:setReloadEnabled(false)
	R:goToTile(69,52.5)
	R:goToTile(21.5,52.5)
	R:goToTile(21.5,42)
	R:goToTile(21.5,42)
	R:goToTile(19,42)
	R:goToTile(19,24.5)
	R:goToTile(13,24.5)
	R:goToTile(13,23.5)
	R:setDisposed()

end	