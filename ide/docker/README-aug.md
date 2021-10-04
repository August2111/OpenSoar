# XCSoar Docker Image - Bemerkungen aug:

Git for Linux:
git config --global core.autocrlf input
Git for Windows
git config --global core.autocrlf true

Docker-Build:
docker build --file ide/docker/Dockerfile -t xcsoar/xcsoar-main:latest ./ide/

Docker-Run:
 - Linux-Host:
   docker run --mount type=bind,source="$(pwd)",target=/opt/xcsoar -it xcsoar/xcsoar-build:latest xcsoar-compile ANDROID
 - Windows-Host:
   docker run --mount type=bind,source="%CD%",target=/opt/xcsoar -it xcsoar/xcsoar-build:latest xcsoar-compile ANDROID
   docker run --mount type=bind,source="%CD:\=/%",target=/opt/xcsoar -it xcsoar/xcsoar-build:latest xcsoar-compile ANDROID

  docker run --mount type=bind,source="%CD:\=/%",target=/opt/xcsoar -it xcsoar/xcsoar-master:latest /bin/bash
  
  docker run --mount type=bind,source="%CD:\=/%",target=/opt/xcsoar -it xcsoar/xcsoar-main:latest /bin/bash

docker ps --all
docker container prune
(docker system prune löscht docker komplett!!!!)

LINUX es fehlt:
(sudo apt install libglm-dev)
apt-get install -y libglm-dev
apt-get install -y libglm-dev


Falsch: /opt/xcsoar/output/src/curl-7.64.1/configure CC=i686-w64-mingw32-Falsch: gcc CXX=i686-w64-mingw32-g++ CFLAGS=-Os -g -ffunction-sections -fdata-sections -fvisibility=hidden  -march=i586 -mwin32 -mwindows -mms-bitfields CXXFLAGS=-Os -g -ffunction-sections -fdata-sections -fvisibility=hidden  -march=i586 -mwin32 -mwindows -mms-bitfields CPPFLAGS=-isystem /opt/xcsoar/output/PC/lib/i686-w64-mingw32/root/include -DNDEBUG -I./output/PC/include -DWINVER=0x0600 -D_WIN32_WINDOWS=0x0600 -D_WIN32_WINNT=0x0600 -D_WIN32_IE=0x0600 -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DHAVE_STRUCT_POLLFD -DHAVE_MSVCRT -DUNICODE -D_UNICODE -DSTRICT -isystem ./output/PC/lib/i686-w64-mingw32/root/include -DEYE_CANDY -DTESTING -DUSE_WIN32_RESOURCES   LDFLAGS=-L/opt/xcsoar/output/PC/lib/i686-w64-mingw32/root/lib -Wl,--major-subsystem-version=5 -Wl,--minor-subsystem-version=00 -Wl,-subsystem,console -static-libstdc++ -static-libgcc  LIBS=  AR=i686-w64-mingw32-ar ARFLAGS=rcs RANLIB=i686-w64-mingw32-ranlib STRIP=i686-w64-mingw32-strip --host=i686-w64-mingw32 --prefix=/opt/xcsoar/output/PC/lib/i686-w64-mingw32/root --enable-silent-rules --disable-shared --enable-static --disable-debug --enable-http --enable-ipv6 --enable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-manual --disable-threaded-resolver --disable-verbose --disable-sspi --disable-crypto-auth --disable-ntlm-wb --disable-tls-srp --disable-cookies --without-ssl --without-gnutls --without-nss --without-libssh2 --enable-pthreads=no

ANDROID
# ergänze in Zeile xx von android.mk
TARGET_CPPFLAGS += -I/usr/include -I/usr/include/x86_64-linux-gnu
# das erste wegen fwd.hpp, das zweite wegen "bits/libc-header-start.h"

apt-get install -y libc6-dev-i386


# unbedingt wegen Zugriff
# kein sudo, weil debian (und nicht ubuntu)
chmod o+r /etc/resolv.conf
# installiere alle tools, die für build python notwendig sind!
apt update
apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libsqlite3-dev libreadline-dev libffi-dev curl libbz2-dev
# download python (aktuellste Version!)
curl -O https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tar.xz
extract python
tar -xf Python-3.8.3.tar.xz



Linux:
--------
docker build --file ide/docker/Dockerfile -t xcsoar/xcsoar-main:latest ./ide/
docker run --mount type=bind,source="$(pwd)",target=/opt/xcsoar -it xcsoar/xcsoar-main:latest /bin/bash

Windows:
--------
docker build --file ide/docker/Dockerfile -t xcsoar/xcsoar-main:latest ./ide/
docker run --mount type=bind,source="%CD:\=/%",target=/opt/xcsoar -it xcsoar/xcsoar-main:latest /bin/bash