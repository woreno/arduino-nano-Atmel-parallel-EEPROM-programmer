# arduino-nano-eeprom-programmer
Atmel AT28C64/128/256 parallel EEPROMs programmer using Arduino Nano Every

This project was from 2020. I tried to build a board following Ben Eater's schematic, for accepting different sizes of parallel EEPROMS using switches. After i realized that some EEPROMs i bought in AliExpress had Software Data Protection (SDP), so i needed to add code to unblock these. However some failed to unprotect using this method (because the filling the serial register takes times), so i had to use a larger GPIO, Arduino Uno, to unblock them using all the GPIO in parallel. These protected PROMs need faster speeds for
unprotect (the periods are in Atmel datasheets and SDP applies only, i believe, to the 256 and beyond chips).
