-- Routine for NPC "Vincent"

velocity = 60

loadRoutine = function(R)

	if (R:isConditionFulfilled("npc_innkeeper","bought_feudal_fire") and not R:isConditionFulfilled("npc_vincent","talked")) then 

		R:goToTile(10.2,10)
		R:goToTile(15,10)
		
		R:setTalkingActiveForce(true)
		R:setLooped(false)
		
	else

		R:wait(5000)	
		R:goToTile(10.2,8.0)
		R:wait(5000)
		R:goToTile(10.2,8.0)
		
		R:setLooped(true)
	end

end	