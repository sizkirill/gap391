local green = 0x00ff00ff
local brown = 0x663300ff

local function Process (lineBuilder, variable, numIterations, stepAmount, rngDevice)
	local actualStepAmount = stepAmount / (2 ^ numIterations)
	if string.char(variable) == '0' then
		AddVertex(lineBuilder, green)
		MoveTransform(lineBuilder, actualStepAmount)
		AddVertex(lineBuilder, green)
	elseif string.char(variable) == '1' then
		AddVertex(lineBuilder, brown)
		MoveTransform(lineBuilder, actualStepAmount)
		AddVertex(lineBuilder, brown)
	elseif string.char(variable) == '[' then
		PushTransform(lineBuilder)
		RotateTransform(lineBuilder, -30)
	elseif string.char(variable) == ']' then
		PopTransform(lineBuilder)
		RotateTransform(lineBuilder, 15)
	else
		Log("Reached default")
	end
end

return Process