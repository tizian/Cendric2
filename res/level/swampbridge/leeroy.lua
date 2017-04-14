-- Behavior for LEEEROY JENKINS

leeroy = false

update = function(B, W)
    if (not leeroy) then
        leeroy = true
        B:say("Leeroy", 5)
    end
end

onDeath = function(B, W)
	B:say("Chicken", 4)
end
