// SRAM bugfix for 'SEGA Mega Drive & Genesis Classics'
// Copyright © 2017 Clownacy

#include <stdio.h>
#include <windows.h>

int (*GetAudioFrame_orig)(float *var1, int var2);
int (*GetAverageColor_orig)(void);
void* (*GetRomState_orig)(void *var1);
int (*GetRomStateSize_orig)(void);
int (*InitRom_orig)(char *var1, char *var2, char var3, char var4, int var5, int var6, int sram_size, char var8);
char (*LoadROMState_orig)(int var1);
int (*PowerSwitch_orig)(int var1);
void (*Release_orig)(void);
char (*SaveROMState_orig)(int var1);
int (*SetBramUpdateCallback_orig)(int (*var1)(void));
int (*SetRegion_orig)(int var1);
int (*SetRomState_orig)(void *var1, size_t var2);
int (*SetScreenSizeCallback_orig)(int var1);
void (*SetTexturePtr_orig)(int var1);
void (*SetTime_orig)(float var1);
int (*UnityRenderEvent_orig)(void);
int (*Update_orig)(float var1, short var2, short var3, char var4);

void LoadOriginalDLL(void)
{
	static HMODULE original_dll = NULL;
			
	if (original_dll == NULL)
	{
		original_dll = LoadLibrary("SEGAGameRoom_Data/Plugins/GenesisEmuWrapperOriginal.dll");

		GetAudioFrame_orig = (int(*)(float *var1, int var2))GetProcAddress(original_dll, "GetAudioFrame");
		GetAverageColor_orig = (int(*)(void))GetProcAddress(original_dll, "GetAverageColor");
		GetRomState_orig = (void*(*)(void *var1))GetProcAddress(original_dll, "GetRomState");
		GetRomStateSize_orig = (int(*)(void))GetProcAddress(original_dll, "GetRomStateSize");
		InitRom_orig = (int(*)(char *var1, char *var2, char var3, char var4, int var5, int var6, int sram_size, char var8))GetProcAddress(original_dll, "InitRom");
		LoadROMState_orig = (char(*)(int var1))GetProcAddress(original_dll, "LoadROMState");
		PowerSwitch_orig = (int(*)(int var1))GetProcAddress(original_dll, "PowerSwitch");
		Release_orig = (void(*)(void))GetProcAddress(original_dll, "Release");
		SaveROMState_orig = (char(*)(int var1))GetProcAddress(original_dll, "SaveROMState");
		SetBramUpdateCallback_orig = (int(*)(int (*var1)(void)))GetProcAddress(original_dll, "SetBramUpdateCallback");
		SetRegion_orig = (int(*)(int var1))GetProcAddress(original_dll, "SetRegion");
		SetRomState_orig = (int(*)(void *var1, size_t var2))GetProcAddress(original_dll, "SetRomState");
		SetScreenSizeCallback_orig = (int(*)(int var1))GetProcAddress(original_dll, "SetScreenSizeCallback");
		SetTexturePtr_orig = (void(*)(int var1))GetProcAddress(original_dll, "SetTexturePtr");
		SetTime_orig = (void(*)(float var1))GetProcAddress(original_dll, "SetTime");
		UnityRenderEvent_orig = (int(*)(void))GetProcAddress(original_dll, "UnityRenderEvent");
		Update_orig = (int(*)(float var1, short var2, short var3, char var4))GetProcAddress(original_dll, "Update");
	}
}

__declspec(dllexport) int GetAudioFrame(float *var1, int var2)
{
	LoadOriginalDLL();

	return GetAudioFrame_orig(var1, var2);
}

__declspec(dllexport) int GetAverageColor(void)
{
	LoadOriginalDLL();

	return GetAverageColor_orig();
}

__declspec(dllexport) void* GetRomState(void *var1)
{
	LoadOriginalDLL();

	return GetRomState_orig(var1);
}

