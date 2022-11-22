project('XCSoar', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3', 
	'cpp_std=c++20']
)

global_args = [
    '-DWIN32',
    '-D_WINDOWS',
    '-DUSE_GDI',
    '-DCURL_STATICLIB',
    '-DNDEBUG',
    '-DOUTPUT_FOLDER=D:/Projects/Gliding/XCSoar/_build',
    '-D__MSVC__',
    '-D_UNICODE',
    '-D_AUG_MSC=1',
    '-DNO_ERROR_CHECK',
    '-DWIN32_LEAN_AND_MEAN',
    '-D_CRT_SECURE_NO_WARNINGS',
    '-D_AMD64_',
    '-DSODIUM_STATIC=1',
    '-DLDAP_STATICLIB',
    '-DGNU_CONST=',
    '-DGNU_PURE=',
    '-DGIT_COMMIT_ID="d83dc809dd"',
    '-DNOMINMAX',
    '-DXCSOAR_VERSION="7.19"',
    '-DUSE_WIN32_RESOURCES',
    '-DUSE_WINUSER',
    '-DBOOST_ASIO_SEPARATE_COMPILATION',
    '-DBOOST_JSON_HEADER_ONLY',
    '-DBOOST_JSON_STANDALONE',
    '-DBOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON',
    '-DCMAKE_INTDIR="Debug"',
    '-D_MSC_VER=1929',                                   # TODO(aug): why isn't this predefined???

]
# add_global_arguments(['-DSOME_TOKEN=value','-DWIN32_LEAN_AND_MEAN'], language : 'cpp')
add_global_arguments(global_args, language : 'cpp')
# add_global_arguments('-DWIN32_LEAN_AND_MEAN', language : 'cpp')
add_project_arguments('/Zc:__cplusplus',
    '/wd5030',                                         # C5030: Das Attribut ("gnu::pure", "gnu::const") wurde nicht erkannt.
    language : 'cpp'
)


incdir = include_directories('src', 'src/Engine', '/link_libs/boost/boost-1.77.0/include/boost-1_77')
# boost_inc = include_directories('src', 'src/Engine')

subdir('src/json')
# libs_dep = dependency('libjson')  # , fallback : ['libsimple', 'libsimple_dep'])

### xc_sources = [
###     'src/Startup.cpp',
###     'src/Version.cpp'
### ]
xc_sources = [
    'src/ActionInterface.cpp',
    'src/ApplyExternalSettings.cpp',
    'src/ApplyVegaSwitches.cpp',
    'src/Asset.cpp',
    'src/BallastDumpManager.cpp',
    'src/BatteryTimer.cpp',
    'src/Components.cpp',
    'src/CommandLine.cpp',
    'src/DataGlobals.cpp',
    'src/FlightStatistics.cpp',
    'src/HorizonWidget.cpp',
    'src/Interface.cpp',
    'src/LocalPath.cpp',
    'src/MainWindow.cpp',
    'src/ProcessTimer.cpp',
    'src/ProgressWindow.cpp',
    'src/ProgressGlue.cpp',
    'src/Protection.cpp',
    'src/RadioFrequency.cpp',
    'src/ResourceLoader.cpp',
    'src/Simulator.cpp',
    'src/Startup.cpp',
    'src/UIActions.cpp',
    
    'src/Pan.cpp',
    
    'src/PageSettings.cpp',
    'src/PageState.cpp',
    'src/PageActions.cpp',
    'src/StatusMessage.cpp',
    'src/PopupMessage.cpp',
    'src/Message.cpp',
    
    'src/LogFile.cpp',
    'src/DrawThread.cpp',
    
    'src/UIReceiveBlackboard.cpp',
    'src/UIGlobals.cpp',
    'src/UIState.cpp',
    'src/UISettings.cpp',
    'src/DisplaySettings.cpp',
    'src/MapSettings.cpp',
    'src/SystemSettings.cpp',

    'src/MergeThread.cpp',
    'src/CalculationThread.cpp',
    'src/DisplayMode.cpp',

    'src/UtilsSettings.cpp',
    'src/UtilsSystem.cpp',
    'src/Version.cpp',

    'src/RateLimiter.cpp',
    'src/TeamActions.cpp'
]

exe = executable('xcsoar_exe', xc_sources,
  include_directories : incdir, 
#   dependencies : libs_dep,
  link_with: json,
  cpp_args : '-DWIN32_LEAN_AND_MEAN',
  install : true)

