-- Routine for NPC "Thea"

velocity = 30

loadRoutine = function(R, W)

    if (W:isQuestState("theas_dream", "completed")) then
		R:setDisposed()
		return
	end

	R:setTilePosition(7,10.3)
	
	R:goToTile(6.5,10.3)
	R:goToTile(6.5,10.2)
	R:wait(10000)
	-- aah my beer is empty!
	R:goToTile(11,11.2)
	R:goToTile(11,11.5)
	R:goToTile(17,11.5)
	R:goToTile(17,11.3)
	R:wait(7000)
	R:goToTile(11,11.5)
	R:goToTile(11,10.2)
	
end	