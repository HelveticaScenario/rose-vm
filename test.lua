function _init()
    -- stat, err = pcall(poke, -1, 5)
    -- print(stat, err)
end

x = 0
y = 0
wheelX = 30
wheelY = 30
function _update()
  x, y = mouse()
  newWheelX, newWheelY = wheel()
  wheelX = wheelX + newWheelX
  wheelY = wheelY + (newWheelY * -1)
end

function _draw()
  cls()
  rectfill(0,0,319,179,5)
  -- circfill(100,100,6,6)

  circ(wheelX,wheelY,12,6)
  left, right = btn(10, 11)
  c = 9
  if left then
    c = 7
  elseif right then
    c = 8
  end
  circfill(x,y,12,c)
  
  -- circ(math.random(319),math.random(179),math.random(319),math.random(40),math.random(10)+1)
  -- rectfill(0,0,0,0,5)
  -- rect(math.random(319),math.random(179),math.random(319),math.random(179),math.random(15))
  -- rectfill(math.random(319),math.random(179),math.random(319),math.random(179),math.random(15))
end
