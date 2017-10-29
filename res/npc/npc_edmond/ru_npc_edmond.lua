-- Routine for NPC "Hunter Edmond"

velocity = 50

loadRoutine = function(R, W)
		if (W:isConditionFulfilled("default","tutorial_complete")) then 
		
			if (W:isConditionFulfilled("npc_edmond", "talked_after_tutorial")) then
				R:setTilePosition(57, 17)

				R:goToTile(57,17)
				R:goToTile(65,17)
				R:goToTile(65,14)
				R:goToTile(66,14)
				R:goToTile(66,12)
				R:goToTile(61,12)
				R:goToTile(61,13)
				R:goToTile(60,13)
				R:goToTile(60,17)
			else 
			
				R:setTilePosition(57, 17)
				R:setLooped(false)
				R:setTalkingActive(true)
				
			end
			
		elseif (W:isConditionFulfilled("trigger","tutorial_started")) then 
		
			R:setTilePosition(44, 17)
			R:setTalkingEnabled(false)
			
			R:wait(1000)
			R:goToTile(46,18)
			R:setLooped(false)
            
        elseif (W:isItemEquipped("we_walkingpole") and not W:isConditionFulfilled("trigger","tutorial_started")) then
            R:setTalkingActiveForce(true)
			R:setReloadEnabled(true)
			R:setTilePosition(44, 17)
			
			R:wait(1000)
			R:goToTile(40,17)
			R:wait(1000)
			R:goToTile(44,17)
			
		else
			R:setTalkingActive(true)
			R:setReloadEnabled(true)
			R:setTilePosition(44, 17)
			
			R:wait(1000)
			R:goToTile(40,17)
			R:wait(1000)
			R:goToTile(44,17)
		end
    end