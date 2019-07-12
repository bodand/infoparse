from conans import ConanFile, CMake


class InfoparseConan(ConanFile):
    name = "InfoParse"
    version = "1.3"
    license = "BSD 3-Clause"
    author = "bodand bodand@pm.me"
    url = "https://github.com/isbodand/"
    description = "C++17 Parameter parser like Perl's Getopt::Long"
    topics = ("parser", "getopt-long", "c++17")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=True"
    exports = "*"
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/isbodand/infoparse.git")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="infoparse")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include", src="infoparse")
        self.copy("*infoparse.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["infoparse"]
