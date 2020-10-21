local green = 0x00ff00ff
local brown = 0x663300ff
local blue = 0x0000ffff
local white = 0xffffffff

local function Process (lineBuilder, variable, numIterations, stepAmount, rngDevice)
	if string.char(variable) == 'F' then
		AddVertex(lineBuilder, white)
		MoveTransform(lineBuilder, stepAmount * (FRand(rngDevice) + 0.5))
		AddVertex(lineBuilder, white)
	elseif string.char(variable) == '[' then
		PushTransform(lineBuilder)
	elseif string.char(variable) == ']' then
		PopTransform(lineBuilder)
	elseif string.char(variable) == '-' then
		RotateTransform(lineBuilder, -30 + FRand(rngDevice) * 10)
	elseif string.char(variable) == '+' then
		RotateTransform(lineBuilder, 20 + FRand(rngDevice) * 10)
	end
end

return Process