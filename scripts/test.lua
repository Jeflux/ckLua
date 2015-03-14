function create()
	keyPressed = false

	time = 0;

	for x=0,22 do
		for y=0,7 do
			setLed(x, y, (y/7) * 255, 0, 0)
		end
	end

	backgroundleds = {}
    typeleds = {}
    for x=0,21 do
		backgroundleds[x] = {}
        typeleds[x] = {}
        for y=0,6 do
			local f = (x * math.pi) / 10

			local r = 0
			local g = 0
			local b = 0

		    backgroundleds[x][y] = { r = r, g = g, b = b }
            typeleds[x][y] = { r = r, g = g, b = b }
		end
	end

    balls = {}

    rainbow = require("scripts/modules/rainbow"):new()
    rainbow:create()
end

function update(dt)
	time = time + dt

	rainbow:update(dt)

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
			rbgleds = rainbow:getbgleds()

            local r = lerp(rbgleds[x][y].r / 4, 255, 1 - typeleds[x][y].r)
            local g = lerp(rbgleds[x][y].g / 4, 255, 1 - typeleds[x][y].g)
            local b = lerp(rbgleds[x][y].b / 4, 255, 1 - typeleds[x][y].b)

			setLed(x, y, r, g, b)
		end
	end

    if #balls < 2 then
        local vx = math.random(-5, 5)
        while vx >= -0.5 and vx <= 0.5 do vx = math.random(-5, 5) end

		if math.ceil(math.random(0, 10)) == 10 then
			table.insert(balls, { x = math.random(0, 13.5), y = 0,
								  vx = math.random(-5, 5), vy = 0,
								  radius = math.random(0.5, 1),
								  bounces = 0,
								  r = math.random(200, 255), g = math.random(200, 255), b = math.random(200, 255) })
		end
    end

    for k,v in pairs(balls) do
        v.x = v.x + v.vx * dt
        v.y = v.y + v.vy * dt
        v.vy = v.vy + 9.82 * dt

        if v.vy > 100 then
            v.vy = 100
        end

        if v.x <= 1 + v.radius / 2 or v.x >= 13.5 - v.radius / 2 then
             v.vx = -v.vx
        end

        if v.y >= 7 then
			v.v = 6
            v.vy = -(v.vy * 0.75)
			v.bounces = v.bounces + 1
        end

		if v.bounces >= 5 then
			table.remove(balls, k)
		end

        drawCircle(v.x, v.y, v.radius, v.r, v.g, v.b)
    end
end

function drawCircle(x, y, radius, r, g, b)
    for i = 1, 360 do
        local angle = i * math.pi / 180
        local ptx, pty = x + radius * math.cos( angle ), y + radius * math.sin( angle )
        ptx = math.floor(ptx + 0.5)
        pty = math.floor(pty + 0.5)

        if ptx >= 0 and pty >= 0 and ptx <= 21 and pty <= 6 then
            setLed(ptx, pty, r, g, b)
        end
    end
end


function keyDown(vkcode, x, y, key)
	local vkhex = string.format("%02x", vkcode)
	vkhex = string.upper(vkhex)

	--print("0x"..vkhex .. " " .. key .. " was pressed. Corresponds to " .. x .. ":" .. y)

    --print(x, y)

	--setLed(x, y, 255, 255, 255)

	--[[if key == "f4" or key == "f5" then
		print("Reloading script and keymap")
		loadKeymap("keymaps/sv_keymap.keyconf")
		loadScript("scripts/test.lua")
	end]]--

    if x >= 0 and x <= 21 and y >= 0 and y <= 6 then
        local r = 1.25
        for i = 1, 360 do
            local angle = i * math.pi / 180
            local ptx, pty = x + r * math.cos( angle ), y + r * math.sin( angle )
            ptx = math.floor(ptx + 0.5)
            pty = math.floor(pty + 0.5)

            if ptx >= 0 and pty >= 0 and ptx <= 21 and pty <= 6 then
                --typeleds[ptx][pty].r = 0
                --typeleds[ptx][pty].g = 0
                --typeleds[ptx][pty].b = 0
            end
        end
    end

    x = math.floor(x + 0.5)
    y = math.floor(y + 0.5)
    if x >= 0 and x <= 22 and y >= 0 and y <= 6 then
        typeleds[x][y].r = 0
        typeleds[x][y].g = 0
        typeleds[x][y].b = 0
    end



end

function keyUp(x, y, key)
end

function lerp(a, b, t)
    return a + t*(b - a)
end
