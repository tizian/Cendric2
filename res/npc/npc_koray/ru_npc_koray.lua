-- Routine for NPC "Koray"

loadRoutine = function(R, W)

    if (W:isQuestState("help_koray","completed")) then
		R:setDisposed()
        return
	end
    
    if (W:isQuestState("help_koray","void")) then
        R:setTilePosition(23,18)
        
        R:setReloadEnabled(true)
        R:setLooped(false)
        R:setFacingRight()
        return
    end
    
    if (W:isQuestState("help_koray","started")) then
        R:setTilePosition(23,18)

        R:setLooped(false)
        R:goToTile(23,21)
        R:goToTile(27,21)
        R:goToTile(27,23)
        R:goToTile(30,23)
        R:goToTile(30,24)
        R:goToTile(35,24)
        R:goToTile(35,27)
        R:goToTile(38,27)
        R:setFacingDown()
        
    end

end	