-- Maze.lua
-- lever_Shape -> lever5_Shape

-- The list of levers from c++
levers = {}
function SetLever( lever )
	table.insert( levers, lever )
end

-- Pull a lever
function PullLever( name )
	if name == "leverShape" then 				--Level 2
		Game.ResetMaze()
		Game.OpenMazeDoor( "gate_5Shape" )
		Game.OpenMazeDoor( "gate_8Shape" )
		Game.OpenMazeDoor( "gate_9Shape" )
		Game.OpenMazeDoor( "gate_10Shape" )
		Game.OpenMazeDoor( "gate_11Shape" )
		Game.OpenMazeDoor( "gate_12Shape" )
		--Game.CloseMazeDoor( "gate_6Shape" )
		--Game.CloseMazeDoor( "gate_7Shape" )
	elseif name == "leverShape1" then			--Level 3
		Game.ResetMaze()
		Game.OpenMazeDoor( "gate_1Shape" )
		Game.OpenMazeDoor( "gate_2Shape" )
		Game.OpenMazeDoor( "gate_3Shape" )
		Game.OpenMazeDoor( "gate_5Shape" )
		Game.OpenMazeDoor( "gate_6Shape" )
		Game.OpenMazeDoor( "gate_11Shape" )
		Game.OpenMazeDoor( "gate_12Shape" )
		--Game.CloseMazeDoor( "gate_8Shape" )
		--Game.CloseMazeDoor( "gate_9Shape" )
		--Game.CloseMazeDoor( "gate_10Shape" )
	elseif name == "leverShape2" then			--Level 4
		Game.ResetMaze()
		Game.OpenMazeDoor( "gate_1Shape" )
		Game.OpenMazeDoor( "gate_3Shape" )
		Game.OpenMazeDoor( "gate_5Shape" )
		Game.OpenMazeDoor( "gate_6Shape" )
		Game.OpenMazeDoor( "gate_7Shape" )
		Game.OpenMazeDoor( "gate_8Shape" )
		Game.OpenMazeDoor( "gate_9Shape" )
		Game.OpenMazeDoor( "gate_10Shape" )
		Game.OpenMazeDoor( "gate_14Shape" )
		--Game.CloseMazeDoor( "gate_2Shape" )
		--Game.CloseMazeDoor( "gate_11Shape" )
		--Game.CloseMazeDoor( "gate_12Shape" )
	elseif name == "leverShape3" then			--Level 5
		Game.ResetMaze()
		Game.OpenMazeDoor( "gate_0Shape" )
		Game.OpenMazeDoor( "gate_1Shape" )
		Game.OpenMazeDoor( "gate_2Shape" )
		Game.OpenMazeDoor( "gate_3Shape" )
		Game.OpenMazeDoor( "gate_5Shape" )
		Game.OpenMazeDoor( "gate_6Shape" )
		Game.OpenMazeDoor( "gate_7Shape" )
		Game.OpenMazeDoor( "gate_8Shape" )
		Game.OpenMazeDoor( "gate_9Shape" )
		Game.OpenMazeDoor( "gate_10Shape" )
		Game.OpenMazeDoor( "gate_11Shape" )
		Game.OpenMazeDoor( "gate_12Shape" )
		Game.OpenMazeDoor( "gate_14Shape" )
	elseif name == "leverShape4" then      		--Level 6
		Game.ResetMaze()
		Game.OpenMazeDoor( "gate_0Shape" )
		Game.OpenMazeDoor( "gate_1Shape" )
		Game.OpenMazeDoor( "gate_2Shape" )
		Game.OpenMazeDoor( "gate_3Shape" )
		Game.OpenMazeDoor( "gate_5Shape" )
		Game.OpenMazeDoor( "gate_6Shape" )
		Game.OpenMazeDoor( "gate_7Shape" )
		Game.OpenMazeDoor( "gate_8Shape" )
		Game.OpenMazeDoor( "gate_9Shape" )
		Game.OpenMazeDoor( "gate_10Shape" )
		Game.OpenMazeDoor( "gate_11Shape" )
		Game.OpenMazeDoor( "gate_12Shape" )
		Game.OpenMazeDoor( "gate_13Shape" )
		Game.OpenMazeDoor( "gate_14Shape" )
	end
end

Maze = {}