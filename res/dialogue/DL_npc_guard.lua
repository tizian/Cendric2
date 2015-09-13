-- Lua tests
print("Hello World\n")
loadDialogue = function(dl)
        if (dl:isNPCNeverTalkedTo("npc_guard")) then
            dl:setNPCTalking()
			dl:setText("DL_Guard_Hello")
			dl:setNextTag(-1)
			dl:addNode(0)
			dl:setRoot(0)
        else
            dl:setCendricTalking()
			dl:setText("DL_Guard_Hello")
			dl:setNextTag(-1)
			dl:addNode(0)
			dl:setRoot(0)
        end
    end