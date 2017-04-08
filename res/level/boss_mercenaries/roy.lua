-- Behavior for enemy roy
started = false

update = function(B, W)
		if (not started) then
			started = true
			B:say("RoyStart", 4)
		end
    end

