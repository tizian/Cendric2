-- Behavior for enemy zeff

onDeath = function(B, W)
		B:say("Argh", 4)
		if (W:isQuestState("zeffs_curse", "started")) then
			W:changeQuestState("zeffs_curse", "failed")
		end
		if (W:isQuestState("hungry_wolf", "started")) then
			W:changeQuestState("hungry_wolf", "failed")
		end
    end