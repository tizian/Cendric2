-- Behavior for prisoner (2) in the level gandriamines

routine = function(O) 
	O:gotoTile(89.7, 17.1)
	O:setFacingRight()
	
	for i = 1, 3 do 
		O:executeFightAnimation()
		O:wait(1)
		O:executeFightAnimation()
		O:wait(1)
		O:executeFightAnimation()
		O:wait(5)
	end

	O:say("WorkWork", 2)
	O:wait(2)
end
