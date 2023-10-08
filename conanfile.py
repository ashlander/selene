from os.path import join
from os import sep
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import copy, rm, replace_in_file
from conan.tools.scm import Git

class SeleneConan(ConanFile):
    name = "selene"
    version = "0.3.1"
    license = "MIT"
    url = "https://github.com/kmhofmann/selene"
    description = "A C++17 image representation, processing and I/O library."

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared":False}

    def requirements(self):
        self.requires("libjpeg-turbo/[>=1.5.0]")
        self.requires("libpng/[>=1.2.0]")
        self.requires("libtiff/4.5.1@bsw/stable")

    def generate(self):
        #apply patch
        self.patch()

        tc = CMakeToolchain(self)
        tc.cache_variables["WITH_CONAN"] = "ON"
        tc.cache_variables["CMAKE_POSITION_INDEPENDENT_CODE"] = "True"
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()

    def patch_file(self, path):
        replace_in_file(self, path, "uint32* ", "uint32_t* ", strict=False)
        replace_in_file(self, path, "uint32& ", "uint32_t& ", strict=False)
        replace_in_file(self, path, "uint32 ", "uint32_t ", strict=False)
        replace_in_file(self, path, "<uint32>", "<uint32_t>", strict=False)
        replace_in_file(self, path, "<uint32*>", "<uint32_t*>", strict=False)
        replace_in_file(self, path, "uint32{", "uint32_t{", strict=False)
        replace_in_file(self, path, "uint32(", "uint32_t(", strict=False)
        replace_in_file(self, path, "int32& ", "int32_t& ", strict=False)
        replace_in_file(self, path, "int32 ", "int32_t ", strict=False)
        replace_in_file(self, path, "<int32>", "<int32_t>", strict=False)
        replace_in_file(self, path, "int32{", "int32_t{", strict=False)
        replace_in_file(self, path, "int32)", "int32_t)", strict=False)
        replace_in_file(self, path, "uint16& ", "uint16_t& ", strict=False)
        replace_in_file(self, path, "uint16 ", "uint16_t ", strict=False)
        replace_in_file(self, path, "<uint16>", "<uint16_t>", strict=False)
        replace_in_file(self, path, "<uint16,", "<uint16_t,", strict=False)
        replace_in_file(self, path, "uint16{", "uint16_t{", strict=False)
        replace_in_file(self, path, "uint16)", "uint16_t)", strict=False)
        replace_in_file(self, path, "int16& ", "int16_t& ", strict=False)
        replace_in_file(self, path, "int16 ", "int16_t ", strict=False)
        replace_in_file(self, path, "<int16>", "<int16_t>", strict=False)
        replace_in_file(self, path, "int16{", "int16_t{", strict=False)
        replace_in_file(self, path, "int16)", "int16_t)", strict=False)

    def patch(self):
        # apply patch
        path = join(self.source_folder, "selene", "img_io", "tiff", "_impl", "TIFFDetail.hpp")
        self.patch_file(path)

        path = join(self.source_folder, "selene", "img_io", "tiff", "_impl", "TIFFDetail.cpp")
        self.patch_file(path)

        path = join(self.source_folder, "selene", "img_io", "tiff", "_impl", "TIFFReadTiles.cpp")
        self.patch_file(path)

        path = join(self.source_folder, "selene", "img_io", "tiff", "_impl", "TIFFReadHighLevel.cpp")
        self.patch_file(path)

        path = join(self.source_folder, "selene", "img_io", "tiff", "_impl", "TIFFReadStrips.cpp")
        self.patch_file(path)

        path = join(self.source_folder, "selene", "img_io", "tiff", "Read.cpp")
        self.patch_file(path)

        path = join(self.source_folder, "selene", "img_io", "tiff", "Write.cpp")
        self.patch_file(path)

    def source(self):
        rm(self, "*", self.source_folder, recursive=True) # remove leftovers
        git = Git(self)
        git.clone(url="https://github.com/ashlander/selene.git", target=self.source_folder)
        git.folder = self.source_folder 
        git.checkout("clang14")

    def package(self):
        copy(self, "license*", self.source_folder, join(self.package_folder, "licenses"),  ignore_case=True, keep_path=False)
        copy(self, "*.so", self.source_folder, join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.a", self.source_folder, join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.hpp", self.source_folder, join(self.package_folder, "include"), keep_path=True)

    def package_info(self):
        self.cpp_info.libs = ["selene_base",
                              "selene_base_io",
                              "selene_img",
                              "selene_img_io",
                              "selene_img_io_jpeg",
                              "selene_img_io_png",
                              "selene_img_io_tiff",
                              "selene_img_ops"]
