-- chest in level ppuzzle01                               

onLoot = function(W) 
	W:setConditionFulfilled("trigger", "tower_1_done")
    W:setQuestConditionFulfilled("tower_1", "tower_climbed")
    W:setMap("res/map/island/island.tmx", 1215, 660)
end

