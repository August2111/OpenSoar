set(_SOURCES
        Device/Config.cpp
        Device/Factory.cpp
        Device/Declaration.cpp
        Device/Descriptor.cpp
        Device/device.cpp
        Device/Dispatcher.cpp
        Device/DataEditor.cpp
        Device/Driver.cpp
        Device/Driver/AirControlDisplay.cpp
        Device/Driver/AltairPro.cpp
        Device/Driver/Anemoi.cpp
        Device/Driver/ATR833/Register.cpp
        Device/Driver/ATR833/Device.cpp
        Device/Driver/BlueFly/Misc.cpp
        Device/Driver/BlueFly/Parser.cpp
        Device/Driver/BlueFly/Register.cpp
        Device/Driver/BlueFly/Settings.cpp
        Device/Driver/BorgeltB50.cpp
        Device/Driver/CAI302/Declare.cpp
        Device/Driver/CAI302/Logger.cpp
        Device/Driver/CAI302/Manage.cpp
        Device/Driver/CAI302/Mode.cpp
        Device/Driver/CAI302/Parser.cpp
        Device/Driver/CAI302/PocketNav.cpp
        Device/Driver/CAI302/Protocol.cpp
        Device/Driver/CAI302/Register.cpp
        Device/Driver/CAI302/Settings.cpp
        Device/Driver/CaiGpsNav.cpp
        Device/Driver/CaiLNav.cpp
        Device/Driver/Condor.cpp
        Device/Driver/CProbe.cpp
        Device/Driver/EW.cpp
        Device/Driver/EWMicroRecorder.cpp
        Device/Driver/Eye.cpp
        Device/Driver/FLARM/BinaryProtocol.cpp
        Device/Driver/FLARM/CRC16.cpp
        Device/Driver/FLARM/Declare.cpp
        Device/Driver/FLARM/Device.cpp
        Device/Driver/FLARM/Logger.cpp
        Device/Driver/FLARM/Mode.cpp
        Device/Driver/FLARM/Parser.cpp
        Device/Driver/FLARM/Register.cpp
        Device/Driver/FLARM/Settings.cpp
        Device/Driver/FLARM/StaticParser.cpp
        Device/Driver/FLARM/TextProtocol.cpp
        Device/Driver/FlymasterF1.cpp
        Device/Driver/FlyNet.cpp
        Device/Driver/Flytec/Logger.cpp
        Device/Driver/Flytec/Parser.cpp
        Device/Driver/Flytec/Register.cpp
        Device/Driver/FreeVario.cpp
        Device/Driver/Generic.cpp
        Device/Driver/ILEC.cpp
        Device/Driver/IMI/Declare.cpp
        Device/Driver/IMI/Internal.cpp
        Device/Driver/IMI/Logger.cpp
        Device/Driver/IMI/Protocol/Checksum.cpp
        Device/Driver/IMI/Protocol/Communication.cpp
        Device/Driver/IMI/Protocol/Conversion.cpp
        Device/Driver/IMI/Protocol/IGC.cpp
        Device/Driver/IMI/Protocol/MessageParser.cpp
        Device/Driver/IMI/Protocol/Protocol.cpp
        Device/Driver/IMI/Register.cpp
        Device/Driver/KRT2.cpp
        Device/Driver/AR62xx.cpp
        Device/Driver/Larus.cpp
        Device/Driver/Leonardo.cpp
        Device/Driver/LevilAHRS_G.cpp
        Device/Driver/LX/Convert.cpp
        Device/Driver/LX/Declare.cpp
        Device/Driver/LX/Logger.cpp
        Device/Driver/LX/LXN.cpp
        Device/Driver/LX/Mode.cpp
        Device/Driver/LX/NanoDeclare.cpp
        Device/Driver/LX/NanoLogger.cpp
        Device/Driver/LX/Parser.cpp
        Device/Driver/LX/Protocol.cpp
        Device/Driver/LX/Register.cpp
        Device/Driver/LX/Settings.cpp
        Device/Driver/NmeaOut.cpp
        Device/Driver/OpenVario.cpp
        Device/Driver/PosiGraph.cpp
        Device/Driver/ThermalExpress/Driver.cpp
        Device/Driver/Vaulter.cpp
        Device/Driver/Vega/Misc.cpp
        Device/Driver/Vega/Parser.cpp
        Device/Driver/Vega/Register.cpp
        Device/Driver/Vega/Settings.cpp
        Device/Driver/Vega/Volatile.cpp
        Device/Driver/Volkslogger/Database.cpp
        Device/Driver/Volkslogger/dbbconv.cpp
        Device/Driver/Volkslogger/Declare.cpp
        Device/Driver/Volkslogger/grecord.cpp
        Device/Driver/Volkslogger/Logger.cpp
        Device/Driver/Volkslogger/Parser.cpp
        Device/Driver/Volkslogger/Protocol.cpp
        Device/Driver/Volkslogger/Register.cpp
        Device/Driver/Volkslogger/Util.cpp
        Device/Driver/Volkslogger/vlapi2.cpp
        Device/Driver/Volkslogger/vlapihlp.cpp
        Device/Driver/Volkslogger/vlconv.cpp
        Device/Driver/Westerboer.cpp
        Device/Driver/XCOM760.cpp
        Device/Driver/XCTracer/Parser.cpp
        Device/Driver/XCTracer/Register.cpp
        Device/Driver/XCVario.cpp
        Device/Driver/Zander.cpp
        Device/MultipleDevices.cpp
        Device/Parser.cpp
        Device/Port/BufferedPort.cpp
        Device/Port/ConfiguredPort.cpp
        Device/Port/DumpPort.cpp
        Device/Port/K6BtPort.cpp
        Device/Port/NullPort.cpp
        Device/Port/Port.cpp
        Device/Port/SocketPort.cpp
        Device/Port/TCPClientPort.cpp
        Device/Port/TCPPort.cpp
        Device/Port/UDPPort.cpp
        Device/Register.cpp
        Device/Simulator.cpp
        Device/Util/LineSplitter.cpp
        Device/Util/NMEAReader.cpp
        Device/Util/NMEAWriter.cpp
)
if(UNIX)
  list(APPEND _SOURCES
        Device/Port/TTYEnumerator.cpp
        Device/Port/TTYPort.cpp
  )
elseif(WIN32)
  list(APPEND _SOURCES
        Device/Port/SerialPort.cpp
  )
endif()

set(SCRIPT_FILES
    CMakeSource.cmake
)
