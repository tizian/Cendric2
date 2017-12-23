-- chest in level ppuzzle03                               

onLoot = function(W) 
	W:setConditionFulfilled("trigger", "tower_3_done")
    W:setQuestConditionFulfilled("tower_3", "tower_climbed")
    W:setMap("res/map/island/island.tmx", 1215, 660)
end

