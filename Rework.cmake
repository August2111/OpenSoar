TODO 08.02.2021

cmake-error4.txt:
-----------------
create memory_fields.c (statt copy...) 'D:/Projects/XCSoar/cmake/temp/data/memory_fields.c'
IGCFileUpload.cpp ist bereits weglide Cloud/CMakeSource
Contest/Solvers/OLCTriangle.cpp  disabled (moved ???) Contest/Solvers/OLCTriangleRules.cpp changed in src/engine/CMakeSource.txt
FileDescriptor.cxx  moved from src/system to src/io
Screen-Sources (f.e. BufferCanvas.cpp) moved from src/Screen to src/ui/canvas/gdi  (or src/ui/canvas ?)
jpc_rtc.cpp - special file for outsourcing jasper?
Settings/Panels/WeGlideConfigPanel.cpp  -> ist bereits weglide ausgeschalten in Dialogs/CMakeSource


cmake-error4.txt:
-----------------
Data/XCSoar.def - brauche ich für msvc
src/system/LogError.cpp gibt es nicht mehr?
splitting dir Screen and ui -> make different subdirs!
UpperCase to LowerCase in event, lua, net, thread, time, ui, util, io
moved src/event to src/ui/event in the CMakeSource
switch off lua/Cloud.cpp  (no we_glide!)
switch off io/LogFile.cpp  -> where is this functionality???


Commit: 

add util/xcs_functional.hpp

Math/FastMath.cpp: Template for sin_cos (no sincos in STL
Geo/ConvexHull/GrahamScan.cpp: #include <iterator>  // necessary for std::back_inserter
Data to create from XCSoar-Tools in temp/data copied!!
if you don't include winsock2. h before windows.h, their are many probems behind 
TODO(August2111):  in Weather/METARParser.cpp exists StaticString<max>::begin(), das ist gleich StaticString<max>::c_str() (zeigt auf den Anfang eines Strings
* 2 Möglichkeiten: die begin() Funktion im StaticString<max> anzupassen oder statt begin() c_str() zu nehmen
Read and Write to a file with VS: Include "io.h" and align read => _read, write => _write and close => _close!!! (see util/Computer.h, line 148 ff.

Bufferhandling with NON-const buffersize...
  // TODO(August2111): // is 0x200 big enough? handle with care,
  // _tcslen is not a constexpr
  char buffer[0x200];


//  branch cmake withot we_glide

src/Renderer/WayPointLabeList.hpp:  geht so unter MSVC nicht ;-(
  const Label *begin() const {
    return labels.begin();
  }
  const Label *end() const {
    return labels.end();
  }
  
  Checke GCC_OLDER_THAN (unter MSVC = TRUE!), ob die Dinge der MSVC nicht auch kann!?!
  * Dazu gehören BindungsFunctions u.ä. 
  
MSVC: Problems with allocation of iterator to member object
Unfortunately the iterator cannot used directly
The iterator has to be redirected by *(&begin())
- Terrain/Loader.cpp
- Terrain/RasterTileCache.cpp

MSVC: include "jasper/jas_compiler.h" before including "jasper/jas_seq.h"
- Terrain/RasterTile.cpp
- Terrain/RasterTileCache.cpp
- Terrain/ZzipStream.hpp

MSVC: Events with binding has to be without 'noexcept'
TODO(August2111): to look for a better solution for this
- Tracking/SkyLines/Client.cpp
- Tracking/SkyLines/Client.hpp
- event/DeferEvent.hxx
- event/SocketEvent.hxx
- event/TimerEvent.hxx
- event/Loop.hxx
- Device/Port/TCPClientPort.cpp
- Device/Port/TCPClientPort.hpp
- Device/Port/TCPPort.cpp
- Device/Port/TCPPort.hpp
- Device/Port/UDPPort.cpp
- Device/Port/UDPPort.hpp


non const array size definitions
check the possibility of the max size
... and the intention of XCSoar
Buffer with non-const size
- io/CSVLine

MSVC: Missing io.h and definition of mode_t 
io/FileDescriptor.hxx

cmake: include c-ares lib

cmake: reorganize the jasper inclusion

C++17: binary_function and unary_function removed
since C++11 they are 'DEPRECATED'
- util/xcs_functional.hpp 

Cleanup the control file and define some new tasks

Letzte Handgriffe am Project XCsoarAug-MSVC
- kopiere XCSoar.hpp (von XCSoarAug-master) zu ./Data 
- kopiere den data-Folder (von XCSoarAug-master) zu ./output/data 
- kopiere den _Deprecated.lib (von XCSoarAug-master) zu ./src/_Deprecated

BOUND_NOEXPECT!!! see TaskList MSVC

