-- chest in level ppuzzle02                               

onLoot = function(W) 
	W:setConditionFulfilled("trigger", "tower_2_done")
    W:setQuestConditionFulfilled("tower_2", "tower_climbed")
    W:setMap("res/map/island/island.tmx", 1215, 660)
end

