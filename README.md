# RENDEX

Rendex is a Casio Graphics Calculator add-in developed purely as a proof of concept, in order to see how far the SuperH3 architecture and a monochromatic screen can be pushed. The final concept is to have a software renderer that can load OBJ files, and render meshes and also rasterize models and transform them in real time.

## Installation

Unfortunately, setting up a development envrionment is woesome, but to install it on your own SH3 Casio calculator, download rendex.G1A from the release section, and either look into installing [p7](https://p7.planet-casio.com/), or [FA-124](https://edu.casio.com/education/support_software/dl/PC_links/fa124_inst_204_2.zip) to use in accordance with a USB cable or 3-pin cable to transfer the file onto your calculator as an add-in.

#### P7 command (UNIX based Operating systems)

```bash
sudo p7 send "path to rendex.g1a"
```
#### FA-124 command (Windows)
```bash
FA-124 is GUI based application, so just click and dragging will pull you through here :)
```

## Dependencies
#### GINT : Alternative library and kernel for CASIO OS
[Gint](https://gitea.planet-casio.com/Lephenixnoir/gint)
#### NEWLIB : Alternative libc for CASIO
[New-lib](https://git.planet-casio.com/Memallox/libc)
#### FXLIB : Standard Casio Library paired with in-house SDK. Linked is a modded version of it.
[fxlib](http://revolution-fx.sourceforge.net/)

## COMPILATION
Compilation was achieved with a custom GCC Toolchain.
sh3eb-elf-gcc to be exact. Will add more info later. For now, If you run arch, or have access to a package manager such as yaourt, yay, etc... It should be downloadable:
```bash
yay -Ss sh3eb-elf-gcc
yay -S "result that was found to be close enough to above search"
```


## License
[MIT](https://choosealicense.com/licenses/mit/)
