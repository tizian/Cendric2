-- Routine for NPC "Lloyd" in the tavern

velocity = 60

loadRoutine = function(R, W)

	R:setTilePosition(18,8)
	
	if (W:isConditionFulfilled("boss", "BossMercenaries") and not W:isConditionFulfilled("npc_lloyd4", "talked")) then
		R:setReloadEnabled(true)
		R:setLooped(false)
		R:setTalkingActive(true)
		R:setFacingUp()
		return
	end
	
	R:setTalkingEnabled(false)
	R:setReloadEnabled(false)
	R:goToTile(18,7)
	R:goToTile(10,7)
	R:goToTile(10,11)
	R:goToTile(9,11)
	R:goToTile(9,15)
	R:setDisposed()

end	