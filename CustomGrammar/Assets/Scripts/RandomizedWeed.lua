local green = 0x00ff00ff
local brown = 0x663300ff
local blue = 0x0000ffff
local white = 0xffffffff

local function Process (lineBuilder, variable, numIterations, stepAmount, rngDevice)
	if string.char(variable) == 'F' then
		AddVertex(lineBuilder, green)
		MoveTransform(lineBuilder, stepAmount)
		AddVertex(lineBuilder, wgreen)
	elseif string.char(variable) == '[' then
		PushTransform(lineBuilder)
	elseif string.char(variable) == ']' then
		PopTransform(lineBuilder)
	elseif string.char(variable) == '+' then
		RotateTransform(lineBuilder, 15 + FRand(rngDevice) * 7)
	elseif string.char(variable) == '-' then
		RotateTransform(lineBuilder, -15 - FRand(rngDevice) * 7)
	end
end

return Process