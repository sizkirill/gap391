local green = 0x00ff00ff
local brown = 0x663300ff
local blue = 0x0000ffff

local function Process (lineBuilder, variable, numIterations, stepAmount, rngDevice)
	if string.char(variable) == 'F' then
		AddVertex(lineBuilder, blue)
		MoveTransform(lineBuilder, stepAmount)
		AddVertex(lineBuilder, blue)
	elseif string.char(variable) == '-' then
		RotateTransform(lineBuilder, -90)
	elseif string.char(variable) == '+' then
		RotateTransform(lineBuilder, 90)
	end
end

return Process