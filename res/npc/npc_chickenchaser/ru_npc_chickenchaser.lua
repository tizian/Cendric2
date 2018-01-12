-- Routine for NPC "Chicken Chaser"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("npc_chickenchaser", "dead")) then
        R:setDisposed()
        return
    end

	R:setTilePosition(15,12)
    R:setLooped(false)
    R:setReloadEnabled(true)
	
	if (W:isConditionFulfilled("npc_chickenchaser", "dies") and not W:isConditionFulfilled("npc_chickenchaser", "dead")) then
        W:addConditionProgress("npc_chickenchaser", "dead")
		W:spawnNPC("npc_chickenchaser2", 15 * 50, 12 * 50)
		R:setDisposed()
	end

end	