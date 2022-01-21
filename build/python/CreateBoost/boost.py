
import subprocess
from build.project import Project

class BoostProject(Project):
    def __init__(self, url, alternative_url, md5, installed, configure_args=[],
                 **kwargs):
        Project.__init__(self, url, alternative_url, md5, installed, 'boost', '1.75.0', **kwargs)
        self.configure_args = configure_args

    def configure(self, toolchain):
        # disable usage of pthreads for Win32 builds
        if 'mingw' in toolchain.actual_arch:
            self.configure_args.append('--enable-pthreads=no')

        return Project.configure(self, toolchain)
    def build(self, toolchain):
        src = self.unpack(toolchain, out_of_tree=False)
        olddir = os.getcwd()
        os.chdir(src)
        print(platform.system(), ': ', os.getcwd(), "vs. ", olddir)
        if platform.system() == 'Windows':
            if not os.isfile('b2.exe'):
                subprocess.check_call(['bootstrap'],
                              cwd=src, env=toolchain.env)

            subprocess.check_call(['b2', cross_args],
                              cwd=src, env=toolchain.env)
        else:
            subprocess.check_call(['./configure', '--prefix=' + toolchain.install_prefix, '--static'],
                              cwd=src, env=toolchain.env)
        MakeProject.build(self, toolchain, src)
        os.chdir(olddir)