# test('basic', exe)
=======
project('XCSoar', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=4', 
	'cpp_std=c++20']
)

global_args = [
    '-DWIN32',
    '-D_WINDOWS',
    '-DUSE_GDI',
    '-DCURL_STATICLIB',
    '-DNDEBUG',
    '-DPROJECT_OUTPUT_FOLDER=D:/Projects/Gliding/XCSoar/output',
    '-D__MSVC__',
    '-D_UNICODE',
    '-D_AUG_MSC=1',
    '-DNO_ERROR_CHECK',
    '-DWIN32_LEAN_AND_MEAN',
    '-D_CRT_SECURE_NO_WARNINGS',
    '-D_AMD64_',
    '-DSODIUM_STATIC=1',
    '-DLDAP_STATICLIB',
    '-DGNU_CONST=',
    '-DGNU_PURE=',
    '-DGIT_COMMIT_ID="d83dc809dd"',
    '-DNOMINMAX',
    '-DXCSOAR_VERSION="7.19"',
    '-DUSE_WIN32_RESOURCES',
    '-DUSE_WINUSER',
    '-DBOOST_ASIO_SEPARATE_COMPILATION',
    '-DBOOST_JSON_HEADER_ONLY',
    '-DBOOST_JSON_STANDALONE',
    '-DBOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON',
    '-DCMAKE_INTDIR="Debug"',
    '-D_MSC_VER=1929',                                   # TODO(aug): why isn't this predefined???

]
# add_global_arguments(['-DSOME_TOKEN=value','-DWIN32_LEAN_AND_MEAN'], language : 'cpp')
add_global_arguments(global_args, language : 'cpp')
# add_global_arguments('-DWIN32_LEAN_AND_MEAN', language : 'cpp')
add_project_arguments('/Zc:__cplusplus',
    '/wd5030',                                         # C5030: Das Attribut ("gnu::pure", "gnu::const") wurde nicht erkannt.
    language : 'cpp'
)


incdir = include_directories('src', 'src/Engine', '/link_libs/boost/boost-1.77.0/include/boost-1_77')
# boost_inc = include_directories('src', 'src/Engine')

subdir('src/json')
# libs_dep = dependency('libjson')  # , fallback : ['libsimple', 'libsimple_dep'])

### xc_sources = [
###     'src/Startup.cpp',
###     'src/Version.cpp'
### ]
xc_sources = [
    'src/ActionInterface.cpp',
    'src/ApplyExternalSettings.cpp',
    'src/ApplyVegaSwitches.cpp',
    'src/Asset.cpp',
    'src/BallastDumpManager.cpp',
    'src/BatteryTimer.cpp',
    'src/Components.cpp',
    'src/CommandLine.cpp',
    'src/DataGlobals.cpp',
    'src/FlightStatistics.cpp',
    'src/HorizonWidget.cpp',
    'src/Interface.cpp',
    'src/LocalPath.cpp',
    'src/MainWindow.cpp',
    'src/ProcessTimer.cpp',
    'src/ProgressWindow.cpp',
    'src/ProgressGlue.cpp',
    'src/Protection.cpp',
    'src/RadioFrequency.cpp',
    'src/ResourceLoader.cpp',
    'src/Simulator.cpp',
    'src/Startup.cpp',
    'src/UIActions.cpp',
    
    'src/Pan.cpp',
    
    'src/PageSettings.cpp',
    'src/PageState.cpp',
    'src/PageActions.cpp',
    'src/StatusMessage.cpp',
    'src/PopupMessage.cpp',
    'src/Message.cpp',
    
    'src/LogFile.cpp',
    'src/DrawThread.cpp',
    
    'src/UIReceiveBlackboard.cpp',
    'src/UIGlobals.cpp',
    'src/UIState.cpp',
    'src/UISettings.cpp',
    'src/DisplaySettings.cpp',
    'src/MapSettings.cpp',
    'src/SystemSettings.cpp',

    'src/MergeThread.cpp',
    'src/CalculationThread.cpp',
    'src/DisplayMode.cpp',

    'src/UtilsSettings.cpp',
    'src/UtilsSystem.cpp',
    'src/Version.cpp',

    'src/RateLimiter.cpp',
    'src/TeamActions.cpp'
]

exe = executable('xcsoar_exe', xc_sources,
  include_directories : incdir, 
#   dependencies : libs_dep,
  link_with: json,
  cpp_args : '-DWIN32_LEAN_AND_MEAN',
  install : true)

# test('basic', exe)
