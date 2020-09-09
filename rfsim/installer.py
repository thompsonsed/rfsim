"""
Compile **librfsim** with default or provided compilation options. Intended for internal usage during ``pip`` or ``conda``
builds, although manual installation is also possible by running this file from the command line.
``python installer.py`` configures the install by detecting system components and compiles the ``C++`` files, if
possible. Command line flags can be provided to installer.py to modify the install (see
:ref:`Compilation Options <sec Compilation Options>` for more information).
"""

from __future__ import print_function, absolute_import, division  # Only Python 2.x

import itertools
import logging
import os
import platform
import re
import shutil
import subprocess
import sys
import numpy
import time
from distutils import sysconfig

# Define the FileNotFound errors for python < 3
try:  # pragma: no cover
    FileExistsError = FileExistsError
    FileNotFoundError = FileNotFoundError
except NameError:  # pragma: no cover

    class FileExistsError(IOError):
        pass

    class FileNotFoundError(IOError):
        pass


from setuptools.command.build_ext import build_ext

mod_directory = os.path.dirname(os.path.abspath(__file__))


class Installer(build_ext):  # pragma: no cover
    """Wraps configuration and compilation of C++ code."""

    def __init__(self, dist, **kwargs):
        """Generates the link to the mod directory for installation."""
        build_ext.__init__(self, dist)
        self.mod_dir = mod_directory
        self.build_dir = None
        self.obj_dir = None

    def get_build_dir(self):
        """
        Gets the build directory.

        :return: the build directory path
        """
        if self.build_dir is None:
            directory = os.path.join(self.mod_dir, "librfsim")
            return directory
        return os.path.join(self.build_dir)

    def get_obj_dir(self):
        """
        Gets the obj directory for installing obj files to.

        :return: the obj directory path
        """
        if self.obj_dir is None:
            return "obj"
        return self.obj_dir

    def get_ldshared(self):
        """Get the ldshared Python variables and replaces -bundle with -shared for proper compilation."""
        ldflags = sysconfig.get_config_var("LDSHARED")
        if ldflags is None:
            return ""
        ldflags = " ".join(ldflags.split()[1:]).replace("-bundle", "-shared")
        return ldflags

    def setuptools_cmake(self, ext):
        """
        Configures cmake for setuptools usage.

        :param ext: the extension to build cmake on
        """
        if "conda" not in sys.version and "Continuum" not in sys.version:
            extdir = os.path.abspath(
                os.path.join(os.path.dirname(self.get_ext_fullpath(ext.name)), "rfsim", "librfsim")
            )
        else:
            sp_dir = os.environ.get("SP_DIR")
            if sp_dir is None:
                sp_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
            extdir = os.path.join(sp_dir, "rfsim", "librfsim")
        cmake_args, build_args = self.get_default_cmake_args(extdir)
        env = os.environ.copy()
        env["CXXFLAGS"] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get("CXXFLAGS", ""), self.distribution.get_version())
        if "INTEL_LICENSE_FILE" in env.keys():
            env["CXX"] = "icpc"
            env["CC"] = "icc"
            cmake_args.extend(["-DCMAKE_C_COMPILER=icc", "-DCMAKE_CXX_COMPILER=icpc", "-DUSING_INTEL=ON"])
        self.run_cmake(ext.sourcedir, cmake_args, build_args, self.build_temp, env)

    def run_cmake(self, src_dir, cmake_args, build_args, tmp_dir, env):
        """
        Runs cmake to compile librfsim.

        :param src_dir: the source directory for librfsim .cpp and .h files
        :param cmake_args: arguments to pass to the cmake project
        :param tmp_dir: the build directory to output cmake files to
        :param env: the os.environ (or other environmental variables) to pass on
        """
        if not os.path.exists(tmp_dir):
            os.makedirs(tmp_dir)
        try:
            subprocess.check_call(["cmake", src_dir] + cmake_args, cwd=tmp_dir, env=env)
            subprocess.check_call(["cmake", "--build", ".", "--target", "rfsim"] + build_args, cwd=tmp_dir, env=env)
        except subprocess.CalledProcessError as cpe:
            raise SystemError("Fatal error running cmake in directory: {}".format(cpe))
        if platform.system() == "Windows":
            shutil.copy(
                os.path.join(tmp_dir, "Release", "rfsim.pyd"), os.path.join(self.get_build_dir(), "librfsim.pyd")
            )

    def run(self):
        """Runs installation and generates the shared object files - entry point for setuptools"""
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        """Builds the C++ and Python extension."""
        self.build_dir = os.path.abspath(
            os.path.join(os.path.dirname(self.get_ext_fullpath(ext.name)), "rfsim", "librfsim")
        )
        self.obj_dir = self.build_dir
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        self.setuptools_cmake(ext)

    def clean_cmake(self):
        """Deletes the cmake files and object locations if they exist."""
        for path in [
            os.path.join(self.get_build_dir(), "librfsim.so"),
            os.path.join(self.get_build_dir(), "librfsim.so.dSYM"),
            os.path.join(self.get_build_dir(), "librfsim.pyd"),
            os.path.join(self.get_build_dir(), "librfsim.dylib"),
            os.path.join(self.mod_dir, "obj"),
        ]:
            if os.path.exists(path):
                if os.path.isdir(path):
                    shutil.rmtree(path)
                else:
                    os.remove(path)

    def get_default_cmake_args(self, output_dir):
        """
        Returns the default cmake configure and build arguments.

        :param output_dir: the output directory to use

        :return: tuple of two lists, first containing cmake configure arguments, second containing build arguments
        :rtype: tuple
        """
        cfg = "Debug" if self.debug else "Release"
        cflags = sysconfig.get_config_var("CFLAGS")
        if cflags is not None:
            cflags = str(re.sub(r"-arch \b[^ ]*", "", cflags)).replace("\n", "")  # remove any architecture flags
        else:
            cflags = ""
        gdal_inc_path = None
        gdal_dir = None
        if platform.system() == "Windows":
            libdir = get_python_library("{}.{}".format(sys.version_info.major, sys.version_info.minor))
        else:
            conda_prefix = os.environ.get("PREFIX", None)  # for conda only under unix
            libdir = sysconfig.get_config_var("LIBDIR")
            if conda_prefix is not None:
                gdal_inc_path = os.path.join(conda_prefix, "include")
                gdal_dir = os.path.join(conda_prefix, "lib")
            else:
                try:
                    gdal_inc_path = subprocess.check_output(["gdal-config", "--cflags"], env=os.environ)
                    gdal_dir = subprocess.check_output(["gdal-config", "--prefix"], env=os.environ)
                except subprocess.CalledProcessError:
                    pass
                if gdal_inc_path is not None:
                    gdal_inc_path = gdal_inc_path.decode("utf-8").split(" ")[0][2:].replace("\n", "")
                if gdal_dir is not None:
                    gdal_dir = gdal_dir.decode("utf-8").replace("\n", "")
        if libdir is None:
            libdir = os.path.abspath(os.path.join(sysconfig.get_config_var("LIBDEST"), "..", "libs"))
            if sysconfig.get_config_var("LIBDEST") is None:
                raise SystemError("Cannot detect library directory for Python install.")
        cmake_args = [
            "-DPYTHON_LIBRARY:FILEPATH={}".format(libdir),
            "-DPYTHON_CPPFLAGS:='{}'".format(cflags),
            "-DPYTHON_LDFLAGS:='{}'".format(self.get_ldshared()),
            "-DPYTHON_INCLUDE_DIR:FILEPATH={}".format(sysconfig.get_python_inc()),
            "-DNUMPY_INCLUDE_DIR:='{}'".format(numpy.get_include()),
            "-DCMAKE_BUILD_TYPE={}".format(cfg),
        ]
        if gdal_inc_path is not None:
            cmake_args.append("-DGDAL_INCLUDE_DIR={}".format(gdal_inc_path))
        if gdal_dir is not None:
            cmake_args.append("-DGDAL_DIR={}".format(gdal_dir))
        build_args = ["--config", cfg]
        if platform.system() == "Windows":
            cmake_args += [
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH={}".format(
                    # cfg.upper(),
                    output_dir
                )
            ]
            if sys.maxsize > 2 ** 32:
                cmake_args += ["-A", "x64"]
            build_args += ["--", "/m"]
        else:

            cmake_args += ["-DCMAKE_BUILD_TYPE=" + cfg, "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH={}".format(output_dir)]
            build_args += ["--", "-j2"]
        return cmake_args, build_args


