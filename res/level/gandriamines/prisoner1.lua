-- Behavior for prisoner (1) in the level gandriamines

routine = function(O) 
	O:gotoTile(91.7, 43.1)
	O:setFacingLeft()
	
	for i = 1, 3 do 
		O:executeFightAnimation()
		O:wait(1)
		O:executeFightAnimation()
		O:wait(1)
		O:executeFightAnimation()
		O:wait(5)
	end

	O:say("SoExhausted", 2)
	O:wait(2)
end
