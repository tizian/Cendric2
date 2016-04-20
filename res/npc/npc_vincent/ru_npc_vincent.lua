-- Routine for NPC "Vincent"

velocity = 60

loadRoutine = function(R)

	if (R:hasItem("pe_feudalfire",1) and not R:isConditionFulfilled("npc_vincent","talked")) then 

		R:goToTile(10.2,10)
		R:goToTile(15,10)
		
		R:setTalkingActiveForce(true)
		R:setLooped(false)
		
	elseif (R:isConditionFulfilled("npc_vincent","talked") and (not R:isConditionFulfilled("npc_vincent", "cooperated") or R:isQuestState("spoiled_fire", "completed"))) then
		
		-- vincent is leaving....
		R:setTilePosition(13.5,20)
		R:goToTile(10.2,10)
		R:goToTile(13.5,10)
		R:goToTile(13.5,20)
		R:setDisposed()

	else

		R:wait(5000)	
		R:goToTile(10.2,7.5)
		R:wait(5000)
		R:goToTile(11.2,7.5)
		
		R:setLooped(true)
	end

end	