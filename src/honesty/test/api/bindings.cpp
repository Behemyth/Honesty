#include <nanobind/nanobind.h>

int add(int a, int b)
{
	return a + b;
}

NB_MODULE(honesty_bindings, m)
{
	m.def("add", &add);
}
