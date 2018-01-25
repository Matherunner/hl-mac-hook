#pragma once

#include "libbase.hpp"
#include "hud.hpp"

class LibClient : public LibBase {
	struct GData {
		uintptr_t pgEngfuncs;
	};

	struct Offset {
		ptrdiff_t cl_enginefunc_t__DrawConsoleString;
		ptrdiff_t cl_enginefunc_t__DrawSetTextColor;
	};

	ORIGDECLP(int, DrawConsoleString, (int, int, const char *))
	ORIGDECLP(void, DrawSetTextColor, (float, float, float))
	HOOKDECL(void, HUD_Redraw, (float, int))

	public:
		LibClient();

		inline const GData &gdata() const { return _gdata; }

		constexpr static Offset offset{
			.cl_enginefunc_t__DrawConsoleString = 0x6c,
			.cl_enginefunc_t__DrawSetTextColor = 0x70,
		};

	protected:
		virtual void post_symtab_search();

	private:
		GData _gdata;
		HUD _hud;
};

LibClient *g_libclient();
