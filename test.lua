function _init()
    -- stat, err = pcall(poke, -1, 5)
    -- print(stat, err)
end

function _update()

end
x = 0
y = 0
function _draw()
  cls()
  -- circfill(100,100,6,6)
  circ(30,30,12,6)
  circ(x,y,12,6)
  x = x + 1
  if x >= 320 then
    x = 0
    y = y + 1
  end
  
  -- circ(math.random(319),math.random(179),math.random(319),math.random(40),math.random(10)+1)
  -- rectfill(0,0,0,0,5)
  -- rect(math.random(319),math.random(179),math.random(319),math.random(179),math.random(15))
  -- rectfill(math.random(319),math.random(179),math.random(319),math.random(179),math.random(15))
end
