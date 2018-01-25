#pragma once

#include "libbase.hpp"

class LibHL : public LibBase {
	struct GData {
		uintptr_t ppmove;
	};

	struct Offset {
		ptrdiff_t playermove_s__velocity;
	};

	HOOKDECL(void, PM_Jump, ())
	HOOKDECL(int, CBasePlayer_TakeDamage, (uintptr_t, uintptr_t, uintptr_t, float, int))

	public:
		LibHL();
		inline const GData &gdata() const { return _gdata; }

		constexpr static Offset offset{
			.playermove_s__velocity = 0x5c,
		};

	protected:
		virtual void post_symtab_search();

	private:
		GData _gdata;
};

LibHL *g_libhl();
