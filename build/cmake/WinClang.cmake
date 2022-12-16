set(TARGET_NAME "XCSoarAug-Clang")  # hardcoded

message(STATUS "+++ System = WIN32 / Clang!")

if(NOT TOOLCHAIN)
#    set(TOOLCHAIN clang14)
endif()

include_directories(D:/Programs/LLVM/${TOOLCHAIN}/include)
# include_directories(D:/Programs/Android/android-ndk-r25b/sources/cxx-stl/llvm-libc++/include)
# include_directories(D:/Programs/Android/android-ndk-r25b/sources/cxx-stl/system/include)
include_directories(D:/Programs/LLVM/clang10/lib/clang/${TOOLCHAIN}/include)

# set(LIB_PREFIX "" )  # "lib")
# set(LIB_SUFFIX ".lib")    # "a")
set(LIB_PREFIX "lib")
set(LIB_SUFFIX ".a")
# add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
add_compile_definitions(__CLANG__)
add_compile_definitions(_AUG_CLANG=1)
# add_compile_definitions(HAVE_MSVCRT)
add_compile_definitions(_UNICODE)
add_compile_definitions(UNICODE)  # ???
add_compile_definitions(STRICT)
add_compile_definitions(_USE_MATH_DEFINES)   # necessary under C++17!
# add_compile_definitions(USE_WIN32_RESOURCES)
#this has to be defined in zzip-Project, not here (but without MSVC!)
add_compile_definitions(ZZIP_1_H)   # definition of uint32_t and Co.!

# CXX_FEATURES += -fcoroutines
# CXX_FEATURES += -fconserve-space -fno-operator-names

# add_compile_definitions(fcoroutines-ts)
## add_compile_definitions(fconserve-space)
## add_compile_definitions(fno-operator-names)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20")  ## c++20 - only for cpp!
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
## set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fconserve-space")
## set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fno-operator-names")


# gibt es nicht mehr: --- include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporary data!
if (ON OR WIN64)  # momentan kein Flag verfügbar!
    add_compile_definitions(_AMD64_)
else()
    message(FATAL_ERROR "Error: WIN32 not implemented?")
endif()

set(SSL_LIB )  # no ssl lib on windows! == Use Schannel
set(CRYPTO_LIB Crypt32.lib BCrypt.lib) # no (OpenSSL-)crypto lib on windows!

set(PERCENT_CHAR "%%" GLOBAL)
set(DOLLAR_CHAR  "$$" GLOBAL)
### message(FATAL_ERROR "Stop clang")
