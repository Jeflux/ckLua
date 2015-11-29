Rainbow = {}

function Rainbow:create()
	time = 0;

	self.bgleds = {}
    for x=0,21 do
		self.bgleds[x] = {}
        for y=0,6 do
			local f = (x * math.pi) / 10

			local r = 0
			local g = 0
			local b = 0

		    self.bgleds[x][y] = { r = r, g = g, b = b }
		end
	end

    print("Created rainbow script")
end

function Rainbow:update(dt)
	time = time + dt

	for x=0,21 do
		for y=0,6 do
			local t = time * 2
			local f = (x * math.pi) / 10

			local r = (math.sin(f + t) + 1) * 255 / 2
			local g = (math.sin(f + t + 2) + 1) * 255 / 2
			local b = (math.sin(f + t + 4) + 1) * 255 / 2

	    self.bgleds[x][y].r = r
            self.bgleds[x][y].g = g
            self.bgleds[x][y].b = b
		end
	end


--[[
	for x=0,21 do
		for y=0,6 do
            typeleds[x][y].r = typeleds[x][y].r + 1 * dt
            typeleds[x][y].g = typeleds[x][y].g + 1 * dt
            typeleds[x][y].b = typeleds[x][y].b + 1 * dt

            if typeleds[x][y].r > 1 then typeleds[x][y].r = 1 end
            if typeleds[x][y].g > 1 then typeleds[x][y].g = 1 end
            if typeleds[x][y].b > 1 then typeleds[x][y].b = 1 end
		end
	end

    for x=0,21 do
		for y=0,6 do
            local r = lerp(backgroundleds[x][y].r, 255 - backgroundleds[x][y].r, 1 - typeleds[x][y].r)
            local g = lerp(backgroundleds[x][y].g, 255 - backgroundleds[x][y].r, 1 - typeleds[x][y].g)
            local b = lerp(backgroundleds[x][y].b, 255 - backgroundleds[x][y].r, 1 - typeleds[x][y].b)

			setLed(x, y, r, g, b)
		end
	end
]]--
end

function Rainbow:getbgleds()
	return self.bgleds
end

function Rainbow:keyDown(vkcode, x, y, key)
end

function Rainbow:keyUp(x, y, key)
end

return Rainbow
