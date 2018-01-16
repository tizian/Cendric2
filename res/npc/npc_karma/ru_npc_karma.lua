-- Routine for NPC "Karma"

loadRoutine = function(R, W)

	R:setTilePosition(17,5)
    
	R:setFacingDown()
    R:wait(4000)
    R:gotoTile(17,6)
    R:gotoTile(18,6)
    R:gotoTile(18.5,5)
    R:setFacingUp()
    R:wait(4000)
    R:gotoTile(18,6)
	R:gotoTile(17,6)
    R:gotoTile(17,5)
end	