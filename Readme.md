![cendric logo](images/title.png)
# Cendric - RPG Platformer

*Cendric* is a 2D RPG Platformer game in which players explore a world by completing levels and quests.

The game is currently in development. See http://cendric.ch for more information about its development process.

## Gameplay

Cendric moves through the world on a 2D map and interacts with NPCs to trade or accept quests. At many places, he can enter levels in which he fights enemies, collects items or completes tricky platforming challenges. To accomplish all this, Cendric can master various spells from different classes of magic.

## Features

- Interactive Levels: Some of Cendric's spells affect the levels and change his surroundings in the levels.
- Modifiable Spells: All spells can be modified using gems to make them stronger or behave in new ways.
- Special Effects: The game contains particle effects, a water simulation and simple lighting.
- Localization: All texts in the game are available in English, German and Swiss German.

![cendric icons](images/icons.png)

## Current stable build

The final game will be available for Windows, Mac and Linux. As we are currently only focusing on one platform, most test builds will only be available for Windows at the moment. However, feel free to try and compile *Cendric* on any platform.

[Cendric v0.2.0](https://github.com/tizian/Cendric2/releases/download/v0.2.0/Cendric_v0.2.0_win64.zip) (Windows x64)

[Cendric v0.2.0](https://github.com/tizian/Cendric2/releases/download/v0.2.0/Cendric_v0.2.0_mac.zip) (Mac OS X)

[Cendric v0.2.0](https://github.com/tizian/Cendric2/releases/download/v0.2.0/Cendric_v0.2.0_ubuntu.tar.gz) (Ubuntu)


A list of all builds is available [here](https://github.com/tizian/Cendric2/releases).

## How to compile

The recommended way to compile the code is to use [**cmake**](https://cmake.org/), which automatically generates project files for your favourite IDE or build system:
```
git clone https://github.com/tizian/Cendric2.git --recursive
cd Cendric2
mkdir build
cd build
cmake ..
```

## Used Libraries

* [SFML](http://www.sfml-dev.org/) : Window creation, rendering and sound
* [SQLite](https://www.sqlite.org/) : Database for translations and items
* [Lua](http://www.lua.org/work/) and [LuaBridge](https://github.com/vinniefalco/LuaBridge) : Scripting
* [TinyXML 2](http://www.grinninglizard.com/tinyxml2/index.html) : XML parsing

## License

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />Game assets (images, fonts and sound files) found in the "res" folder are licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>. Give attribution to Isa Roesch & Tizian Zeltner, if you're going to use them.

