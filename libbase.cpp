#include <mach-o/dyld.h>
#include <mach-o/dyld_images.h>
#include <mach-o/nlist.h>
#include "mach_override.h"
#include "libbase.hpp"

LibBase::LibBase(std::initializer_list<const char *> sym_names)
{
	for (const char *name : sym_names) {
		_sym_names[name] = 0;
	}
}

LibBase::~LibBase()
{
}

void LibBase::init(uintptr_t base, const char *path)
{
	FILE *lib_file = fopen(path, "rb");
	if (!lib_file) {
		throw std::runtime_error("unable to open library file");
	}

	fseek(lib_file, 0, SEEK_END);
	auto file_size = (size_t)ftell(lib_file);
	fseek(lib_file, 0, SEEK_SET);
	auto data = (uint8_t *)malloc(file_size);
	fread(data, 1, file_size, lib_file);
	fclose(lib_file);

	const auto baseaddr = (uintptr_t)data;
	const auto *header = (mach_header *)data;
	const auto *segcmd = (segment_command *)(baseaddr + sizeof(*header));
	const symtab_command *symcmd = nullptr;

	// Get symtab segment command
	for (uint32_t i = 0; i < header->ncmds; ++i) {
		if (segcmd->cmd == LC_SYMTAB) {
			symcmd = (symtab_command *)segcmd;
			break;
		}
		segcmd = (segment_command *)((uintptr_t)segcmd + segcmd->cmdsize);
	}

	if (!symcmd) {
		free(data);
		throw std::runtime_error("unable to find symtab command");
	}

	// Get the string table base
	const auto *strtab = (char *)(baseaddr + symcmd->stroff);

	// Point to the symbol table
	const auto *symtab = (struct nlist *)(baseaddr + symcmd->symoff);
	for (uint32_t i = 0; i < symcmd->nsyms; ++i) {
		const auto *entry = symtab + i;
		if (!(entry->n_type & N_SECT)) {
			continue;
		}

		// Get the name from the string table
		const auto *entry_name = strtab + entry->n_un.n_strx;
		const auto it = _sym_names.find(entry_name);
		if (it == _sym_names.end()) {
			continue;
		}

		it->second = base + entry->n_value;
	}

	free(data);
	post_symtab_search();
}

void LibBase::override_func(const char *name, const void *hooked_func,
		void **orig_func) const
{
	auto *func = (void *)_sym_names.at(name);
	mach_error_t err = mach_override_ptr(func, hooked_func, orig_func);
	if (err) {
		std::string msg = "failed to hook: ";
		msg += name;
		throw std::runtime_error(msg);
	}
}
