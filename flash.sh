openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program build/bootloader.elf verify reset exit"
