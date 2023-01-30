set (BIN_FILES
   AUTHORS.gz
   COPYING.gz
   NEWS.txt.gz
   Data/other/egm96s.dem
)

file(GLOB ICON_FILES        "icons/*.svg")
# file(GLOB GRAPHIC_FILES     "graphics/*.svg")
set(GRAPHIC_FILES )
# list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/dialog_title.svg")
# list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/launcher.svg")
list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/logo.svg")
list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/progress_border.svg")
list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/title.svg")

set(SCRIPT_FILES )
set(C_FILES)  # Reset to empty...