def get_python_library(python_version):  # pragma: no cover
    """Get path to the Python library associated with the current Python
    interpreter."""
    # determine direct path to libpython
    python_library = sysconfig.get_config_var("LIBRARY")
    potential_library = None
    # if static (or nonexistent), try to find a suitable dynamic libpython
    if python_library is None or python_library[-2:] == ".a":

        candidate_lib_prefixes = ["", "lib"]

        candidate_extensions = [".lib", ".so"]
        if sysconfig.get_config_var("WITH_DYLD"):
            candidate_extensions.insert(0, ".dylib")

        candidate_versions = [python_version]
        if python_version:
            candidate_versions.append("")
            candidate_versions.insert(0, "".join(python_version.split(".")[:2]))

        abiflags = getattr(sys, "abiflags", "")
        candidate_abiflags = [abiflags]
        if abiflags:
            candidate_abiflags.append("")

        # Ensure the value injected by virtualenv is
        # returned on windows.
        # Because calling `sysconfig.get_config_var('multiarchsubdir')`
        # returns an empty string on Linux, `du_sysconfig` is only used to
        # get the value of `LIBDIR`.
        libdir = sysconfig.get_config_var("LIBDIR")
        if sysconfig.get_config_var("MULTIARCH"):
            masd = sysconfig.get_config_var("multiarchsubdir")
            if masd:
                if masd.startswith(os.sep):
                    masd = masd[len(os.sep) :]
                libdir = os.path.join(libdir, masd)

        if libdir is None:
            libdir = os.path.abspath(os.path.join(sysconfig.get_config_var("LIBDEST"), "..", "libs"))

        candidates = (
            os.path.join(libdir, "".join((pre, "python", ver, abi, ext)))
            for (pre, ext, ver, abi) in itertools.product(
                candidate_lib_prefixes, candidate_extensions, candidate_versions, candidate_abiflags
            )
        )
        for candidate in candidates:
            if os.path.exists(candidate):
                # we found a (likely alternate) libpython
                potential_library = candidate
                if potential_library[-2:] != ".a":
                    break
    # Otherwise still a static library, keep searching
    if potential_library is None:
        raise IOError("No Python libraries found")
    return potential_library


