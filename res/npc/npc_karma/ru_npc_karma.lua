-- Routine for NPC "Karma"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("npc_karma", "sex") and not W:isConditionFulfilled("npc_karma", "after_sex")) then
        R:setTalkingActive(true)
    end

	R:setTilePosition(17,5)
    
	R:setFacingDown()
    R:wait(4000)
    R:goToTile(17,6)
    R:goToTile(18,6)
    R:goToTile(18.3,5)
    R:setFacingUp()
    R:wait(4000)
    R:goToTile(18,6)
	R:goToTile(17,6)
    R:goToTile(17,5)
end	