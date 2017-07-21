-- Routine for NPC "Zephyra" 

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_zephyra","talked")) then
		R:setTalkingActive(false)
		R:setReloadEnabled(false)
		R:setTilePosition(36.5,28)
		R:setFacingDown()
		R:wait(1000)
		R:wait(5000)
		R:goToTile(36.5,28)
	else
		R:setTalkingActive(true)
		R:setReloadEnabled(true)
		R:setTilePosition(36.5,28)
		R:setFacingDown()
        R:setLooped(false)
	end

end	