# rp2040_msc

A POC for using the internal flash storage to transfer files to and from RP2040 equipped boards.

### Credits/dependencies

[FatFs](http://elm-chan.org/fsw/ff/00index_e.html)<br>
[TinyUSB](https://github.com/hathach/tinyusb)<br>
Also, a lot of snippets are borrowed from or inspired by [Micropython](https://github.com/micropython/micropython).

### Caveats

FatFs doesn't have any wear levelling algorithms, and flash storage wears out faster than, for example, EEPROM. Therefore, it's advisable to keep writing to a minimum.