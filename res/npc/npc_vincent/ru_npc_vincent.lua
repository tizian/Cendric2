-- Routine for NPC "Vincent"

velocity = 60

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("default", "gandria_open") or W:isConditionFulfilled("npc_vincent","vincent_gone")) then
		R:setDisposed()
		return
	end
	
	if (W:hasItem("pe_feudalfire",1) and not W:isConditionFulfilled("npc_vincent","talked")) then 

		R:setReloadEnabled(true)
		R:setTalkingActiveForce(true)
		R:setLooped(false)
		
		R:goToTile(10.2,10)
		R:goToTile(15,10)

	elseif (W:isConditionFulfilled("npc_vincent","talked") and (not W:isConditionFulfilled("npc_vincent", "cooperated") or W:isQuestState("spoiled_fire", "completed"))) then
		
		-- vincent is leaving....
		R:setReloadEnabled(false)
		R:setTilePosition(13.5,20)
		R:goToTile(10.2,10)
		R:goToTile(13.5,10)
		R:goToTile(13.5,20)
		R:setDisposedStep()
		W:addConditionProgress("npc_vincent", "vincent_gone")

	else
		
		R:setLooped(true)
		R:setReloadEnabled(true)

		R:wait(5000)	
		R:goToTile(10.2,7.5)
		R:wait(5000)
		R:goToTile(11.2,7.5)

	end
end	