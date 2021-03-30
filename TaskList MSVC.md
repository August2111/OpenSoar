Tasklist MSCV
-------------


Es fehlen noch die Tools zum Erstellen der Key-Events *.xci ==> *.cpp, z.B default.xci ==> InputEvent_default.cpp
==> suche in build/gettext.mk und build/generate.mk...
siehe auch src/Input/InputEvents.hpp

util/Compiler.h
// TODO(August2111): where is this defined (on GCC)?
typedef size_t   ssize_t;  // (only if is not predefined...

Const und nonconst arrays
  // TODO(August2111): // is 0x200 big enough? handle with care,
  // _tcslen is not a constexpr
  char buffer[0x200];
  
List.hpp- WorkAround unbedingt beseitigen, dass MUSS ordentlich laufen!
  
 8 grundsätzliche Problem-Projekte (09.92.2021, 7:00)
======================================================
 * Tracking: neue Thread-Komponente(!), nicht mehr Boost::Asio-Thread! (MPD: MusicPlayer...)
 * Device:
   - Probleme in event/SocketEvent.hxx
   - Probleme in event/TimerEvent.hxx
   - Device/Port/TCPClientPort.cpp
 * Renderer
   - abhängig von MapWindow, Terrain...
 * Dialogs:
   - abhängig von MapWindow und Device
 *io
   - 
 * libXCSoar
 * XCSoarAug-MSVC

 
Jasper bei Terrain 'herausoperieren'...
 




