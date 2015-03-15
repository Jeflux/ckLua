function create()
	print("---------------------------------------------------------")
	print("This script will help you find out keycodes for key map")
	print("The code is the left hand of the mapping")
	print("For example Q = { 0x10, 0x0E } where Q has the scancode 0x10")
	print("---------------------------------------------------------")
end

function update(dt)
end

function keyDown(vkcode, x, y, key)
	local vkhex = string.format("%02x", vkcode)
	vkhex = string.upper(vkhex)

	print("0x"..vkhex .. "\t" .. key, x .. " : " .. y)
end

function keyUp(x, y, key)
end
