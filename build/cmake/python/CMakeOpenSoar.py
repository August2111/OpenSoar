#!/usr/bin/env python3

import os, sys, subprocess

# 
Configuration = 'Release'
# Configuration = 'Debug'
# Configuration = 'Multi'


# if len(sys.argv) > 1:
#   print(sys.argv[1])
#   os.chdir(sys.argv[1])              ## batch: cd /D %~dp0
#   print(os.getcwd())
# else:
#   print('No Parameter!')

# my_env = []  # global!
# my_env = os.environ.copy()

prev_batch = None
cmake_generator = None
program_dir = None
is_windows = False
build_system = 'unknown'
toolchain_file = None

def gcc(toolchain, env):
  global cmake_generator
  global prev_batch
  global program_dir
  global toolchain_file
  src_dir = os.getcwd()  
  if sys.platform.startswith('win'):
      if toolchain == 'mingw' or toolchain.startswith('mgw'):
         toolchain_file = src_dir.replace('\\','/') + '/build/cmake/toolchains/MinGW.toolchain'
      # cmake_generator = '\"MinGW Makefiles\"'
      print('src_dir = ',src_dir)
      
      cmake_generator = 'MinGW Makefiles'
      return program_dir.replace('/', '\\') + '\\MinGW\\' + toolchain + '\\bin;' + env['PATH']
  else:
     # cmake_generator ='Unix Makefiles'  # normal Unix
     cmake_generator ='Ninja'
     env_path = env['PATH']
  return env_path

def clang(toolchain, env):
  global cmake_generator
  global toolchain_file

  env_path = env['PATH']
  if sys.platform.startswith('win'):
     # env_path = program_dir.replace('/', '\\') + '\\MinGW\\mgw122;' + env_path
     # env_path = program_dir.replace('/', '\\') + '\\MinGW\\mgw122\\include;' + env_path
     if toolchain == 'clangXX':
        env_path = program_dir.replace('/', '\\') + '\\LLVM\\' + toolchain + '-llvm\\bin;' + env_path['PATH']
        #Android-Clang!
        #     env_path = program_dir.replace('/', '\\') + '\\LLVM\\clang14-android\\bin;' + env_path
        #     env_path = program_dir.replace('/', '\\') + '\\LLVM\\llvm\\bin;' + env_path
        ### Android: env_path = program_dir + '/Android/android-ndk-r25b/toolchains/llvm/prebuilt/windows-x86_64/bin;' + env_path
        # toolchain_file = src_dir.replace('\\','/') + '/build/cmake/toolchains/MinGW.toolchain'
     elif toolchain == 'clang12': 
        # env_path = program_dir.replace('/', '\\') + '\\MinGW\\mgw112\\bin;' + env_path
        env_path = program_dir.replace('/', '\\') + '\\LLVM\\' + toolchain + '\\bin;' + env_path  # ['PATH']
     else: 
        env_path = program_dir.replace('/', '\\') + '\\LLVM\\' + toolchain + '\\bin;' + env_path  # ['PATH']
        # env_path = program_dir.replace('/', '\\') + '\\MinGW\\mgw112\\bin;' + env_path
     # toolchain_file = os.getcwd().replace('\\','/') + '/build/cmake/toolchains/Android/x86_64.toolchain'
     toolchain_file = os.getcwd().replace('\\','/') + '/build/cmake/toolchains/WinClang.toolchain'
  else:
     env_path = env['PATH']
      # cmake_generator ='Unix Makefiles'
  cmake_generator ='Ninja'
  return env_path

generator = {
           'mgw73' : 'MinGW Makefiles',
           'mgw82' : 'MinGW Makefiles',
           'mgw103' : 'MinGW Makefiles',
           'mgw112' : 'MinGW Makefiles',
           'mgw122' : 'MinGW Makefiles',
           # 'ninja' : 'MinGW Makefiles',
           'ninja' : 'Ninja',
           'unix' : 'Unix Makefiles',
           'mingw' : 'MinGW Makefiles',
           'clang10' : 'Clang',
           'clang11' : 'Clang',
           'clang12' : 'Clang',
           'clang13' : 'Clang',
           'clang14' : 'Clang',
           'clang15' : 'Clang',
           'clang16' : 'Clang',
           'clang17' : 'Clang',
           'msvc2015' : 'Visual Studio 14',
           'msvc2017' : 'Visual Studio 15',
           'msvc2019' : 'Visual Studio 16',
           'msvc2022' : 'Visual Studio 17',
}

