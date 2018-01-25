#include "hud.hpp"
#include "cvar.hpp"
#include "libhl.hpp"
#include "libhw.hpp"
#include "libclient.hpp"

void HUD::draw(float time)
{
	draw_hello(time);
	draw_speed(time);
}

void HUD::draw_hello(float time)
{
	if (Cvar::mh_hud_rect.value == 0.0f) {
		return;
	}

	g_libhw()->orig_Draw_FillRGBA(30, 30, 60, 60,
			128 + (int)(127 * sinf(2 * time)), 255, 0, 255);
	(*g_libclient()->porig_DrawSetTextColor)(1.0f, 0.7f, 0.0f);
	(*g_libclient()->porig_DrawConsoleString)(30, 100, "Hello from Mac Hook custom HUD!");
}

void HUD::draw_speed(float)
{
	const auto pmove = *(uintptr_t *)g_libhl()->gdata().ppmove;
	const auto pvelocity = pmove + g_libhl()->offset.playermove_s__velocity;
	const auto *velocity = (float *)pvelocity;
	char buf[30];
	snprintf(buf, sizeof(buf), "%g", (double)hypotf(velocity[0], velocity[1]));
	(*g_libclient()->porig_DrawConsoleString)(30, 120, buf);
}
