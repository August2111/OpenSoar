set(io_SOURCES
        io/Async/AsioThread.cpp
        io/Async/GlobalAsioThread.cpp
        io/BufferedOutputStream.cxx
        io/BufferedReader.cxx
        io/BufferedLineReader.cpp
        io/ConfiguredFile.cpp
        io/ConvertLineReader.cpp
        io/CSVLine.cpp
        io/DataFile.cpp
        io/FileCache.cpp
        io/FileLineReader.cpp
        io/FileOutputStream.cxx
        io/FileReader.cxx
        io/FileTransaction.cpp
        io/GunzipReader.cxx
        io/KeyValueFileReader.cpp
        io/KeyValueFileWriter.cpp
        io/MapFile.cpp
        io/StringConverter.cpp
        io/TextWriter.cpp
        io/ZipArchive.cpp
        io/ZipLineReader.cpp
        io/ZipReader.cpp
        io/ZlibError.cxx

## branch cmake        io/LogFile.cpp  # aug!
        io/FileDescriptor.cxx
)
if(UNIX)
  list(APPEND io_SOURCES
        io/Async/SignalListener.cpp
  )
endif()

set(SCRIPT_FILES
    CMakeSource.cmake
)
