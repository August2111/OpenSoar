
import subprocess, os, platform
from build.project import Project

class BoostProject(Project):
    def __init__(self, url, alternative_url, md5, installed, cross_args, configure_args=[],
                 **kwargs):
        Project.__init__(self, url, alternative_url, md5, installed, 'boost', '1.75.0', **kwargs)
        self.cross_args = cross_args
        self.configure_args = configure_args

    def configure(self, toolchain):
        # disable usage of pthreads for Win32 builds
        if 'mingw' in toolchain.actual_arch:
            self.configure_args.append('--enable-pthreads=no')

        return Project.configure(self, toolchain)

    def build(self, toolchain):
        src = None
        ### aug! src = self.unpack(toolchain, out_of_tree=True)
        is_windows_os = platform.system() == 'Windows'
        if src is None:
            src = toolchain.src_path +'\\' + self.base
        olddir = os.getcwd()
        compile_args = []
        if is_windows_os:
            os.chdir(src)
            ### The cwd setting doesnt work ?!? on windows
            if not os.path.isfile(src+'/b2.exe'):
                subprocess.check_call(['bootstrap.bat'], cwd=src, env=toolchain.env) 
            compile_args = ['b2.exe']
        else:
            if not os.path.isfile(src+'/b2'):
                subprocess.check_call(['./bootstrap.sh'], cwd=src, env=toolchain.env) 
            compile_args = ['./b2']
        for arg in self.cross_args:
            compile_args.append(arg)
        compile_args.append('--prefix=' + toolchain.install_prefix)
        compile_args.append('--build-dir=' + toolchain.build_path)
        compile_args.append('install')
        subprocess.check_call(compile_args, cwd=src, env=toolchain.env)
        if is_windows_os:
            os.chdir(olddir)
        print('Boost is ready in "', '?? WHERE ??' ,'"')
        # os.system('pause')
