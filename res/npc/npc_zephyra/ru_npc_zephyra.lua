-- Routine for NPC "Zephyra" 

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_zephyra","talked")) then
		R:setTalkingActive(false)
		R:setReloadEnabled(false)
		R:setTilePosition(36.5,28)
		R:setFacingDown()
		R:wait(10000)
		R:goToTile(37,28)
        R:goToTile(33,29)
        R:goToTile(33,26)
        R:goToTile(32,26)
        R:goToTile(32,24)
        R:goToTile(34,24)
        R:goToTile(34,22)
        R:goToTile(39,22)
        R:goToTile(39,28)

		R:goToTile(36.5,28)
	else
		R:setTalkingActiveForce(true)
		R:setReloadEnabled(true)
		R:setTilePosition(36.5,27.5)
		R:setFacingDown()
        R:setLooped(false)
	end

end	