def visual_studio(toolchain, env):
  global prev_batch, cmake_generator
  #prev_batch = 'C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Auxiliary/Build/vcvars64.bat'
  if toolchain == 'msvc2022':
    prev_batch = program_dir + '/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/vcvars64.bat'
  else:
    print('wrong toolchain: ', toolchain, '!')
    exit(1)
  cmake_generator = generator[toolchain]
  # cmake_generator ='Visual Studio 16'
  return env['PATH']

  # map the inputs to the function blocks
compiler_setup = {
           'mgw73' : gcc,
           'mgw82' : gcc,
           'mgw103' : gcc,
           'mgw112' : gcc,
           'mgw122' : gcc,
           # 'ninja' : gcc,
           'ninja' : clang,
           'unix' : gcc,
           'mingw' : gcc,
           'clang10' : clang,
           'clang11' : clang,
           'clang12' : clang,
           'clang13' : clang,
           'clang14' : clang,
           'clang15' : clang,
           'clang16' : clang,
           'clang17' : clang,
           'msvc2015' : visual_studio,
           'msvc2017' : visual_studio,
           'msvc2019' : visual_studio,
           'msvc2022' : visual_studio,
}

def create_opensoar(args):
  #Current directory:
  global program_dir
  global is_windows
  is_windows = False
  
  filename = sys.argv[0]
  project_name = args[0]
  branch = args[1]
  toolchain = args[2]

  start_dir = os.path.dirname(filename)
  if len(start_dir) > 0:
      start_dir = start_dir.replace('build/cmake/python', '');
  if len(start_dir) == 0:
     # start_dir = ''
     start_dir = os.getcwd();
  # print('Filename: ', filename)
  # print('Start CMake Creation of ', project_name, ' / ', branch, ' / ', toolchain)
  print('Start CMake Creation of ', project_name, ' / ', branch, ' / ', toolchain)
  print('====================================\n')
  print('CurrDir  :',os.getcwd())
  print('StartDir :',start_dir)
