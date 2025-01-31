import os
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"
import pygame

ps = []
image = pygame.image.load("out.png")
(w,h)=image.get_size()
for yy in range(h//8):
    for xx in range(w//8):
        for y1 in range(8):
            y = y1+yy*8
            p = 0
            for x1 in range(8):
                x = x1+xx*8
                v=image.get_at_mapped((x,y))
                p=(p<<1)+bool(v)
            ps.append(p)
print('#include "bios.h"')
print('const u8 const pattern[] = {')

for i,v in enumerate(ps):
    if i % 16 == 0:print("    ",end="")
    print(f"0x{v:02x}, ",end="")
    if i % 16 == 15:print("")
print('};')

ps = []
image = pygame.image.load("sprite.png")
(w,h)=image.get_size()
for yy in range(h//16):
    for xx in range(w//8):
        for y1 in range(16):
            y = y1+yy*16
            p = 0
            for x1 in range(8):
                x = x1+xx*8
                v=image.get_at_mapped((x,y))
                p=(p<<1)+bool(v)
            ps.append(p)

print('const u8 const spritePattern[] = {')

for i,v in enumerate(ps):
    if i % 16 == 0:print("    ",end="")
    print(f"0x{v:02x}, ",end="")
    if i % 16 == 15:print("")
print('};')
