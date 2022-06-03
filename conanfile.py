from conans import ConanFile, CMake


class SeleneConan(ConanFile):
    name = "selene"
    version = "0.3.1"
    license = "MIT"
    url = "https://github.com/kmhofmann/selene"
    description = "A C++17 image representation, processing and I/O library."

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    build_policy = "missing"

    requires = ("libjpeg-turbo/[>=1.5.0]",
                "libpng/[>=1.2.0]",
                "libtiff/4.3.0@bsw/stable")

    exports_sources = ("../../selene*",
                       "../../cmake*",
                       "../../CMakeLists.txt",
                       "../../conanfile.txt",
                       "../../LICENSE")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["WITH_CONAN"] = "ON"
        cmake.definitions['CMAKE_POSITION_INDEPENDENT_CODE'] = "True"
        cmake.configure(source_folder="")
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("license*", dst="licenses",  ignore_case=True, keep_path=False)
        self.copy("*.so", dst="lib", src="package/lib", keep_path=False)
        self.copy("*.a", dst="lib", src="package/lib", keep_path=False)
        self.copy("*.hpp", dst="include", src="package/include", keep_path=True)

    def package_info(self):
        self.cpp_info.libs = ["selene_base",
                              "selene_base_io",
                              "selene_img",
                              "selene_img_io",
                              "selene_img_io_jpeg",
                              "selene_img_io_png",
                              "selene_img_io_tiff",
                              "selene_img_ops",
                              "selene"]
