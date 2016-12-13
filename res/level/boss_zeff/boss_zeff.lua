onWin = function(W)
	W:setMap("res/map/meadows/meadows.tmx", 1060, 1025)
	W:addConditionProgress("boss", "BossZeff")

	if (W:isQuestState("zeffs_curse", "started")) then
		W:changeQuestState("zeffs_curse", "failed")
	end
	if (W:isQuestState("hungry_wolf", "started")) then
		W:changeQuestState("hungry_wolf", "failed")
	end
end
	
onLose = function(W)
	W:setMap("res/map/meadows/meadows.tmx", 1060, 1025)
end