ifeq ($(HAVE_WIN32)$(TARGET_IS_KOBO)$(TARGET_IS_DARWIN),nnn)
  # temporarily not for windows
# ifeq ($(TARGET_IS_KOBO)$(TARGET_IS_DARWIN),nn)
  # Android, UNIX, OV, Windows,...:
    # for build:
    HAVE_SKYSIGHT := y
    # for cpp sources:
    TARGET_CPPFLAGS += -DHAVE_SKYSIGHT
else 
  # Kobo, MacOS, iOS,...:
  HAVE_SKYSIGHT := n
endif
