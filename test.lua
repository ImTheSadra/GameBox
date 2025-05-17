local texture = 0
local angle = 0
local lastJoy
local am = 0.0
local amY = 0.0
local angleY = 0

useShader = false

function g_init()
    texture = loadTexture("gbox.png")
    title("GBox Demo")
    camera(
        math.sin(angle)*4,0,math.cos(angle)*4,
        0,0,0
    )

    print(K_RIGHT, "K_RIGHT")
end

function g_loop(time)
    fill(0.3, 0.3, 0.3)

    useTexture(texture)
    
    beginDraw(DRAW_QUADS)
        color(0.0, 0.0, 0.8)
        vertex(0.5, -0.5, 0.0)
        texCoord(0, 0)

        color(0.7, 0.0, 0.8)
        vertex(0.5, 0.5, 0.0)
        texCoord(1, 0)

        color(0.0, 0.0, 0.8)
        vertex(-0.5, 0.5, 0.0)
        texCoord(1, 1)

        color(0.7, 0.0, 0.8)
        vertex(-0.5, -0.5, 0.0)
        texCoord(0, 1)
    endDraw()

    if  am ~= 0.0  then
        angle = angle+(am/100*time)
        camera(
            math.sin(angle)*4, math.sin(angleY), math.cos(angle)*4,
            0,0,0
        )
    end
end

function g_wResize(w,h)
    print("resized to -> ",w,h)
end

function g_joystickAdded(id)
    lastJoy = joyOpen(id)
    print(id, " - ", joyName(id), " - connected")
end

function g_joykeydown(device, btn)
    joyRumble(device, 1, 1, 1000)
    print(device, "-> pressed ", btn)
end

function g_joyAxisMove(device, num, a)
    am = -num;
end

function g_keydown(key)
    print(key, "pressed")
    if k == K_RIGHT then
        am = 1
    end
    if k == K_LEFT then
        am = -1
    end
end

function g_keyup(key)
    am = 0
end