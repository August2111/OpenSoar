set(SCREEN_SRC_DIR "Screen")  # branch cmake!!!
## moved to ui'set(CANVAS_SRC_DIR "src/ui/canvas")  # branch cmake!!!
## moved to ui'set(WINDOW_SRC_DIR "src/ui/window")
## moved to ui'
## moved to ui'set(Screen_SOURCES
## moved to ui'        ${CANVAS_SRC_DIR}/BufferCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/BufferWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Canvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Color.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Debug.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/DoubleBufferWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Icon.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Layout.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/ProgressBar.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Ramp.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/SingleWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/SolidContainerWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/TerminalWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Util.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Window.cpp
## moved to ui')
## moved to ui'
if(UNIX)  # OpenGL
## moved to ui'  list(APPEND Screen_SOURCES
## moved to ui'        ${CANVAS_SRC_DIR}/FreeType/Font.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/FreeType/Init.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/X11/TopWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/Cache.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Init.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Dynamic.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Rotate.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Geo.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Globals.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Extension.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/FBO.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/VertexArray.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/ConstantAlpha.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Bitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/RawBitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Canvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/BufferCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/TopCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/SubCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Texture.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/UncompressedImage.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Buffer.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Shapes.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Surface.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Shaders.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/OpenGL/Triangulate.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/LibPNG.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/LibJPEG.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/LibTiff.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/DoubleClick.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/GeoBitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/Pen.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/LargeTextWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/Window.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/WList.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/ContainerWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/TopWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/SingleWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/MoreCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/Files.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/Bitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/Custom/ResourceBitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/GLX/Init.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/GLX/TopCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/FB/Window.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/FB/TopWindow.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/FB/SingleWindow.cpp
## moved to 'src/ui'  )
elseif(WIN32)  # GDI
if(USE_MEMORY_CANVAS)
  list(APPEND Screen_SOURCES
## moved to ui'        ${CANVAS_SRC_DIR}/custom/Cache.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/memory/Bitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/memory/RawBitmap.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/memory/VirtualCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/memory/SubCanvas.cpp
## moved to ui'        ${CANVAS_SRC_DIR}/memory/Canvas.cpp
  )
# MEMORY_CANVAS_CPPFLAGS = -DUSE_MEMORY_CANVAS
else()
   ### message(FATAL_ERROR "GDI-Stop!  in Screen/CmakeSource")
### 10.02.2021   list(APPEND Screen_SOURCES
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Bitmap.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Brush.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/BufferCanvas.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Canvas.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/ContainerWindow.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Font.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Init.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/LargeTextWindow.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/PaintCanvas.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/PaintWindow.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Pen.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/RawBitmap.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/ResourceBitmap.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/SingleWindow.cpp
### 10.02.2021#        Screen/GDI/Timer.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/TopWindow.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/VirtualCanvas.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/Window.cpp
### 10.02.2021        ${SCREEN_SRC_DIR}/GDI/WindowCanvas.cpp
### 10.02.2021  )
  endif()
endif()

list(APPEND Screen_SOURCES
  Debug.cpp

  Layout.cpp
#  ProgressBar.cpp
#  TerminalWindow.cpp

)

if(USE_GDI)
  # list(APPEND Screen_SOURCES GDI/LargeTextWindow.cpp)
else()
  ## list(APPEND Screen_SOURCES Custom/LargeTextWindow.cpp)
endif()
set(SCRIPT_FILES CMakeSource.cmake)



