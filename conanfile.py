from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class AutoPackage(ConanFile):
	name = "honesty"
	version = "1.0"
	settings = "os", "compiler", "build_type", "arch"
	requires = ["opentelemetry-cpp/[>=1.21.0]"]

	def layout(self):
		cmake_layout(self)

	def generate(self):
		deps = CMakeDeps(self)
		deps.generate()
		tc = CMakeToolchain(self)
		tc.user_presets_path = None
		tc.variables["BUILD_TESTING"] = "OFF"
		tc.generate()

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		cmake = CMake(self)
		cmake.install()

	def package_info(self):
		self.cpp_info.set_property("cmake_file_name", "honesty")

		self.cpp_info.components["honesty"].libs = ["honesty"]
		self.cpp_info.components["honesty"].set_property("cmake_target_name", "synodic::honesty")

		self.cpp_info.components["main"].libs = ["honesty_main"]
		self.cpp_info.components["main"].set_property("cmake_target_name", "synodic::honesty::main")
		self.cpp_info.components["main"].requires = ["honesty"]

	def export_sources(self):
		copy(self, "CMakeLists.txt", src=self.recipe_folder, dst=self.export_sources_folder)
		copy(self, "src/*", src=self.recipe_folder, dst=self.export_sources_folder)
		copy(self, "cmake/*", src=self.recipe_folder, dst=self.export_sources_folder)
