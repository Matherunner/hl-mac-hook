#pragma once

#include <unordered_map>
#include <string>
#include <initializer_list>

#define ORIGDECLP(ret, name, args) \
	public: \
		ret (**porig_##name)args = nullptr; \
	private:

#define ORIGDECL(ret, name, args) \
	public: \
		ret (*orig_##name)args = nullptr; \
	private:

#define ORIGDEF(sym, name) \
	orig_##name = (decltype(orig_##name))_sym_names[sym];

#define HOOKDECL(ret, name, args) \
	private: \
		static ret my_##name args; \
	public: \
		ret (*orig_##name)args = nullptr; \
	private:

#define HOOKDEF(sym, name) \
	override_func(sym, (void *)my_##name, (void **)&orig_##name);


class LibBase {
	public:
		LibBase(std::initializer_list<const char *> sym_names);
		virtual ~LibBase();

		void init(uintptr_t base, const char *path);
		inline const char *name() const { return _lib_name; }

	protected:
		std::unordered_map<std::string, uintptr_t> _sym_names;
		const char *_lib_name;

		virtual void post_symtab_search() = 0;
		void override_func(const char *name, const void *hooked_func,
				void **orig_func) const;
};
