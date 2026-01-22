# CMake Patch for project PROJ (proj)
import os, sys, subprocess

if len(sys.argv) < 1:
    print('No patch: ', sys.argv)
    exit(1)

patch_dir = os.path.dirname(sys.argv[0])
print('Patch-Dir: ', patch_dir, ' -> ' , os.getcwd(), ' =========================')

my_env = os.environ.copy()

patch_detect = 'patch_active'
do_patch = not os.path.exists(patch_detect)

if not do_patch: # and False:
    os.remove(patch_detect)
    myprocess = subprocess.Popen(['git', 'reset', '--hard', 'HEAD'], env = my_env)
    myprocess.wait()
    do_patch = True

try:
   file = open(patch_detect, 'r')
   print('patch is active')
except IOError:
   with open(patch_dir +'/patches/series') as file:
    for line in file:
        patch = line.rstrip().split('#')[0]
        if patch and len(patch) > 0:
            patch_file = patch_dir +'/patches/' + patch
            if os.path.exists(patch_file):
                print(patch_file)
                myprocess = subprocess.Popen(['git', 'apply', '--verbose', patch_file], env = my_env)
                myprocess.wait()
            else:
                print(patch_file, 'doesn\'t exist')
   file = open(patch_detect, 'w')

exit(0)

