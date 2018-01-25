#include <cstdio>
#include <libgen.h>
#include <dlfcn.h>
#include <sys/param.h>
#include "hook.hpp"
#include "libhl.hpp"
#include "libclient.hpp"
#include "libhw.hpp"

// Get bass address of a loaded library
static uintptr_t get_lib_base(const char *lib_name)
{
	task_dyld_info dyld_info;
	mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
	const kern_return_t ret = task_info(mach_task_self_, TASK_DYLD_INFO,
			(task_info_t)&dyld_info, &count);
	if (ret != KERN_SUCCESS) {
		return 0;
	}

	const mach_vm_address_t image_infos = dyld_info.all_image_info_addr;
	const auto *infos = (dyld_all_image_infos *)image_infos;
	const dyld_image_info *image_array = infos->infoArray;
	for (uint32_t i = 0; i < infos->infoArrayCount; ++i) {
		if (strstr(image_array->imageFilePath, lib_name)) {
			return (uintptr_t)image_array->imageLoadAddress;
		}
		image_array++;
	}
	return 0;
}

// Initialise all libraries hooks that match the path name
static void init_libs(const char *path)
{
	if (!path) {
		return;
	}

	LibBase *libs[] = {g_libhl(), g_libhw(), g_libclient()};
	char basename_buf[MAXPATHLEN] = {0};
	for (LibBase *lib : libs) {
		if (!basename_r(path, basename_buf)) {
			continue;
		}
		if (strcmp(basename_buf, lib->name()) == 0) {
			lib->init(get_lib_base(path), path);
			return;
		}
	}
}

static void *_dlopen(const char *path, int mode)
{
	void *ret = dlopen(path, mode);
	init_libs(path);
	return ret;
}
DYLD_INTERPOSE(_dlopen, dlopen)
