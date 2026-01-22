# CMake Patch for project SODIUM (sodium, libsodium)

import os, sys, subprocess, shutil

if len(sys.argv) < 2:
    print('No patch: ', sys.argv)
    exit(1)

patch_dir = os.path.dirname(sys.argv[0])
lib_name = sys.argv[1]

if not os.path.exists(patch_dir +'/' + lib_name):
    print('No patchdir exists for: ', lib_name)
    exit(1)

patch_dir += '/' + lib_name
print('Patch-Dir: ', patch_dir, ' -> ' , os.getcwd(), ' =========================')

cmake_lists_in = patch_dir + '/cmake/' +lib_name+'-CMakeLists.txt.in'
if os.path.exists(cmake_lists_in):
    # configure CMakeLists.txt:
    print('configure CMakeLists.txt from: ', lib_name)
    

my_env = os.environ.copy()

patch_detect = 'patch_active'
do_patch = not os.path.exists(patch_detect)

if not do_patch: # and False:
    os.remove(patch_detect)
    myprocess = subprocess.Popen(['git', 'reset', '--hard', 'HEAD'], env = my_env)
    myprocess.wait()
    do_patch = True

patch_dir += '/patches/'
# print('configure CMakeLists.txt from: ', lib_name, ' /// ' + cmake_lists_in)
try:
   file = open(patch_detect, 'r')
   print('patch is active')
except IOError:
   with open(patch_dir +'series') as file:
    for line in file:
        patch = line.rstrip().split('#')[0]
        if patch and len(patch) > 0:
            patch_file = patch_dir + patch
            if os.path.exists(patch_file):
                print(patch_file)
                myprocess = subprocess.Popen(['git', 'apply', '--verbose', patch_file], env = my_env)
                myprocess.wait()
            else:
                print(patch_file, 'doesn\'t exist')
   file = open(patch_detect, 'w')

### with open(patch_dir +'series') as file:
###     for line in file:
###         patch = line.rstrip().split('#')[0]
###         if patch and len(patch) > 0:
###             patch_file = patch_dir + patch
###             if os.path.exists(patch_file):
###                 print(patch_file)
###             else:
###                 print(patch_file, 'doesn\'t exist')
###         else:
###             print('no patch: ',line.rstrip())

if False:
    try:
        file = open('patch_active', 'r')
        print('patch is active')
    except IOError:
        print('patch is to do')
        # myprocess = subprocess.Popen(['git', 'apply', patch_dir +'/patches/disable_db.patch'], env = my_env)
        # myprocess.wait()
###try:
###   src=patch_dir + '/cmake/sodium_CMakeLists.txt.in'
###   if os.path.exists(src):
###        dst='CMakeLists.txt'
###        shutil.copy(src, dst)
###        print('Copy: ', src, ' -> ', dst, ' successful')
###        if os.path.exists(dst):
###            file = open('patch_active', 'w')
###        # test only: shutil.copy(src, '_CMakeLists.txt')
###   else:
###        print('Copy: ', src, ' not found!')
###except:
###   print('Copy error: ', src, ' -> ', dst)
###   exit(1)

exit(1)

