#include <nanobind/nanobind.h>

int add(int a, int b)
{
	return a + b;
}

NB_MODULE(honesty_ext, m)
{
	m.def("add", &add);
}
