-- chest in level ppuzzle04                               

onLoot = function(W) 
	W:setConditionFulfilled("trigger", "tower_4_done")
    W:setQuestConditionFulfilled("tower_4", "tower_climbed")
    W:setMap("res/map/island/island.tmx", 1215, 660)
end

