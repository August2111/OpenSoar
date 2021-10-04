# set(SCREEN_SRC_DIR "${SRC}/Screen")  # branch cmake!!!
set(CANVAS_SRC_DIR "ui/canvas")  # branch cmake!!!
set(WINDOW_SRC_DIR "ui/window")
set(EVENT_SRC_DIR "ui/event")
set(CONTROL_SRC_DIR "ui/control")

set(ui_SOURCES
          ${CONTROL_SRC_DIR}/ProgressBar.hpp
          ${CONTROL_SRC_DIR}/ProgressBar.cpp
          ${CONTROL_SRC_DIR}/TerminalWindow.hpp
          ${CONTROL_SRC_DIR}/TerminalWindow.cpp
          ${CONTROL_SRC_DIR}/LargeTextWindow.hpp
          
          ${CANVAS_SRC_DIR}/AnyCanvas.hpp
          ${CANVAS_SRC_DIR}/Bitmap.hpp
          ${CANVAS_SRC_DIR}/Brush.hpp
        ${CANVAS_SRC_DIR}/BufferCanvas.cpp
          ${CANVAS_SRC_DIR}/BufferCanvas.hpp
        ### 2021 ${CANVAS_SRC_DIR}/BufferWindow.cpp
        ${CANVAS_SRC_DIR}/Canvas.cpp
          ${CANVAS_SRC_DIR}/Canvas.hpp
        ${CANVAS_SRC_DIR}/Color.cpp
          ${CANVAS_SRC_DIR}/Color.hpp
        ### ${CANVAS_SRC_DIR}/Debug.cpp
        ### ${CANVAS_SRC_DIR}/DoubleBufferWindow.cpp
        ${CANVAS_SRC_DIR}/Font.cpp  # neu 2021
          ${CANVAS_SRC_DIR}/Font.hpp  # neu 2021
        ${CANVAS_SRC_DIR}/Icon.cpp
          ${CANVAS_SRC_DIR}/Icon.hpp
        ### 2021 ${CANVAS_SRC_DIR}/Layout.cpp
        ### 2021 ${CANVAS_SRC_DIR}/ProgressBar.cpp
          ${CANVAS_SRC_DIR}/Pen.hpp
          ${CANVAS_SRC_DIR}/PortableColor.hpp
        ${CANVAS_SRC_DIR}/Ramp.cpp
          ${CANVAS_SRC_DIR}/Ramp.hpp
        ### 2021 ${CANVAS_SRC_DIR}/SingleWindow.cpp
        ### 2021 ${CANVAS_SRC_DIR}/SolidContainerWindow.cpp
        ### 2021 ${CANVAS_SRC_DIR}/TerminalWindow.cpp
          ${CANVAS_SRC_DIR}/RawBitmap.hpp  ### 2021 
          ${CANVAS_SRC_DIR}/SubCanvas.hpp  ### 2021 
        ${CANVAS_SRC_DIR}/Util.cpp
          ${CANVAS_SRC_DIR}/Util.hpp
          ${CANVAS_SRC_DIR}/VirtualCanvas.hpp  ### 2021 
        ### 2021 ${CANVAS_SRC_DIR}/Window.cpp
          ${CANVAS_SRC_DIR}/WindowCanvas.hpp  ### 2021 

        ### Window-Sources
        ${WINDOW_SRC_DIR}/BufferWindow.cpp
        ${WINDOW_SRC_DIR}/DoubleBufferWindow.cpp
        ${WINDOW_SRC_DIR}/SingleWindow.cpp
        ${WINDOW_SRC_DIR}/SolidContainerWindow.cpp
        ${WINDOW_SRC_DIR}/Window.cpp
)

