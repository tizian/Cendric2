-- Routine for NPC "Hunter Edmond"

velocity = 50

loadRoutine = function(R)
		if (R:isConditionFulfilled("tutorial_complete")) then 
			R:setTilePosition(65, 17)
			
			R:wait(1000)
			R:goToTile(65,17)
			R:goToTile(65,14)
			R:goToTile(66,14)
			R:goToTile(66,12)
			R:goToTile(60,12)
			R:goToTile(61,13)
			R:goToTile(60,13)
			R:goToTile(60,17)
			
		elseif (R:isConditionFulfilled("talked_to_edmond")) then 
		
			R:setTilePosition(44, 17)
			
			R:wait(1000)
			R:goToTile(46,17)
			R:setLooped(false)
			
		else
		
			R:setTilePosition(44, 17)
			R:wait(1000)
			R:goToTile(40,17)
			R:wait(1000)
			R:goToTile(44,17)
		end
    end