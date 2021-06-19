# GTAV-1HKO

A one-hit KO mod for GTA V (singleplayer).

* You to die if you take any damage.
* Trevor's power is locked, you can't use it.
* Settings:
  * Key **K**: Toggle the mod on/off (reset)
  * Key **J**: Toggle train mode (hit count is incremented and you don't die)
  * Key **Ctrl+J**: Manual mode: Decrement hit count
  * Key **Ctrl+K**: Manual mode: Increment hit count
  * Key **Ctrl+L**: Reload display settings file (1HKO.ini)
  
* Display settings:
  * They are saved in the file "1HKO.ini"
  * The file is automatically created when you start the game if it doesn't exist yet
  * You can modify the file while the game is running, just reload with **Ctrl+L** after your modifications
  * If you mess up the file, delete it and reload in game with **Ctrl+L**, it will be re-created
  * Parameters:
    * offset_x: 0 is the left side of the screen, 1 is the right side (text will be out of the screen)
    * offset_y: 0 is the top side of the screen, 1 is the bottom side (text will be out of the screen)
    * size: Text size
    * red: Red value (0 to 255) for the text colour
    * green: Green value (0 to 255) for the text colour
    * blue: Blue value (0 to 255) for the text colour

## Requirements

- [ScriptHookV](https://www.gta5-mods.com/tools/script-hook-v)

## Installation

Download and place `1HKO.asi` in your GTA V folder.
