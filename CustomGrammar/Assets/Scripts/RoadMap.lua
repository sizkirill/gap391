local green = 0x00ff00ff
local brown = 0x663300ff
local blue = 0x0000ffff
local white = 0xffffffff

local function Process (lineBuilder, variable, stepAmount, rngDevice)
	if string.char(variable) == 'F' then
		AddVertex(lineBuilder, white)
		MoveTransform(lineBuilder, stepAmount)
		AddVertex(lineBuilder, white)
	elseif string.char(variable) == 'L' then
        rotAngle = -10 + FRand(rngDevice) * 10
        RotateTransform(lineBuilder, -rotAngle)
        PushTransform(lineBuilder)
        RotateTransform(lineBuilder, 2 * rotAngle)
    elseif string.char(variable) == 'R' then
        rotAngle = 10 + FRand(rngDevice) * 10
        RotateTransform(lineBuilder, -rotAngle)
        PushTransform(lineBuilder)
        RotateTransform(lineBuilder, 2 * rotAngle)
    elseif string.char(variable) == 'B' then
        AddRect(lineBuilder, 4,4,green)
    elseif string.char(variable) == 'P' then
        PopTransform(lineBuilder)
    elseif string.char(variable) == 'S' then
        rotAngle = 35 + FRand(rngDevice) * 10
        ResetTransform(lineBuilder)
        RotateTransform(lineBuilder, rotAngle)
        RotateInitialTransform(lineBuilder, rotAngle)
	end
end

return Process