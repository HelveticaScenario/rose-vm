function _onkey(code, pressed, isrepeat) 
  print(code, pressed, isrepeat)
end

function _init()
  screen = {}
  for x=1,320 do
    screen[x] = {}
    for y=1,180 do
      screen[x][y] = 0
    end
  end
end

function drawscreen()
  for x=1,320 do
    for y=1,180 do
      pset(x,y,screen[x][y])
    end
  end
end

x = 0
y = 0
function _update()
  x, y = mouse()
  if btn(10) then
    screen[x][y] = 1
  end
end

function _draw()
  cls()
  drawscreen()
  c = 5
  if key(key.space) then
    c = 9
  end
  pset(x, y, c)
end
