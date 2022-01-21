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


BOUND_NOEXCEPT 
==============
In der Main-Software sind viele Funktionen mit 'noexcept' abgesichert.
Das funktioniert auch mit MSVC, aber nicht bei 'gebundenen' Callbacks,,;-(, Dafür habe ich das (leere) DEFINE BOUND_NOEXCEPT eingeführt, dass 
für alle anderen Compiler wirken soll - das ist natürlich schlecht für das Design. Lösungen:
- generel´le Definition '#define BOUND_NOEXCEPT noexcept' für alle anderen Compiler (schlechte Lösung, muss ja auch in die Make-Dateien...
- Mache MSVC fit für noexcept callbacks ...







