-- Maze.lua
-- lever_Shape -> lever5_Shape

-- The list of levers from c++
levers = {}
function SetLever( lever )
	table.insert( levers, lever )
end

-- Pull a lever
function PullLever( name )
	if name == "lever_Shape" then 				--Level 2
		Game.OpenMazeDoor( "gate_5Shape" )
		Game.CloseMazeDoor( "gate_6Shape" )
		Game.CloseMazeDoor( "gate_7Shape" )
	else if name == "lever_1Shape" then			--Level 3
		Game.OpenMazeDoor( "gate_1Shape" )
		Game.OpenMazeDoor( "gate_2Shape" )
		Game.OpenMazeDoor( "gate_3Shape" )
		Game.OpenMazeDoor( "gate_6Shape" )
		Game.CloseMazeDoor( "gate_8Shape" )
		Game.CloseMazeDoor( "gate_9Shape" )
		Game.CloseMazeDoor( "gate_10Shape" )
	else if name == "lever_2Shape" then			--Level 4
		Game.OpenMazeDoor( "gate_7Shape" )
		Game.OpenMazeDoor( "gate_8Shape" )
		Game.OpenMazeDoor( "gate_9Shape" )
		Game.OpenMazeDoor( "gate_10Shape" )
		Game.OpenMazeDoor( "gate_14Shape" )
		Game.CloseMazeDoor( "gate_2Shape" )
		Game.CloseMazeDoor( "gate_11Shape" )
		Game.CloseMazeDoor( "gate_12Shape" )
	else if name == "lever_3Shape" then			--Level 5
		Game.OpenMazeDoor( "gate_0Shape" )
		Game.OpenMazeDoor( "gate_2Shape" )
		Game.OpenMazeDoor( "gate_11Shape" )
		Game.OpenMazeDoor( "gate_12Shape" )
end

