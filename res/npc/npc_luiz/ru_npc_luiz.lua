-- Routine for NPC "Luiz"

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_luiz", "talked")) then 
	
		R:setTalkingActive(true) 
		R:setReloadEnabled(true)
		R:setLooped(false)
		R:setTilePosition(2,14)
		R:goToTile(2,16)
		
		R:goToTile(2,11.5)
		R:goToTile(12,11.5)
		R:goToTile(12,7)
		R:goToTile(2,7)
		R:goToTile(2, 2.5)
		R:goToTile(4.3, 3.1)
		R:setFacingDown()
		return

	end

	R:setTilePosition(4.3, 3.1)
	R:setReloadEnabled(true)
	
	R:setFacingDown()
	R:wait(15000)

	R:goToTile(2, 2.5)
	R:setFacingUp()
	R:wait(6000)
	
	R:goToTile(2,7)
	R:goToTile(8,7)
	R:goToTile(8,6)
	R:goToTile(7.2,6)
	R:setFacingLeft()
	R:wait(6000)
	
	R:goToTile(12,6)
	R:goToTile(12,11.5)
	R:goToTile(2,11.5)
	R:goToTile(2,16)
	R:goToTile(5,16)
	R:setFacingUp()
	R:wait(7000)
	
	R:goToTile(2,16)
	R:goToTile(2,11.5)
	R:goToTile(12,11.5)
	R:goToTile(12,7)
	R:goToTile(2,7)
	R:goToTile(2, 2.5)
	R:goToTile(4.3, 3.1)
	
end	