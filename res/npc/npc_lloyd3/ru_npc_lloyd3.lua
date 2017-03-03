-- Routine for NPC "Lloyd" in Gandria

velocity = 60

loadRoutine = function(R, W)

	R:setTilePosition(45,6)
	
	if (W:isConditionFulfilled("boss", "BossJanus") and not W:isConditionFulfilled("npc_lloyd3", "talked")) then
		R:setReloadEnabled(true)
		R:setLooped(false)
		R:setTalkingActive(true)
		R:setFacingUp()
		return
	end
	
	R:setTalkingEnabled(false)
	R:setReloadEnabled(false)
	R:goToTile(45,8)
	R:goToTile(36,8)
	R:goToTile(36,21)
	R:goToTile(42,21)
	R:goToTile(42,26)
	R:goToTile(29.5,26)
    R:goToTile(29.5,35)
    R:goToTile(19,35)
    R:goToTile(19,24.5)
	R:goToTile(13,24.5)
	R:goToTile(13,23.5)
	R:setDisposed()

end	