-- Behavior for prisoner (1) in the level gandriamines

routine = function(O) 
	O:gotoTile(96, 42)
	O:executeFightAnimation()
	O:wait(3)
	O:executeFightAnimation()
	O:wait(2)
	O:say("SoExhausted", 2)
	O:wait(2)
end