if __name__ == "__main__":  # pragma: no cover
    fail = True
    from distutils.dist import Distribution
    import argparse

    parser = argparse.ArgumentParser(description="Build the C++ library (librfsim) required for rfsim.")
    parser.add_argument("--cmake", action="store_true", default=True, dest="cmake", help="use the cmake build process")
    parser.add_argument(
        "--autotools",
        action="store_true",
        default=False,
        dest="autotools",
        help="Use the autotools build process (./configure and make)",
    )
    parser.add_argument(
        "--compiler-args",
        metavar="N",
        type=str,
        nargs="+",
        dest="compiler_args",
        default=[],
        help="Additional arguments to pass to the autotools compiler",
    )
    parser.add_argument(
        "--cmake-args",
        metavar="N",
        type=str,
        nargs="+",
        dest="cmake_args",
        default=[],
        help="Additional arguments to pass to the cmake compiler during configuration",
    )
    parser.add_argument(
        "--cmake-build-args",
        metavar="N",
        type=str,
        nargs="+",
        dest="cmake_build_args",
        default=[],
        help="Additional arguments to pass to the cmake compiler at build time",
    )
    parser.add_argument("--debug", action="store_true", default=False, dest="debug", help="Compile using DEBUG defines")
    parser.add_argument(
        "-c",
        "-C",
        "--compile",
        action="store_true",
        default=False,
        dest="compile_only",
        help="Compile only, do not re-configure librfsim",
    )
    parser.add_argument(
        "--clean",
        action="store_true",
        default=False,
        dest="clean",
        help="Clean previous cmake builds from this directory.",
    )

    args = parser.parse_args()
    if args.cmake and args.autotools:
        raise ValueError("Cannot use both cmake and autotools build process - specify one or the other.")
    if not args.cmake or args.autotools:
        raise ValueError("Must specify compilation either using autotools or cmake.")
    dist = Distribution()
    installer = Installer(dist)
    installer.debug = args.debug
    if args.clean:
        installer.clean_cmake()
    else:
        if args.cmake:
            env = os.environ.copy()
            build_dir = installer.get_build_dir()
            obj_dir = installer.get_obj_dir()
            cmake_args, build_args = installer.get_default_cmake_args(build_dir)
            cmake_args += args.cmake_args
            build_args += args.cmake_build_args
            src_dir = os.path.join(installer.mod_dir, "lib")
            installer.run_cmake(src_dir, cmake_args, build_args, obj_dir, env)
        else:
            if platform.system() != "Windows":
                raise SystemError("Usage of configure and make on a windows system is not supported.")
            if args.compile_only:
                installer.copy_makefile()
                installer.do_compile()
                fail = False
            if fail:
                installer.configure_and_compile(args.compiler_args)
