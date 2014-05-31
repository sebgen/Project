-- Maze.lua
-- lever_Shape -> lever5_Shape

-- The list of levers from c++
levers = {}
function SetLever( lever )
	table.insert( levers, lever )
end

-- Pull a lever
function PullLever( name )
	if name == "lever_Shape" then
		Game.OpenMazeDoor()
	end
end