__declspec(dllexport) int GetRomStateSize(void)
{
	LoadOriginalDLL();

	return GetRomStateSize_orig();
}

__declspec(dllexport) int InitRom(char *rom_path, char *rom_name, char is_pak_file, char var4, int sram_flag, int sram_address, int sram_size, char var8)
{
	LoadOriginalDLL();

	#ifdef DEBUG
	remove("clown_debug.txt");
	FILE *file = fopen("clown_debug.txt", "a");
	#endif

	if (rom_path[0] != '\0' && !is_pak_file)
	{
		// File is actually a hack ROM
		#ifdef DEBUG
		fprintf(file, "Detected ROM: %s\n\n", rom_name);
		#endif

		FILE *rom_file = fopen(rom_path, "rb");

		fseek(rom_file, 0x1B0, SEEK_SET);

		if (fgetc(rom_file) == 'R' && fgetc(rom_file) == 'A')
		{
			// SRAM detected
			#ifdef DEBUG
			fprintf(file, "ROM uses SRAM\n\n");
			#endif

			fseek(rom_file, 0x1B4, SEEK_SET);

			sram_address = (fgetc(rom_file) << 24) | (fgetc(rom_file) << 16) | (fgetc(rom_file) << 8) | fgetc(rom_file);
			int sram_end = (fgetc(rom_file) << 24) | (fgetc(rom_file) << 16) | (fgetc(rom_file) << 8) | fgetc(rom_file);
			sram_size = (sram_end - (sram_address & ~1)) + 1;

			sram_flag = 1;

			#ifdef DEBUG
			fprintf(file, "SRAM start = 0x%X\nSRAM end = 0x%X\nSRAM size = 0x%X\n\n\n", sram_address, sram_end, sram_size);
			#endif
		}
		#ifdef DEBUG
		else
		{
			fprintf(file, "ROM does not use SRAM\n\n\n");
		}
		#endif

		fclose(rom_file);
	}

	#ifdef DEBUG
	fclose(file);
	#endif

	return InitRom_orig(rom_path, rom_name, is_pak_file, var4, sram_flag, sram_address, sram_size, var8);
}

__declspec(dllexport) char LoadROMState(int var1)
{
	LoadOriginalDLL();

	return LoadROMState_orig(var1);
}

__declspec(dllexport) int PowerSwitch(int var1)
{
	LoadOriginalDLL();

	return PowerSwitch_orig(var1);
}

__declspec(dllexport) void Release(void)
{
	LoadOriginalDLL();

	return Release_orig();
}

__declspec(dllexport) char SaveROMState(int var1)
{
	LoadOriginalDLL();

	return SaveROMState_orig(var1);
}

__declspec(dllexport) int SetBramUpdateCallback(int (*var1)(void))
{
	LoadOriginalDLL();

	return SetBramUpdateCallback_orig(var1);
}

__declspec(dllexport) int SetRegion(int var1)
{
	LoadOriginalDLL();

	return SetRegion_orig(var1);
}

__declspec(dllexport) int SetRomState(void *var1, size_t var2)
{
	LoadOriginalDLL();

	return SetRomState_orig(var1, var2);
}

__declspec(dllexport) int SetScreenSizeCallback(int var1)
{
	LoadOriginalDLL();

	return SetScreenSizeCallback_orig(var1);
}

__declspec(dllexport) void SetTexturePtr(int var1)
{
	LoadOriginalDLL();

	return SetTexturePtr_orig(var1);
}

__declspec(dllexport) void SetTime(float var1)
{
	LoadOriginalDLL();

	return SetTime_orig(var1);
}

__declspec(dllexport) int UnityRenderEvent(void)
{
	LoadOriginalDLL();

	return UnityRenderEvent_orig();
}

__declspec(dllexport) int Update(float var1, short var2, short var3, char var4)
{
	LoadOriginalDLL();

	return Update_orig(var1, var2, var3, var4);
}