#  os.chdir(start_dir)
#  print('CurrDir  :',os.getcwd())
  
  my_env = os.environ.copy()
  creation = 15
  if len(sys.argv) > 3:
    creation = int(sys.argv[3])
  # creation = 14  # ohne CMake!
  verbose = creation & 0x100
  print('creation-flag = ', str(creation)) # ohne CMake!
  ### verbose = True
  if True: # False: # 
    i = 0
    for arg in args:   # sys.argv:
      print(i, ': ', arg)
      i = i + 1
    if verbose:
      print('Debug-Stop')
      input("Press Enter to continue...")

  if sys.platform.startswith('win'):
    is_windows = True
    project_dir = 'D:/Projects'
    program_dir = 'D:/Programs'
    # not necessary ?! install_bindir = 'bin'
    # src_dir = start_dir  # changed 05.12.2022
	# ATTENTION: this isn't finished yet!!!
    src_dir = start_dir  # changed 05.12.2022
    if branch:
       binary_dir= project_dir + '/Binaries/OpenSoar/' + branch  # changed 10.01.2023
    else:
       binary_dir= project_dir + '/Binaries/OpenSoar/build'  # changed 10.01.2023
    link_libs = project_dir + '/link_libs'  # Windows on August2111/Flaps6!!!
    # build_dir = binary_dir + '/'+ project_name + '/' + branch + '/' + toolchain
    build_dir = binary_dir + '/'+ toolchain

    
    # TODO(August2111): delete: third_party = binary_dir + 'D:/Projects/3rd_Party'  # Windows!
    third_party = binary_dir + '/3rd_Party'  # Windows!
    install_dir = program_dir + '/Install/' + project_name
  else:
    src_dir = start_dir
    root_dir = my_env['HOME']
    project_dir = root_dir + '/Projects'
    # program_dir = '/usr/bin'
    # program_dir = '/usr/local/bin'
    program_dir = root_dir + '/Programs'
    # binary_dir = project_dir + '/Binaries'
    binary_dir= start_dir + '/_build'  # new from 02.02.2021, simular to OpenSoar upstream
    # build_dir = binary_dir + '/'+ project_name+ '/' + toolchain
    build_dir = binary_dir + '/'+ toolchain
    link_libs = project_dir + '/link_libs'
    # third_party = project_dir + '/3rd_Party'
    third_party = binary_dir + '/3rd_Party'  # Windows!
    install_dir = program_dir + '/Install/' + project_name

  toolset = None

  python_exe = ''
  try:
    myprocess = subprocess.Popen(['python', '--version'], env = my_env)
    myprocess.wait()
    python_exe = 'python'
  except:
    print('"python" not callable')
  try:
    myprocess = subprocess.Popen(['python3', '--version'], env = my_env)
    myprocess.wait()
    python_exe = 'python3'
  except:
    print('"python3" not callable')

  if sys.platform.startswith('win'):
    cmake_exe = (program_dir  + '/CMake/bin/') + 'cmake.exe'
    my_env['PATH'] =  (program_dir  + '/CMake/bin;').replace('/', '\\') + my_env['PATH']
  else:
    cmake_exe = 'cmake'

  # wget https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2.tar.gz
  
  try:
    myprocess = subprocess.Popen([cmake_exe, '--version'], env = my_env)
    myprocess.wait()
  except:
    print('"cmake" not callable!')
    creation = 0

  if not os.path.exists(build_dir):
    os.makedirs(build_dir)
    creation = creation | 1

  my_env['PATH'] = compiler_setup[toolchain](toolchain, my_env)
  print(my_env['PATH'])

  #  compiler_setup[args[1]](args[1], my_env['PATH'])

  print('Creation-Flag: ', creation)
  if prev_batch:
    print(prev_batch)
    
  if build_dir.endswith('msvc2022'):
    if Configuration == 'Release':
      build_dir = build_dir + 'release'
    elif Configuration == 'Debug':
      build_dir = build_dir + 'debug'
  #========================================================================
  if creation & 1:
    print('Python Step 1 - Configure CMake')
    if os.path.isfile(build_dir+ '/CMakeCache.txt'):
      os.remove(build_dir+ '/CMakeCache.txt')
    arguments = []
    # if prev_batch:
       # arguments.append(prev_batch)
       # arguments.append(' & ')
    arguments.append(cmake_exe)
    # if toolchain in ['clang10']:                                          ## Clang!
    #   my_env['PATH'] = program_dir + '/llvm/bin;'          ## Clang!
    #   my_env['PATH'] = my_env['PATH'] + program_dir + '/CMake/bin;'      ## Clang!
    #   arguments.append(cmake_exe)  # 'cmake')
    #   arguments.append('-H.')  ## Clang!
    arguments.append('-S')
    arguments.append(src_dir)  # curr_dir.replace('\\', '/'))
    arguments.append('-B')
    arguments.append(build_dir)
    arguments.append('-G') 
    arguments.append(cmake_generator) 
