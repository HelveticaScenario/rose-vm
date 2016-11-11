function _init()
  palset(4, 255,200,200)
    -- stat, err = pcall(poke, -1, 5)
    -- print(stat, err)
end

function _update()

end

function _draw()
  pset(0,0,4)
  pset(319,179,4)
  pset(0,179,4)
  pset(319,0,4)
end
