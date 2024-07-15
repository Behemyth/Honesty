export module synodic.honesty.test:suite;

import synodic.honesty.utility;
import synodic.honesty.test.backend;

namespace synodic::honesty::test
{
	// TODO: Make this a compile-time check
	export void VerifySuiteName(const std::string_view name)
	{
		// Empty check
		if (name.empty())
		{
			throw std::runtime_error(std::format(
				"Empty suite name - {}, {}",
				std::source_location::current().file_name(),
				std::source_location::current().line()));
		}

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
				"Duplicate suite name - {}, {}",
				std::source_location::current().file_name(),
				std::source_location::current().line()));
		}

		// Uppercase check
		std::ranges::for_each(
			name,
			[](const char character)
			{
				// Uppercase check
				if (std::isupper(character))
				{
					throw std::runtime_error(std::format(
						"Uppercase suite name - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}

				// Space check
				if (std::isspace(character))
				{
					throw std::runtime_error(std::format(
						"Suite name contains spaces - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}

				// Check for non-alphanumeric characters, excluding '_'
				if (!std::isalnum(character) && character != '_')
				{
					throw std::runtime_error(std::format(
						"Non-alphanumeric suite name - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}
			});
	}

	export template<size_t... NameSizes>
	class SuiteRegistrar
	{
	public:
		explicit SuiteRegistrar(const Suite<NameSizes>&... suites)
		{
			(VerifySuiteName(suites.Name()), ...);
			(AddSuite(suites), ...);
		}
	};
}