if(UNIX)  # OpenGL
  list(APPEND ui_SOURCES
        ${CANVAS_SRC_DIR}/FreeType/Font.cpp
        ${CANVAS_SRC_DIR}/FreeType/Init.cpp
        ${WINDOW_SRC_DIR}/x11/TopWindow.cpp
        ${CANVAS_SRC_DIR}/custom/Cache.cpp
        ${CANVAS_SRC_DIR}/opengl/Init.cpp
        ${CANVAS_SRC_DIR}/opengl/Dynamic.cpp
        ${CANVAS_SRC_DIR}/opengl/Rotate.cpp
        ${CANVAS_SRC_DIR}/opengl/Geo.cpp
        ${CANVAS_SRC_DIR}/opengl/Globals.cpp
        ${CANVAS_SRC_DIR}/opengl/Extension.cpp
        ${CANVAS_SRC_DIR}/opengl/FBO.cpp
        ${CANVAS_SRC_DIR}/opengl/VertexArray.cpp
        ${CANVAS_SRC_DIR}/opengl/ConstantAlpha.cpp
        ${CANVAS_SRC_DIR}/opengl/Bitmap.cpp
        ${CANVAS_SRC_DIR}/opengl/RawBitmap.cpp
        ${CANVAS_SRC_DIR}/opengl/Canvas.cpp
        ${CANVAS_SRC_DIR}/opengl/BufferCanvas.cpp
        ${CANVAS_SRC_DIR}/opengl/TopCanvas.cpp
        ${CANVAS_SRC_DIR}/opengl/SubCanvas.cpp
        ${CANVAS_SRC_DIR}/opengl/Texture.cpp
        ${CANVAS_SRC_DIR}/opengl/UncompressedImage.cpp
        ${CANVAS_SRC_DIR}/opengl/Buffer.cpp
        ${CANVAS_SRC_DIR}/opengl/Shapes.cpp
        ${CANVAS_SRC_DIR}/opengl/Surface.cpp
        ${CANVAS_SRC_DIR}/opengl/Shaders.cpp
        ${CANVAS_SRC_DIR}/opengl/Triangulate.cpp
        ${CANVAS_SRC_DIR}/custom/LibPNG.cpp
        ${CANVAS_SRC_DIR}/custom/LibJPEG.cpp
        ${CANVAS_SRC_DIR}/custom/LibTiff.cpp
        ${CANVAS_SRC_DIR}/custom/DoubleClick.cpp
        ${CANVAS_SRC_DIR}/custom/GeoBitmap.cpp
        ${CANVAS_SRC_DIR}/custom/Pen.cpp
        ${CANVAS_SRC_DIR}/custom/LargeTextWindow.cpp
        ${CANVAS_SRC_DIR}/custom/Window.cpp
        ${CANVAS_SRC_DIR}/custom/WList.cpp
        ${CANVAS_SRC_DIR}/custom/ContainerWindow.cpp
        ${CANVAS_SRC_DIR}/custom/TopWindow.cpp
        ${CANVAS_SRC_DIR}/custom/SingleWindow.cpp
        ${CANVAS_SRC_DIR}/custom/MoreCanvas.cpp
        ${CANVAS_SRC_DIR}/custom/Files.cpp
        ${CANVAS_SRC_DIR}/custom/Bitmap.cpp
        ${CANVAS_SRC_DIR}/custom/ResourceBitmap.cpp
        ${CANVAS_SRC_DIR}/glx/Init.cpp
        ${CANVAS_SRC_DIR}/glx/TopCanvas.cpp
        ${CANVAS_SRC_DIR}/fb/Window.cpp
        ${CANVAS_SRC_DIR}/fb/TopWindow.cpp
        ${CANVAS_SRC_DIR}/fb/SingleWindow.cpp
  )
