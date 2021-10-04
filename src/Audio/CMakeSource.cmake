set(Audio_SOURCES
        Audio/Settings.cpp
        Audio/Sound.cpp
        Audio/VarioSettings.cpp
)


if(UNIX)
  list(APPEND Audio_SOURCES
        Audio/VarioGlue.cpp
        Audio/ToneSynthesiser.cpp
        Audio/VarioSynthesiser.cpp
        Audio/PCMPlayer.cpp
        Audio/GlobalPCMResourcePlayer.cpp
        Audio/GlobalPCMMixer.cpp
        Audio/GlobalVolumeController.cpp
        Audio/MixerPCMPlayer.cpp
        Audio/PCMBufferDataSource.cpp
        Audio/PCMMixerDataSource.cpp
        Audio/PCMMixer.cpp
        Audio/PCMResourcePlayer.cpp
        Audio/VolumeController.cpp
        Audio/ALSAEnv.cpp
        Audio/ALSAPCMPlayer.cpp
  )
endif()
