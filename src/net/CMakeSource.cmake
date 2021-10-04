set(net_SOURCES
        net/AddressInfo.cxx
        net/HostParser.cxx
        net/Resolver.cxx
        net/SocketError.cxx
        net/State.cpp
        net/ToString.cxx
        net/IPv4Address.cxx
        net/IPv6Address.cxx
        net/StaticSocketAddress.cxx
        net/AllocatedSocketAddress.cxx
        net/SocketAddress.cxx
        net/SocketDescriptor.cxx
)
list(APPEND net_SOURCES
        net/http/DownloadManager.cpp
        net/http/Progress.cpp
        net/http/OutputStreamHandler.cxx
        net/http/Multi.cxx
        net/http/FormData.cpp
        net/http/Adapter.cxx
        net/http/Setup.cxx
        net/http/Request.cxx
        net/http/CoRequest.cxx
        net/http/CoStreamRequest.cxx
        net/http/CoDownloadToFile.cpp
        net/http/Global.cxx
        net/http/Init.cpp
)

set (SCRIPT_FILES
    CMakeSource.cmake
)

