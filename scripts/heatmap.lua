function create()
	keyPressed = false

	time = 0;

	leftBound = 1
	rightBound = 12
	topBound = 3
	bottomBound = 5

	excludekeys = {}
	excludekeys["space"] = true

	heatmap = {}    
    for x=0,21 do
		heatmap[x] = {}
        for y=0,6 do
			heatmap[x][y] = 0
			setLed(x, y, 70, 70, 100)
		end
	end

	setLed("stop", 		120, 100, 200)
	setLed("previous", 	100, 100, 200)
	setLed("play", 		120, 100, 200)
	setLed("next", 		120, 100, 200)
	setLed("mute", 		120, 100, 200)

	col1 = { r = 40, g = 0, b = 150 }
	col2 = { r = 200, g = 10, b = 100 }

	rainbow = require("scripts/modules/rainbow")
	rainbow:create();
	rainbowLeds = rainbow:getbgleds()
end

function update(dt)
	time = time + dt

	local max = 1
	local min = 1
	for x=leftBound,rightBound do
        for y=topBound,bottomBound do
			local v = heatmap[x][y]

			if v > max then
				max = v
			end
			if v <= min then
				min = v
			end

		end
	end

	rainbow:update(dt)
	for x=0,14 do
		for y=0,1 do
			led = rainbowLeds[x][y]
			setLed(x, y, led.r * 0.5, led.g * 0.5, led.b * 0.5)
		end
	end

	for x=leftBound,rightBound do
        for y=topBound,bottomBound do
			if x == 12 and y == 5 then

			else
				local f = heatmap[x][y] / max

				local r = lerp(col1.r, col2.r, f)
				local g = lerp(col1.g, col2.g, f)
				local b = lerp(col1.b, col2.b, f)

				setLed(x, y, r, g, b)
			end
		end
	end
end

function keyDown(vkcode, x, y, key)
	if excludekeys[key] == true then return end

	local xx = math.floor(x + 0.5)
	local yy = math.floor(y + 0.5)

	if xx >= leftBound and xx <= rightBound and yy >= topBound and yy <= bottomBound then
		heatmap[xx][yy] = heatmap[xx][yy] + 1
	end

	if key == "f5" then
		loadScript("scripts/heatmap.lua")
	end
end

function keyUp(x, y, key)
end

function lerp(a, b, t)
	return a + t * (b - a)
end
