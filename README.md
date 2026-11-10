# arduino-nano-eeprom-programmer
Atmel AT28C64/128/256 programmer made with Arduino Nano

This project was from 2020. I follow the Ben Eater's schematic and created the code for it. I bought some EEPROMs protected so i added code to unblock these. Some EEPROMs i bought however failed to unprotect so i had to use an Arduino Uno to unblock them using all the GPIO in parallel. These might need faster speeds for
uunblocking as this approach using shift register is slow. 
