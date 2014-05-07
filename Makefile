

CC          := /usr/bin/arm-none-eabi-gcc
OBJCOPY     := /usr/bin/arm-none-eabi-objcopy
NRF51_SDK   := ./nrf51822

default: main.bin main.hex

main.o: main.c
	$(CC) -mcpu=cortex-m0 -mthumb -DBOARD_PCA10001 -DNRF51 -I$(NRF51_SDK)/Include -I$(NRF51_SDK)/Include/gcc -c $<

system_nrf51.o: $(NRF51_SDK)/Source/templates/system_nrf51.c 
	$(CC) -mcpu=cortex-m0 -mthumb -DBOARD_PCA10001 -DNRF51 -I$(NRF51_SDK)/Include -I$(NRF51_SDK)/Include/gcc -c $< 

nrf_delay.o: $(NRF51_SDK)/Source/nrf_delay/nrf_delay.c
	$(CC) -mcpu=cortex-m0 -mthumb -DBOARD_PCA10001 -DNRF51 -I$(NRF51_SDK)/Include -I$(NRF51_SDK)/Include/gcc -c $< 

gcc_startup_nrf51.o: $(NRF51_SDK)/Source/templates/gcc/gcc_startup_nrf51.s 
	$(CC) -mcpu=cortex-m0 -mthumb -DBOARD_PCA10001 -DNRF51 -I$(NRF51_SDK)/Include -I$(NRF51_SDK)/Include/gcc -c $< 

main.out: main.o system_nrf51.o nrf_delay.o gcc_startup_nrf51.o
	$(CC) -L"/usr/lib/gcc/arm-none-eabi/4.8.3/armv6-m" -L"/usr/lib/gcc/arm-none-eabi/4.8.3/armv6-m" -Xlinker -Map=main.map -mcpu=cortex-m0 -mthumb -mabi=aapcs -T$(NRF51_SDK)/Source/templates/gcc/gcc_nrf51_common2.ld main.o system_nrf51.o nrf_delay.o gcc_startup_nrf51.o -o main.out

main.bin: main.out
	$(OBJCOPY) -O binary main.out main.bin

main.hex: main.out
	$(OBJCOPY) -O ihex main.out main.hex

install: main.bin
	sed  's#\[\[--filename--\]\]#$(PWD)/main.bin#' segger/burn-template.seg > burn.seg
	./segger/segger.sh $(PWD)/burn.seg

clean:
	rm *.o *.out *.hex *.seg *.map *.bin *.hex
