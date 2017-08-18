-- urn (1) in level yashatemple                                        

onLoot = function(W) 
	local enemyData = {
		id = 3,
		loot = {
				{id="gold", amount=2},
			},
		position = {x=1750, y=1210}
	}
	W:spawnEnemy(enemyData)
    
    if (W:isQuestState("yasha_sanctuary", "started")) then
        W:changeQuestState("yasha_sanctuary", "failed")
    end
    
    W:addConditionProgress("npc_yasha", "urn_open")
end

