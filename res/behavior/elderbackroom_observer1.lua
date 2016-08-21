-- Behavior for observer (1) in the level elderbackroom

routine = function(O) 
	O:gotoTile(15, 20)
	O:gotoTile(25, 20)
	O:wait(1)
	O:say("Test", 3)
end
