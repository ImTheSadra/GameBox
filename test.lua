local texture = 0
local angle = 0

function g_init()
    texture = loadTexture("gbox.png")
    title("GBox Demo")
end

function g_loop()
    fill(1, 0.8, 1)

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
        angle = angle+0.01
        camera(
            math.sin(angle)*4, 0, math.cos(angle)*4,
            0,0,0
        )
    end
end