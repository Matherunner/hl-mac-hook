#include "libhl.hpp"
#include "libhw.hpp"

LibHL *g_libhl()
{
	static auto *libhl = new LibHL();
	return libhl;
}

static const char *s_pmove = "_pmove";
static const char *s_PM_Jump = "_PM_Jump";
static const char *s_CBasePlayer_TakeDamage = "__ZN11CBasePlayer10TakeDamageEP9entvars_sS1_fi";

LibHL::LibHL()
	: LibBase({
		s_pmove,
		s_PM_Jump,
		s_CBasePlayer_TakeDamage,
	})
{
	_lib_name = "hl.dylib";
}

void LibHL::post_symtab_search()
{
	_gdata.ppmove = _sym_names[s_pmove];
	HOOKDEF(s_PM_Jump, PM_Jump);
	HOOKDEF(s_CBasePlayer_TakeDamage, CBasePlayer_TakeDamage);
}

void LibHL::my_PM_Jump()
{
	g_libhl()->orig_PM_Jump();
	const auto pmove = *(uintptr_t *)g_libhl()->gdata().ppmove;
	const auto pvelocity = pmove + LibHL::offset.playermove_s__velocity;
	auto *velocity = (float *)pvelocity;
	velocity[2] = 400;
}

int LibHL::my_CBasePlayer_TakeDamage(uintptr_t pthis, uintptr_t inflictor,
		uintptr_t attacker, float damage, int damage_type)
{
	g_libhw()->orig_Con_Printf("Hooked player TakeDamage: %g\n", (double)damage);
	return g_libhl()->orig_CBasePlayer_TakeDamage(pthis, inflictor, attacker,
			damage, damage_type);
}