elseif(WIN32)  # GDI
    if(USE_MEMORY_CANVAS)
           message(FATAL_ERROR USE_MEMORY_CANVAS!!!!!)
      list(APPEND ui_SOURCES
            ${CANVAS_SRC_DIR}/custom/Cache.cpp
            ${CANVAS_SRC_DIR}/memory/Bitmap.cpp
            ${CANVAS_SRC_DIR}/memory/RawBitmap.cpp
            ${CANVAS_SRC_DIR}/memory/VirtualCanvas.cpp
            ${CANVAS_SRC_DIR}/memory/SubCanvas.cpp
            ${CANVAS_SRC_DIR}/memory/Canvas.cpp
      )
    # MEMORY_CANVAS_CPPFLAGS = -DUSE_MEMORY_CANVAS
    # else()
    elseif(USE_GDI)
       ### message(FATAL_ERROR "GDI-Stop!  in Screen/CmakeSource")
      ### message(FATAL_ERROR USE_GDI!!!!!)
      list(APPEND ui_SOURCES
                ${CANVAS_SRC_DIR}/gdi/AlphaBlend.hpp
              ${CANVAS_SRC_DIR}/gdi/Bitmap.cpp
              ${CANVAS_SRC_DIR}/gdi/GdiPlusBitmap.hpp
              ${CANVAS_SRC_DIR}/gdi/GdiPlusBitmap.cpp
              ${CANVAS_SRC_DIR}/gdi/Brush.cpp
              ${CANVAS_SRC_DIR}/gdi/BufferCanvas.cpp
                ${CANVAS_SRC_DIR}/gdi/BufferCanvas.hpp
              ${CANVAS_SRC_DIR}/gdi/Canvas.cpp
                ${CANVAS_SRC_DIR}/gdi/Canvas.hpp
                ${CANVAS_SRC_DIR}/gdi/Color.hpp
                ${CANVAS_SRC_DIR}/gdi/Features.hpp
              ${CANVAS_SRC_DIR}/gdi/Font.cpp
              ${CANVAS_SRC_DIR}/gdi/PaintCanvas.cpp
                ${CANVAS_SRC_DIR}/gdi/PaintCanvas.hpp
              ${CANVAS_SRC_DIR}/gdi/Pen.cpp
              ${CANVAS_SRC_DIR}/gdi/RawBitmap.cpp
              ${CANVAS_SRC_DIR}/gdi/ResourceBitmap.cpp
                ${CANVAS_SRC_DIR}/gdi/RootDC.hpp
              ${CANVAS_SRC_DIR}/gdi/VirtualCanvas.cpp
              ${CANVAS_SRC_DIR}/gdi/WindowCanvas.cpp

              ${WINDOW_SRC_DIR}/gdi/ContainerWindow.cpp
              ${WINDOW_SRC_DIR}/gdi/Init.cpp
              ${WINDOW_SRC_DIR}/gdi/PaintWindow.cpp
              ${WINDOW_SRC_DIR}/gdi/SingleWindow.cpp
              ${WINDOW_SRC_DIR}/gdi/TopWindow.cpp
              ${WINDOW_SRC_DIR}/gdi/Window.cpp
              ${CONTROL_SRC_DIR}/gdi/LargeTextWindow.cpp
      )
    else()
      list(APPEND ui_SOURCES  XXX)
    endif()
else()
    list(APPEND ui_SOURCES  YYY)
endif()


list(APPEND ui_SOURCES
        ${EVENT_SRC_DIR}/DelayedNotify.cpp
        ${EVENT_SRC_DIR}/Globals.cpp
        ${EVENT_SRC_DIR}/Idle.cpp
        ${EVENT_SRC_DIR}/Notify.cpp
)
if(UNIX)
  list(APPEND ui_SOURCES
          ${EVENT_SRC_DIR}/Poll/Timer.cpp
          ${EVENT_SRC_DIR}/Poll/Loop.cpp
          ${EVENT_SRC_DIR}/Poll/Queue.cpp
          ${EVENT_SRC_DIR}/Poll/X11Queue.cpp
  )
elseif(WIN32)
  list(APPEND ui_SOURCES
          ${EVENT_SRC_DIR}/shared/Timer.cpp
          ${EVENT_SRC_DIR}/shared/TimerQueue.cpp

          ${EVENT_SRC_DIR}/windows/Loop.cpp
          ${EVENT_SRC_DIR}/windows/Queue.cpp
  )
endif()

set(SCRIPT_FILES 
  CMakeSource.cmake
)


