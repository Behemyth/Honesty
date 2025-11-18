export module synodic.honesty.test:suite;

import synodic.honesty.utility;
import synodic.honesty.test.backend;

import std;

namespace honesty::test
{
	void DuplicateCheck(const std::string_view name)
	{
		// Duplicate check
		if (std::ranges::contains(
				GetSuites(),
				name,
				[](const SuiteData& view)
				{
					return view.Name();
				}))
		{
			throw std::runtime_error(std::format(
				"Duplicate suite name '{}' - {}, {}",
				name,
				std::source_location::current().file_name(),
				std::source_location::current().line()));
		}
	}

	export template<size_t... NameSizes>
	class SuiteRegistrar
	{
	public:
		explicit SuiteRegistrar(const Suite<NameSizes>&... suites)
		{
			(DuplicateCheck(suites.Name()), ...);
			(AddSuite(suites), ...);
		}
	};
}
