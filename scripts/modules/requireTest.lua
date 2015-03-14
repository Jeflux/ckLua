Test = {}

function Test:create()
    print("Created test script")
end

function Test:update(dt)
	print("Updated test script")
end

function Test:keyDown(vkcode, x, y, key)
	print("Key down on test script")
end

function Test:keyUp(x, y, key)
	print("Key up on test script")
end

return Test
