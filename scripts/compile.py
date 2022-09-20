import os
import subprocess
from sys import platform
from typing import Iterable
from distutils.dir_util import copy_tree
from abc import ABC, abstractmethod
import shutil

FILE_DIR = os.path.abspath(os.path.dirname(__file__))
ROOT_DIR = os.path.abspath(os.path.join(FILE_DIR, '..'))

MAC_OS = 'darwin'


def Triplet() -> str:
    if platform == MAC_OS:
        return 'x64-osx'
    else:
        return ''


class VcpkgInstaller:
    def __init__(self, path: str, library_list: Iterable):
        self.path = path
        self.library_list = " ".join(library_list)

        # install vcpkg libraries
        os.chdir(path=self.path)

        subprocess.call('./{}'.format(self._bootstrap()), shell=True)

        install_cmd = "./vcpkg install --triplet {} {}".format(
            Triplet(), self.library_list)
        subprocess.call(install_cmd, shell=True)

    def _bootstrap(self):
        if platform == MAC_OS:
            return 'bootstrap-vcpkg.sh'
        else:
            return 'bootstrap-vcpkg.bat'

    def generate_include_dir(self) -> str:
        return "{}/installed/{}/include".format(self.path, Triplet())

    def generate_lib_dir(self) -> str:
        return "{}/installed/{}/lib".format(self.path, Triplet())


def CopyLibraries(src: str, dst: str) -> None:
    if not os.path.exists(dst):
        os.mkdir(dst)
    copy_tree(src=src, dst=dst)


class CMakeBuildBase(ABC):
    def __init__(self, root_dir: str) -> None:
        super().__init__()
        self.root_dir = root_dir

        # remove build folder if exists
        if os.path.exists(self.build_dir()):
            shutil.rmtree(self.build_dir())

        os.mkdir(self.build_dir())

        self.run()

    def run(self):
        os.chdir(self.build_dir())
        cmake_cmd = 'cmake -DVCPKG_TARGET_TRIPLET={} -DCMAKE_TOOLCHAIN_FILE={}  -DCMAKE_BUILD_TYPE={} ..'.format(
            Triplet(), self.toolchain_path(), self.build_type())
        print(cmake_cmd)
        subprocess.call(cmake_cmd, shell=True)

        self._run_build()

        os.chdir(self.root_dir)

    def build_dir(self):
        return "{}/build".format(self.root_dir)

    def _run_build(self):
        if platform == MAC_OS:
            build_cmd = 'make -j8'
            subprocess.call(build_cmd, shell=True)

    @abstractmethod
    def build_type(self) -> str:
        return ''

    def toolchain_path(self) -> str:
        return'{}/vcpkg/scripts/buildsystems/vcpkg.cmake'.format(
            self.root_dir)


class CMakeBuildDebug(CMakeBuildBase):
    def __init__(self, root_dir: str) -> None:
        super().__init__(root_dir)

    def build_type(self) -> str:
        return 'Debug'


class CMakeBuildRelease(CMakeBuildBase):
    def __init__(self, root_dir: str) -> None:
        super().__init__(root_dir)

    def build_type(self) -> str:
        return 'RelWithDebInfo'


def main():
    vcpkg_path = "{}/vcpkg".format(ROOT_DIR)
    vcpkg_libs = ['boost-asio',
                  'beast',
                  'openssl',
                  'boost-regex',
                  'boost-iostreams',
                  'boost-multi-index',
                  'boost-json',
                  'boost-timer',
                  'boost-regex',
                  "boost-program-options"]

    VcpkgInstaller(vcpkg_path, vcpkg_libs)

    CMakeBuildDebug(ROOT_DIR)


if __name__ == '__main__':
    main()
