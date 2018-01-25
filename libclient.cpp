#include "libclient.hpp"
#include "libhw.hpp"
#include "libhl.hpp"
#include "hud.hpp"

LibClient *g_libclient()
{
	static auto *libclient = new LibClient();
	return libclient;
}

static const char *s_gEngfuncs = "_gEngfuncs";
static const char *s_HUD_Redraw = "_HUD_Redraw";

LibClient::LibClient()
	: LibBase({
		s_gEngfuncs,
		s_HUD_Redraw,
	})
{
	_lib_name = "client.dylib";
}

void LibClient::post_symtab_search()
{
	_gdata.pgEngfuncs = _sym_names[s_gEngfuncs];

	porig_DrawConsoleString = (decltype(porig_DrawConsoleString))(
			_gdata.pgEngfuncs + offset.cl_enginefunc_t__DrawConsoleString);
	porig_DrawSetTextColor = (decltype(porig_DrawSetTextColor))(
			_gdata.pgEngfuncs + offset.cl_enginefunc_t__DrawSetTextColor);

	HOOKDEF(s_HUD_Redraw, HUD_Redraw);
}

void LibClient::my_HUD_Redraw(float time, int intermission)
{
	g_libclient()->orig_HUD_Redraw(time, intermission);
	g_libclient()->_hud.draw(time);
}
