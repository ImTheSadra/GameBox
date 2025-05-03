local texture = 0

function g_init()
    texture = loadTexture("gbox.png")
end

function g_loop()
    fill(1, 0.8, 1)

    useTexture(0)

    beginQuad()
    color(0.6, 0.0, 0.8)
    vertex( 0.5, -0.5, 0.0)
    texCoord(0,0)

    vertex( 0.5,  0.5, 0.0)
    texCoord(1,0)

    color(0.0, 0.0, 1.0)
    vertex(-0.5,  0.5, 0.0)
    texCoord(1,1)
    
    vertex(-0.5, -0.5, 0.0)
    texCoord(0,1)

    endDraw()

    if btn(K_LEFT) then
        rotate(0,1,0,-1)
    end
    if btn(K_RIGHT) then
        rotate(0,1,0, 1)
    end

    if btn(K_DOWN) then
        rotate(1,0,0, 1)
    end
    if btn(K_UP) then
        rotate(1,0,0,-1)
    end
end
