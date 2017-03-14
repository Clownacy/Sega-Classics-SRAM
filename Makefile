GenesisEmuWrapper.dll: main.c
	gcc -O3 -s --shared -static-libgcc $^ -o $@
