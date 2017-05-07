-- Routine for NPC "Jonathan" 

velocity = 30

loadRoutine = function(R, W)

    if (W:isQuestComplete("element_master")) then
        R:setDisposed()
        return
    end

	if (W:isQuestState("element_master", "void") and W:isConditionFulfilled("default", "chapter3")) then
		R:setTilePosition(9,7)
		R:setTalkingActive(true)
		R:setReloadEnabled(true)
		return
	end

	if (W:isConditionFulfilled("npc_jonathan","talked")) then
		R:setTalkingActive(false)
		R:setReloadEnabled(false)
		R:setTilePosition(9,7)
		R:setFacingDown()
		R:wait(1000)
		R:goToTile(11,7)
		R:goToTile(11,11)
		R:goToTile(14,11)
		R:goToTile(14,9)
		R:goToTile(13.5,9)
		R:goToTile(13.5,6)
		R:wait(5000)
		R:goToTile(13.5,9)
		R:goToTile(14,9)
		R:goToTile(14,11)
		R:goToTile(11,11)
		R:goToTile(11,6.5)
		R:wait(5000)
		R:goToTile(11,7)
	else
		R:setTalkingActive(false)
		R:setReloadEnabled(true)
		R:setTilePosition(11,12)
		R:setFacingDown()
		R:wait(1000)
		R:goToTile(14,12)
		R:setFacingDown()
		R:wait(1000)
		R:goToTile(11,12)
	end

end	