import json
def find_name(j,name,id="name"):
    for v in j:
        if v[id]==name: return v
    return None
def load_layer(file,name):
    with open(file,"r") as fp:
        j = json.load(fp)
    layer=find_name(j["layers"],name)
    return layer
def print_point(name,layer):
    print(f'const u8 const {name}[] = {"{"}')
    for v in layer["objects"]:
        print(f"    0x{v['x']:02x}, 0x{v['y']:02x},")
    print('};')
def load_tile(file):
    tile=load_layer(file,"tile")
    for i in range(len(tile["data"])):
        tile["data"][i]-=1
    return tile
def print_tile(name,tile):
    print(f'const u8 const {name}[] = {"{"}')
    for i,v in enumerate(tile['data']):
        if i % tile["width"] == 0: print("    ",end="")
        print(f"0x{v:02x}, ",end="")
        if i % tile["width"] == tile["width"]-1: print("")
    print('};')

print('#include "bios.h"')
print_tile("bg",load_tile("bg.tmj"))
print_tile("messagePatternNameTable",load_tile("msg.tmj"))
print_point("enemyPointTable",load_layer("bg.tmj","enemyPointTable"))
