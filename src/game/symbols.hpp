#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	// Functions
	
	WEAK symbol<void(int type, VariableUnion u)> AddRefToValue{0x5C0EB0};
	WEAK symbol<void(int type, VariableUnion u)> RemoveRefToValue{0x5C29B0};

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{0x59A050};

	WEAK symbol<void(const char* cmdName, void(), cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{0x59A5F0};
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{0x59ABA0};

	WEAK symbol<void(XAssetType type, void(__cdecl* func)(game::XAssetHeader, void*), const void* inData, bool includeOverride)>
		DB_EnumXAssets_Internal{0x4129F0};
	WEAK symbol<const char* (const XAsset* asset)> DB_GetXAssetName{0x3E4090};

	WEAK symbol<dvar_t* (const char* name)> Dvar_FindVar{0x618F90};
	WEAK symbol<void(char* buffer, int index)> Dvar_GetCombinedString{0x5A75D0};
	WEAK symbol<dvar_t* (const char* dvarName, bool value, unsigned int flags, const char* description)>
		Dvar_RegisterBool{0x617BB0};
	WEAK symbol<dvar_t* (int dvarName, const char* a2, float x, float y, float z, float w, float min, float max,
		unsigned int flags)> Dvar_RegisterVec4{0x6185F0};
	WEAK symbol<const char* (dvar_t* dvar, void* a2, void* value)> Dvar_ValueToString{0x61B8F0};
	WEAK symbol<void(int hash, const char* name, const char* buffer)> Dvar_SetCommand{0x61A5C0};

	WEAK symbol<int(const char* fname)> generateHashValue{0x343D20};

	WEAK symbol<unsigned int(int entnum, unsigned int classnum)> FindEntityId{0x5C1C50};
	WEAK symbol<void(VariableValue* result, unsigned int classnum, int entnum, int offset)> GetEntityFieldValue{0x5C6100};

	WEAK symbol<Material*(const char* material)> Material_RegisterHandle{0x759BA0};

	WEAK symbol<const float* (const float* v)> Scr_AllocVector{0x5C3220};
	WEAK symbol<void()> Scr_ClearOutParams{0x5C6E50};
	WEAK symbol<scr_entref_t(unsigned int entId)> Scr_GetEntityIdRef{0x5C56C0};
	WEAK symbol<int(unsigned int classnum, int entnum, int offset)> Scr_SetObjectField{0x512190};
	WEAK symbol<void(unsigned int id, scr_string_t stringValue, unsigned int paramcount)> Scr_NotifyId{0x5C8240};

	WEAK symbol<void(float x, float y, float width, float height, float s0, float t0, float s1, float t1,
					 float* color, Material* material)> R_AddCmdDrawStretchPic{0x3C9710};
	WEAK symbol<void(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, 
					 float rotation, float* color, int style)> R_AddCmdDrawText{0x76C660};
	WEAK symbol<void(const char* text, int maxChars, Font_s* font, int fontSize, float x, float y, float xScale, float yScale, float rotation, 
		const float* color, int style, int cursorPos, char cursor)> R_AddCmdDrawTextWithCursor{0x76CAF0};
	WEAK symbol<Font_s*(const char* font, int size)> R_RegisterFont{0x746FE0};
	WEAK symbol<int(const char* text, int maxChars, Font_s* font)> R_TextWidth{0x7472A0};

	WEAK symbol<ScreenPlacement* ()> ScrPlace_GetViewPlacement{0x3E16A0};

	WEAK symbol<const char*(scr_string_t stringValue)> SL_ConvertToString{0x5BFBB0};
	WEAK symbol<scr_string_t(const char* str, unsigned int user)> SL_GetString{0x5C0170};

	WEAK symbol<void()> Sys_ShowConsole{0x633080};

	WEAK symbol<void* (jmp_buf* Buf, int Value)> longjmp{0x89EED0};
	WEAK symbol<int(jmp_buf* Buf)> _setjmp{0x8EC2E0};

	// Variables

	WEAK symbol<cmd_function_s*> cmd_functions{0xAD17BB8};
	WEAK symbol<CmdArgs> cmd_args{0xAD17A60};

	WEAK symbol<const char*> g_assetNames{0xBEF280};

	WEAK symbol<gentity_s> g_entities{0x52DDEC0};

	WEAK symbol<int> keyCatchers{0x203F3C0};

	WEAK symbol<PlayerKeyState> playerKeys{0x1E8767C};

	WEAK symbol<int> dvarCount{0xBFBB310};
	WEAK symbol<dvar_t*> sortedDvars{0xBFBB320};

	WEAK symbol<unsigned int> levelEntityId{0xB5E0B30};
	WEAK symbol<int> g_script_error_level{0xBA9CC24};
	WEAK symbol<jmp_buf> g_script_error{0xBA9CD40};
	WEAK symbol<scr_classStruct_t> g_classMap{0xBF95C0};

	WEAK symbol<scrVmPub_t> scr_VmPub{0xBA9EE40};
}
