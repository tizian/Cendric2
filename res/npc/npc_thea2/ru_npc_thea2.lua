-- Routine for NPC "Thea" in Gandria

velocity = 30

loadRoutine = function(R, W)

    if (not W:isQuestState("theas_dream", "completed")) then
		R:setDisposed()
		return
	end

	R:setTilePosition(14,9.5)
    
    R:setFacingUp()
    R:wait(7000)
    R:goToTile(14,12)
    R:goToTile(9,12)
	R:setFacingDown()
    R:wait(7000)
   
    R:goToTile(14,12)
	R:goToTile(14,9.5)
	
end	