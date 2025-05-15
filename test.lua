local texture = 0
local angle = 0

useShader = false

function g_init()
    texture = loadTexture("gbox.png")
    title("GBox Demo")
    camera(
        math.sin(angle)*4,0,math.cos(angle)*4,
        0,0,0
    )
end

function g_loop(time)
    fill(0.3, 0.3, 0.3)

    useTexture(texture)
    
    beginDraw(DRAW_QUADS)
        color(0.7, 0.0, 0.8)
        vertex(0.5, -0.5, 0.0)
        texCoord(0, 0)

        vertex(0.5, 0.5, 0.0)
        texCoord(1, 0)

        color(0.0, 0.0, 0.8)
        vertex(-0.5, 0.5, 0.0)
        texCoord(1, 1)

        vertex(-0.5, -0.5, 0.0)
        texCoord(0, 1)
    endDraw()

    if btn(K_SPACE) then
        angle = angle+0.01*time
        camera(
            math.sin(angle)*4, 0, math.cos(angle)*4,
            0,0,0
        )
    end
end

function g_wResize(w,h)
    print("resized to -> ",w,h)
end

function g_keydown(key)

end

function g_keyup(key)
    
end