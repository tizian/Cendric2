-- Routine for NPC "Bjarne"

velocity = 50

loadRoutine = function(R, W)

    if (W:isQuestComplete("element_master")) then
        R:setDisposed()
        return
    end

    R:setTilePosition(23.5,59.5)
    
    R:goToTile(3,59.5)
    R:goToTile(3,52.5)
    R:goToTile(23.5,52.5)
    R:goToTile(23.5,59.5)
    
    R:goToTile(34,59.5)
    R:setFacingUp()
    R:wait(5000)

    R:goToTile(23.5,59.5)

end	