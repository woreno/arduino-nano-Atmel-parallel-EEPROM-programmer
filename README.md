# arduino-nano-eeprom-programmer
Atmel AT28C64/128/256 parallel EEPROMs programmer using Arduino Nano Every

This gadget was from 2020. I tried to build a board following Ben Eater's schematic, for accepting different sizes of parallel EEPROMS using switches. After i realized that some EEPROMs i bought in AliExpress had Software Data Protection (SDP), so i needed to add code to unblock these. However some failed to unprotect using this method (because filling the serial register takes some time and the specs demands fasters speeds for SPD commands), so i had to use an Arduino Uno with more GPIO to unblock them, using all the GPIO in parallel. See the correct timmings in Atmel datasheets; SDP applies only, i believe, to the 256 and beyond chips.
