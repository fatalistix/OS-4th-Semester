#include <dlfcn.h>

int main()
{
	void * lib = dlopen("lib/libhello_dynamic_runtime.so", RTLD_LAZY);
	if (!lib)
	{
		return 1;
	}
	void (* hello_from_dynamic_lib)();
	hello_from_dynamic_lib = dlsym(lib, "hello_from_dynamic_lib");

	if (!hello_from_dynamic_lib)
	{
		return 2;
	}

	hello_from_dynamic_lib();
	return 0;
}
