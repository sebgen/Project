-- Dungeon.lua

-- Solution to lever puzzle, right sequence: 2, 1, 3, 4
solution = { "leverShape2", "leverShape7", "leverShape6", "leverShape4" }
count = 0

-- The list of levers from c++
levers = {}
function SetLever( lever )
	table.insert( levers, lever )
end

-- Set all levers in levers to false
function ResetLevers()
	for _, lever in pairs( levers ) do
		lever:Reset()
	end
end

-- Pull a lever
function PullLever( name )

	if solution[count + 1] == name then
		count = count + 1
	else
		count = 0
		ResetLevers()
	end
	
	if count == #solution then
		Game.OpenDoor()
		Game.ChangeNavMesh(2)
	end
end

Dungeon = {}