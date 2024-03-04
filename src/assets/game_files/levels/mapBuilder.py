TILESW = 25
TILESH = 14

class Tile:
  def __init__(self, animName, rx, ry, x, y, vb=0, mb=0) -> None:
    self.animName = animName
    self.rx = rx
    self.ry = ry
    self.x = x
    self.y = y
    self.vb = vb
    self.mb = mb


class Enemy:
  def __init__(self, animName, rx, ry, x, y, hp, dmg, aibhvr, spd, *args) -> None:
    self.animName = animName
    self.rx = rx
    self.ry = ry
    self.x = x
    self.y = y
    self.hp = hp
    self.dmg = dmg
    self.aibhvr = aibhvr
    self.spd = spd
    if aibhvr == 1:
      self.posnum = args[0]
      self.poslist = args[1]


class Player:
  def __init__(self, rx, ry, x, y, bw, bh, spd, hp) -> None:
    self.rx = rx
    self.ry = ry
    self.x = x
    self.y = y
    self.bw = bw
    self.bh = bh
    self.spd = spd
    self.hp = hp


def main():
  rooms = []
  name = input('Name: ')
  inp = ''
  while inp != 'exit':
    inp = input('[0] Create room\n[exit] exit\n>>>')
    if inp == '0':
      rx = int(input('RX: '))
      ry = int(input('RY: '))
      defTile = input('Default tile (empty:3): ')
      if defTile == '':
        defTile = '3'
      defTile = int(defTile)
      room = []
      isThereRoomAbove = False
      isThereRoomBelow = False
      isThereRoomRight = False
      isThereRoomLeft = False
      for rm in rooms:
        #Room above us
        if rm[0].rx == rx-1 and rm[0].ry == ry:
          isThereRoomAbove = True
        elif rm[0].rx == rx+1 and rm[0].ry == ry:
          isThereRoomBelow = True
        elif rm[0].rx == rx and rm[0].ry == ry-1:
          isThereRoomLeft = True
        elif rm[0].rx == rx and rm[0].ry == ry+1:
          isThereRoomRight = True
      if not isThereRoomAbove:
        for i in range(TILESW):
          room.append(Tile(13, rx, ry, i, -1))
      if not isThereRoomBelow:
        for i in range(TILESW):
          room.append(Tile(13, rx, ry, i, TILESH))
      if not isThereRoomLeft:
        for i in range(TILESH):
          room.append(Tile(13, rx, ry, -1, i))
      if not isThereRoomRight:
        for i in range(TILESH):
          room.append(Tile(13, rx, ry, TILESW, i))
      for x in range(TILESW):
        for y in range(TILESH):
          room.append(Tile(defTile, rx, ry, x, y))
      wantToMod = ''
      while wantToMod != 'n':
        wantToMod = input('[0] Add Tile\n[1] Add enemy\n[2] Add player\n[n] exit>>>')
        if wantToMod == '0':
          tx = int(input('Tile x: '))
          ty = int(input('Tile y: '))
          replIx = -1
          for i in range(len(room)):
            if room[i].x == tx and room[i].y == ty and room[i].__class__.__name__ == 'Tile':
              replIx = i
          tileAnim = int(input('Animation: '))
          vb = int(input('Will it block vision 0/1: '))
          mb = int(input('Will it block movement 0/1: '))
          room[replIx] = Tile(tileAnim, rx, ry, tx, ty, vb, mb)
        elif wantToMod == '1':
          ex = int(input('Enemy x: '))
          ey = int(input('Enemy y: '))
          eAnim = int(input('Animation: '))
          hp = int(input('HP: '))
          dmg = int(input('DMG: '))
          aibhvr = int(input('AI Follow-0 Patrol-1: '))
          spd = float(input('Speed (float): '))
          if aibhvr == 1:
            pnum = int(input('Number of pat positions: '))
            ppos = []
            for i in range(pnum):
              ppos.append(int(input(f'Patrol pos {i+1} x: ')))
              ppos.append(int(input(f'Patrol pos {i+1} y: ')))
            room.append(Enemy(eAnim, rx, ry, ex, ey, hp, dmg, aibhvr, spd, pnum, ppos))
          else:
            room.append(Enemy(eAnim, rx, ry, ex, ey, hp, dmg, aibhvr, spd))
        elif wantToMod == '2':
          px = int(input('Player x: '))
          py = int(input('Player y: '))
          bw = int(input('BBWidth (rec:32): '))
          bh = int(input('BBHeight (rec:62): '))
          spd = float(input('Speed (float): '))
          hp = int(input('HP: '))
          room.append(Player(rx, ry, px, py, bw, bh, spd, hp))
      rooms.append(room)
  
  with open(f'{name}.txt', 'w') as f:
    f.write('Menu assets/game_files/levels/menu.txt M\n')
    for r in rooms:
      for e in r:
        if e.__class__.__name__ == 'Tile':
          f.write(f'T {e.animName} {e.rx} {e.ry} {e.x} {e.y} {e.mb} {e.vb}{chr(10)}')
        elif e.__class__.__name__ == 'Enemy':
          f.write(f'E {e.animName} {e.rx} {e.ry} {e.x} {e.y} {e.hp} {e.dmg} {e.aibhvr} {e.spd}')
          if e.aibhvr == 1:
            f.write(f' {e.posnum}')
            for val in e.poslist:
              f.write(f' {val}')
          f.write('\n')
        elif e.__class__.__name__ == 'Player':
          f.write(f'P {e.rx} {e.ry} {e.x} {e.y} {e.bw} {e.bh} {e.spd} {e.hp}{chr(10)}')


if __name__ == '__main__':
  main()
