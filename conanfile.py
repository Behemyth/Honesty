
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class MyProject(ConanFile):
    name = "myproject"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = ["opentelemetry-cpp/[>=1.18.0]"]
    generators = "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