#    arguments.append('-DCMAKE_BUILD_TYPE=Release')

    if Configuration == 'Release' or Configuration == 'Debug':
      arguments.append('-DCMAKE_BUILD_TYPE=' + Configuration)
      arguments.append('--debug-trycompile')

    if is_windows and toolchain.startswith('clang'):
      compiler = toolchain
    if is_windows and toolchain == 'ninja':
      toolchain = 'clang17'
      compiler = toolchain

    print('---')
    if is_windows:
      print('!!! COMPUTERNAME = ', my_env['COMPUTERNAME'],  ', USERNAME = ', my_env['USERNAME'], '!!!')
      if toolchain_file:
        # input('Toolchain file: ', toolchain_file)
        arguments.append('-DCMAKE_TOOLCHAIN_FILE:PATH=' + toolchain_file)
      # if toolchain.starts('clang)
      if build_system.startswith('android'):  # build_system gibt es momentan noch nicht!
        arguments.append('-DCMAKE_TOOLCHAIN_FILE:PATH=\"' + program_dir + '/Android/android-ndk-r25b/build/cmake/android.toolchain.cmake\"')
    # else: arguments.append('-DCMAKE_TOOLCHAIN_FILE:PATH=\"' + src_dir.replace('\\','/') + '/.august/toolchains/mscv2019.toolchain\"')
    else:
      if toolchain == 'mingw':
        arguments.append('-DCMAKE_TOOLCHAIN_FILE:PATH=' + src_dir.replace('\\','/') + '/build/cmake/toolchains/MinGW.toolchain')
      else:
        arguments.append('-DCMAKE_TOOLCHAIN_FILE:PATH=' + src_dir.replace('\\','/') + '/build/cmake/toolchains/LinuxGCC.toolchain')
      print('!!! USER = ', my_env['USER'], '!!!')

    arguments.append('-DTOOLCHAIN=' + toolchain)
    arguments.append('-DTHIRD_PARTY=' + third_party)
    arguments.append('-DLINK_LIBS=' + link_libs)
    arguments.append('-Wno-dev')

    arguments.append('-DCMAKE_INSTALL_PREFIX=' + install_dir)
    if not toolset is None:
      arguments.append('-T' + toolset)
    ### arguments.append('-DCMAKE_INSTALL_BINDIR=' + install_bindir)

    ###  try:
    ###    for cmake_def in project["cmake_definitions"]:
    ###      arguments.append(cmake_def)
    ###  except:
    ###      pass
  
    myprocess = subprocess.Popen(arguments, env = my_env, shell = False)
    myprocess.wait()
    if myprocess.returncode != 0:
      creation = 0
      print('cmd with failure! (', myprocess, ')')
    else:
        if verbose:
            print('Debug-Stop')
            input("Press Enter to continue...")

  #========================================================================
  if creation & 2:
    print('Python Step 2 - Build with cmake')
    print('--------------------------------')
    arguments = []
    arguments.append(cmake_exe)  # 'cmake')
    arguments.append('--build')
    arguments.append(build_dir)
    # if toolchain.startswith('msvc') or toolchain.startswith('mgw'):
    arguments.append('--config')
    if Configuration == 'Release' or Configuration == 'Debug':
      arguments.append(Configuration) 
    elif toolchain.startswith('msvc'): # Visual Studio -> Debug
        arguments.append('Debug') 
    else : # in other Multi-Configuration make 'Release'
        arguments.append('Release') 

    # if False:  # toolchain MinGW/GCC...
    if not toolchain.startswith('msvc'):
    #if not (toolchain.startswith('msvc') or toolchain.startswith('clang')):
        arguments.append('--')  # nachfolgende Befehle werden zum Build tool durchgereicht
        arguments.append('-j')
        # arguments.append('1')  # jobs...
        arguments.append('8')  # jobs...
    myprocess = subprocess.Popen(arguments, env = my_env, shell = False)
    myprocess.wait()
    if myprocess.returncode != 0:
      creation = 0
      print('cmd: (', arguments, ')')
      print('cmd with failure! (', myprocess, ')')
    else:
        if verbose:
            print('Debug-Stop')
            input("Press Enter to continue...")
  
  #========================================================================
  if creation & 0x04:
    print('Python Step 3 - Install')
    arguments = []
    arguments.append(cmake_exe)  # 'cmake')
    arguments.append('--install')
    arguments.append(build_dir)
    myprocess = subprocess.Popen(arguments, env = my_env, shell = False)
    myprocess.wait()
    if myprocess.returncode != 0:
      creation = 0
      print('cmd with failure! (', myprocess, ')')
    else:
        if verbose:
            print('Debug-Stop')
            input("Press Enter to continue...")

  #========================================================================
  if creation & 0x08:   # ==>Run
    print('Python Step 4 - Execute OpenSoar')
    if toolchain.startswith('msvc'):
      if Configuration == 'Release' or Configuration == 'Debug':
        arguments.append(Configuration) 
      else: # MSVC:
        arguments.append('Debug') 
      build_dir = build_dir + '/'+ Configuration
      opensoar_app = project_name + '-MSVC.exe'
      opensoar_app = project_name + '.exe'  #  jetzt so...
    elif toolchain.startswith('mgw'):
      opensoar_app = project_name + '-MinGW.exe'
      # opensoar_app = 'XCSoarAug-MinGW.exe'
    elif toolchain.startswith('clang'):
      opensoar_app = project_name + '-Clang.exe'

    # arguments = [build_dir + '/' + opensoar_app, '-1400x700', '-fly', '-profile=D:/Data/OpenSoarData/August.prf', '-datapath=D:/Data/OpenSoarData']
    arguments = [build_dir + '/' + opensoar_app, '-600x480', '-profile=August.prf', '-datapath=D:/Data/OpenSoarData']
    if not os.path.exists(arguments[0]):
        print("App '", arguments[0], "' doesn't exist!")
        creation = 0
    else:
        # print('Command 4: ', arguments)
        myprocess = subprocess.Popen(arguments, env = my_env, shell = False)
        myprocess.wait()
        if myprocess.returncode != 0:
            creation = 0
            print('cmd with failure! (', myprocess, ')')

  if(not creation):
    print('Error in cmake python script')
    # input('Error in cmake python script')
