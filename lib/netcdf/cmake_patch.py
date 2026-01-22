# CMake Patch for project NETCDF_C (netcdf)
import os, sys, shutil, subprocess

if len(sys.argv) < 1:
    print('No patch: ', sys.argv)
    exit(1)


patch_dir = os.path.dirname(sys.argv[0])
print('Patch-Dir: ', patch_dir, ' -> ' , os.getcwd(), ' =========================')
my_env = os.environ.copy()

if False:
   # print('patch with ');
   myprocess = subprocess.Popen(['python', '--version'], env = my_env)
   myprocess.wait()

try:
   file = open('patch_active', 'r')
   print('patch is active!')
except IOError:
   myprocess = subprocess.Popen(['git', 'apply', patch_dir + '/patches/fix_dutil_4_7_4.patch'], env = my_env)
   myprocess.wait()
   file = open('patch_active', 'w')

exit(0)

