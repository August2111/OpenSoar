set(_SOURCES
        Weather/METARParser.cpp
        Weather/NOAADownloader.cpp
        Weather/NOAAFormatter.cpp
        Weather/NOAAGlue.cpp
        Weather/NOAAStore.cpp
        Weather/NOAAUpdater.cpp
        Weather/PCMet/Images.cpp
        Weather/PCMet/Overlays.cpp
#        Weather/Rasp/Providers.cpp
        Weather/Rasp/RaspCache.cpp
        Weather/Rasp/RaspRenderer.cpp
        Weather/Rasp/RaspStore.cpp
        Weather/Rasp/RaspStyle.cpp
        Weather/Rasp/Configured.cpp
       # SkySight:
        Weather/Skysight/Skysight.cpp
        Weather/Skysight/SkysightAPI.cpp
        Weather/Skysight/SkysightRegions.cpp

        Weather/Skysight/Request.cpp
        Weather/Skysight/CDFDecoder.cpp
        Weather/Skysight/APIQueue.cpp
)

set(SCRIPT_FILES
    CMakeSource.cmake
)

