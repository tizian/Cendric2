-- Behavior for prisoner (5) in the level gandriamines

routine = function(O) 
	O:gotoTile(97, 4.1)
	O:setFacingRight()
	
	for i = 1, 3 do 
		O:executeFightAnimation()
		O:wait(1)
		O:executeFightAnimation()
		O:wait(1)
		O:executeFightAnimation()
		O:wait(5)
	end

	O:say("NeedABreak", 2)
	O:wait(2)
end

