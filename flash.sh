openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program build/bootloader.bin 0x8000000 verify reset exit"
