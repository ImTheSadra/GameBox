local x = 0;
local y = 0;
local angle = 0;

function g_loop()
    color(140, 50, 200)
    fill(40, 150, 30)
    color(255, 255, 255)
    line(400, 300, x, y)
    x = 400+math.sin(angle)*200
    y = 300+math.cos(angle)*200

    rect(x,y,x+10, y+10)
    
    angle = angle+0.001
end