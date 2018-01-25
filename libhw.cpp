#include "libhw.hpp"
#include "cvar.hpp"

LibHW *g_libhw()
{
	static auto *libhw = new LibHW();
	return libhw;
}

static const char *s_build_number = "_build_number";
static const char *s_Draw_FillRGBA = "_Draw_FillRGBA";
static const char *s_Con_Printf = "_Con_Printf";
static const char *s_Cvar_RegisterVariable = "_Cvar_RegisterVariable";
static const char *s_Memory_Init = "_Memory_Init";
static const char *s_Cbuf_Execute = "_Cbuf_Execute";

LibHW::LibHW()
	: LibBase({
		s_build_number,
		s_Draw_FillRGBA,
		s_Con_Printf,
		s_Cvar_RegisterVariable,
		s_Memory_Init,
		s_Cbuf_Execute,
	})
{
	_lib_name = "hw.dylib";
}

void LibHW::post_symtab_search()
{
	ORIGDEF(s_Draw_FillRGBA, Draw_FillRGBA);
	ORIGDEF(s_Con_Printf, Con_Printf);
	ORIGDEF(s_Cvar_RegisterVariable, Cvar_RegisterVariable);

	HOOKDEF(s_build_number, build_number);
	HOOKDEF(s_Memory_Init, Memory_Init);
	HOOKDEF(s_Cbuf_Execute, Cbuf_Execute);
}

int LibHW::my_build_number()
{
	return g_libhw()->orig_build_number() * 10000 + 1337;
}

void LibHW::my_Memory_Init(uintptr_t buf, int size)
{
	g_libhw()->orig_Memory_Init(buf, size);
	Cvar::register_cvars(g_libhw()->orig_Cvar_RegisterVariable);
}

void LibHW::my_Cbuf_Execute()
{
	g_libhw()->orig_Cbuf_Execute();
}
