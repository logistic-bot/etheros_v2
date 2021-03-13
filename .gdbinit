file bin/kernel.elf
target remote :1234
hbreak _start

define hook-quit
	set confirm off
end

skip -rfu write_serial
skip -rfu serial_println
