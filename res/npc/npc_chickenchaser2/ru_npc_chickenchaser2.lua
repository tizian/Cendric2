-- Routine for NPC "Chicken Chaser" when he's dead

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("npc_chickenchaser", "dead")) then
        R:setDisposed()
        return
    end

	R:setTilePosition(15,12)
    R:setTalkingEnabled(false)
    R:setLooped(false)

end	