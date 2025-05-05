local texture = 0

function g_init()
    texture = loadTexture("gbox.png")
    title("GBox Demo")
    print("Welcome to GBox Game Engine :)")
end

function g_loop()
    fill(1, 0.8, 1)

    useTexture(texture)
    
    beginQuad()
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

    rotate(0,1,0, 0.5)
end