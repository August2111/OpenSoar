set(TARGET_NAME "XCSoarAug-MSVC")  # hardcoded yet

message(STATUS "+++ System = WIN32 / MSVC (${TOOLCHAIN})!")

set(LIB_PREFIX "" )  # "lib")
set(LIB_SUFFIX ".lib")    # "a")
# ??? add_compile_definitions(PROJECT_OUTPUT_FOLDER=${OUTPUT_FOLDER})

add_definitions(-DIS_OPENVARIO)  # add special OpenVario functions
add_compile_definitions(__MSVC__)
#********************************************************************************
# if(AUGUST_SPECIAL)
    add_compile_definitions(_AUG_MSC=1)
# endif()
#********************************************************************************

add_compile_definitions(_UNICODE)
add_compile_definitions(UNICODE)  # ???
add_compile_definitions(NO_ERROR_CHECK)  # EnumBitSet funktioniert m.E. noch nicht korrekt!!!!
add_compile_definitions(WIN32_LEAN_AND_MEAN)
 # warning C4996: 'xxx': The POSIX name for this item is deprecated. Instead, use the ISO C and C++ conformant name: _wcsdup. See online help for details.
 # xxx: wcscpy, wcsdup, strtok, strcpy, strdup, ....
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
# add_definitions(/std:c++latest /Zc:__cplusplus)
# add_compile_definitions(-std=c++20)
add_definitions(/std:c++20)
add_definitions(/Zc:__cplusplus)
add_definitions(/wd5030)


add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
add_compile_definitions(BOOST_JSON_HEADER_ONLY)
add_compile_definitions(BOOST_JSON_STANDALONE)
add_compile_definitions(BOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON) 

# add_definitions(/Zc:wchar_t)

include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporary data!
if (ON OR WIN64)  # momentan kein Flag für 64bit verfügbar!
    add_compile_definitions(_AMD64_)
else()
    message(FATAL_ERROR "Error: WIN32 not implemented?")
endif()
set(FREEGLUT_LIB_DIR "${LINK_LIBS}/freeglut-MSVC-3.0.0-2/freeglut")
#      set(SODIUM_LIB "${LINK_LIBS}/libsodium/x64/Release/v142/static/libsodium.lib")
add_compile_definitions(SODIUM_STATIC=1)  # MSCV only...

# see below      add_compile_definitions(CURL_STATICLIB)
add_compile_definitions(LDAP_STATICLIB)

set(SSL_LIB )  # no ssl lib on windows! ${LINK_LIBS}/openssl/OpenSSL-Win64/lib/VC/static/libssl64MDd.lib)
set(CRYPTO_LIB Crypt32.lib BCrypt.lib) # no (OpenSSL-)crypto lib on windows!

set(USE_MEMORY_CANVAS OFF)

set(PERCENT_CHAR %%)
set(DOLLAR_CHAR \$)


if(EXISTS "D:/Programs")  # on Windows only - and at Flaps6 (August2111)
    list(APPEND CMAKE_PROGRAM_PATH "D:/Programs")
endif()
