#include "cvar.hpp"

namespace Cvar {
	cvar_t mh_hud_rect = {"mh_hud_rect", "1"};
	
	static bool cvar_registered = false;

	void register_cvars(register_func_t func)
	{
		if (cvar_registered) {
			return;
		}

		func(&mh_hud_rect);

		cvar_registered = true;
	}
}
