-- Cave.lua 

-- CheckMusickWheels
-- CheckBoilerWheels
-- CheckButtonWheels

-- Solution to Music Wheel puzzle
isMusicPanelOn = false
musicSolution = {}
musicSolution[ "wheel1Shape" ] = 2
musicSolution[ "wheel2Shape" ] = 4
musicSolution[ "wheel3Shape" ] = 2
musicSolution[ "wheel4Shape" ] = 1

-- The list of music wheels from c++
musicWheels = {}
function SetMusicWheel( wheel )
	table.insert( musicWheels, wheel )
end

-- The list of boiler wheels from c++
boilerWheels = {}
function SetBoilerWheel( wheel )
	table.insert( boilerWheels, wheel )
end

-- The list of button wheels from c++
buttonWheels = {}
function SetButtonWheel( wheel )
	table.insert( buttonWheels, wheel )
end

-- Check if all Boiler wheels are active to start the music panel
function CheckBoilerWheels()
	local solved = true
	for _, wheel in pairs( boilerWheels ) do
		if wheel:IsOn() ~= true then
			solved = false
		end
	end
	
	if solved then
		Game.StartMusicPanel()
	end
end

-- Check Puzzle
function CheckMusickWheels( )
	local solved = true
	for _, wheel in pairs( musicWheels ) do
		if musicSolution[ wheel:WheelGetName() ] ~= wheel:WheelGetValue() then
			solved = false
		end
	end
	
	if solved then
		Game.OpenDoor()
		Game.ChangeNavMesh(2)
	end
end