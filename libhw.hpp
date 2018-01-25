#pragma once

#include "libbase.hpp"

class LibHW : public LibBase {
	struct GData {
		// Add data here
	};

	struct Offset {
		// Add offsets
	};

	ORIGDECL(void, Con_Printf, (const char *, ...))
	ORIGDECL(void, Draw_FillRGBA, (int, int, int, int, int, int, int, int))
	ORIGDECL(void, Cvar_RegisterVariable, (void *))
	HOOKDECL(int, build_number, ())
	HOOKDECL(void, Memory_Init, (uintptr_t, int))
	HOOKDECL(void, Cbuf_Execute, ())

	public:
		LibHW();

		inline const GData &gdata() const { return _gdata; }

	protected:
		virtual void post_symtab_search();

	private:
		GData _gdata;
};

LibHW *g_libhw();
