#pragma once

namespace Cvar {
	struct cvar_t {
		const char *name;
		const char *string;
		int flags;
		float value;
		cvar_t *next;
	};

	typedef void (*register_func_t)(void *);
	void register_cvars(register_func_t func);

	// Add all cvars here
	extern cvar_t mh_hud_rect;
}

