# Ghostship for GC/Wii

This is a fork of [HarbourMasters/Ghostship](https://github.com/HarbourMasters/Ghostship),
the Super Mario 64 PC port, built to run **natively on the Nintendo GameCube and Wii**.

The game source is not rewritten. Instead of linking
[libultraship](https://github.com/Kenix3/libultraship), which targets PCs, this fork
links [libultragx](https://github.com/paradox-ng/libultragx), a reimplementation of
that runtime for devkitPPC and libogc. The same Fast3D display lists the PC port
renders through OpenGL are decoded here and driven through the GX fixed-function
pipeline, and the same `.o2r` asset archive is read off an SD card.

Upstream's own documentation is preserved unchanged at
[docs/README-upstream.md](docs/README-upstream.md), and everything it says about the
desktop builds still applies to them. This file covers the console build only.

**This fork ships no copyrighted assets and cannot produce them.** You supply your
own legally dumped ROM, exactly as upstream requires.

## Status

**The Wii build is playable start to finish.** It boots, renders the whole game,
plays music and sound effects, reads and writes saves, and holds its native 30 fps.

**The GameCube build compiles and links but has not been proven on hardware.** The
GameCube has no built-in SD slot, so reaching the asset archive needs an SD Gecko or
an SD2SP2 adapter, and that path is untested. GameCube is the design target, since
its 24 MB of RAM is the budget everything was built to fit, but the Wii is what
currently runs.

All testing so far has been under the Dolphin emulator. Dolphin does not reproduce
the real graphics hardware exactly, so real console testing is still outstanding.

## What you need

- A Wii (or a GameCube, once that build is proven), able to run homebrew
- An SD card
- Your own Super Mario 64 ROM, US or JP, in `.z64` format
- A PC, once, to turn that ROM into an asset archive

## Getting the asset archive

The console build cannot extract assets from a ROM itself; that step needs the
desktop build. Do it once on a PC:

1. Download an upstream [Ghostship release](https://github.com/HarbourMasters/Ghostship/releases)
   for your operating system.
2. Run it and select your ROM. It writes an `sm64.o2r` next to the executable.
3. Copy that `sm64.o2r` to your SD card as described below.

Upstream's README lists the SHA-1 hashes for the supported ROMs if you want to
verify your dump first.

## SD card layout

The game looks for a `Ghostship` folder at the root of the card. The usual Wii
arrangement, which the Homebrew Channel picks up automatically:

```
sd:/
  apps/
    ghostship/
      boot.dol            the build, renamed
      meta.xml            optional: name and description for the channel
      icon.png            optional: 128x48
  Ghostship/
    sm64.o2r              your asset archive, which you supply
    config.ini            created on first boot
    saves/                created when you first save
```

Only the `Ghostship` folder has a fixed location. The executable can live anywhere
your loader looks, and renaming it to `boot.dol` is safe: the data folder is named
after the game itself, not after the file, so the game still finds it.

The `saves` folder is created for you. The `Ghostship` folder is not, so make it
yourself when you copy your archive in.

## Controls

Three controllers work, and they are read together, so you can put one down and pick
another up without telling the game.

| N64 | GameCube pad | Wii Remote + Nunchuk | Classic Controller |
|---|---|---|---|
| Analog stick | Analog stick | Nunchuk stick | Left stick |
| A | A | A | a |
| B | B | B | b |
| Z | Z | Nunchuk Z | ZL or ZR |
| Start | Start | + | + |
| C buttons | C-stick | D-pad | Right stick |
| L | L | 1 | L |
| R | R | Nunchuk C | R |
| D-pad | D-pad | not mapped | D-pad |

A Wii Remote on its own is not supported: it has no analog stick, which the game
needs, and too few reachable buttons.

## Configuration

A `config.ini` is created in the game folder on first boot, with comments. An
existing file is never overwritten, so your settings survive updates.

| Key | Values | Default | Effect |
|---|---|---|---|
| `aspect_ratio` | `auto`, `4:3`, `16:9` | `auto` | `auto` follows the Wii's system setting, and is 4:3 on GameCube. `16:9` widens the field of view for a widescreen TV rather than stretching the picture |
| `fps_counter` | `true`, `false` | `false` | On-screen frame rate, top right |
| `frame_interpolation` | `true`, `false` | `false` | Renders in-between frames for 60 fps motion. The game itself still runs at 30 fps, and this costs noticeably more work per second |
| `antialiasing` | `true`, `false` | `false` | The console's hardware edge antialiasing, similar in spirit to the N64's. It smooths edges at the cost of vertical detail and some colour banding. Worth trying both |
| `debug_profiler` | `true`, `false` | `false` | Development diagnostic |

## Known limitations

- GameCube is unproven on hardware, as above.
- Effects that read back the framebuffer are inert, because render-to-texture is not
  implemented. Nothing in this game is known to depend on it.
- Antialiasing has not been judged on real hardware. Emulators do not reproduce the
  console's copy filter faithfully, so whether it is worth enabling is an open
  question rather than a recommendation.
- The in-game menu from the desktop build is not present. It is built on ImGui, which
  costs memory the GameCube does not have, so settings live in `config.ini` instead.
- Mods and the scripting runtime are not available on console.

## Building

The toolchain runs in Docker, so nothing is installed on your system:

```sh
git clone --recursive https://github.com/paradox-ng/Ghostship -b gx-port
cd Ghostship
docker run --rm -v "$PWD":/project -w /project -u "$(id -u):$(id -g)" \
    devkitpro/devkitppc:latest make -f Makefile.gx PLATFORM=wii -j$(nproc)
```

That produces `ghostship-wii.dol`. Use `PLATFORM=gamecube` for
`ghostship-gamecube.dol`. The `--recursive` clone matters: libultragx is a submodule
and has submodules of its own, and without them the build fails on missing headers.

The desktop build is unaffected by this fork and still uses upstream's CMake; see
[docs/building.md](docs/building.md).

## Credits

This fork is only a port. The game, the decompilation it rests on, and the PC port
being ported are all other people's work.

- [HarbourMasters/Ghostship](https://github.com/HarbourMasters/Ghostship) and
  [Lywx](https://www.github.com/kiritodv), for the Super Mario 64 PC port this forks.
  Their credits are in [docs/README-upstream.md](docs/README-upstream.md) and apply
  in full.
- [Kenix3](https://github.com/Kenix3) and the Harbour Masters contributors, for
  libultraship, the runtime libultragx reimplements.
- [sm64](https://github.com/n64decomp/sm64), the Super Mario 64 decompilation.
- [mkst](https://github.com/mkst/sm64-port)'s Wii branch of sm64-port, the reference
  for how Fast3D geometry reaches GX on this hardware.
- [devkitPro](https://devkitpro.org/), for devkitPPC and libogc.

## License

This fork keeps upstream Ghostship's license; see [LICENSE.md](LICENSE.md).
libultragx, the runtime it links, is separately licensed under GPL-3.0.
