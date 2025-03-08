// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "FileUtil.hpp"
#include "time/FileTime.hxx"
#include "util/StringAPI.hxx"
#include "util/StringCompare.hxx"
#include "Compatibility/path.h"

#ifdef _WIN32
#include "sys/utime.h"
#endif

#include <windef.h> /* for MAX_PATH */

#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_POSIX
#include <dirent.h>
#include <unistd.h>
#include <fnmatch.h>
#include <utime.h>
#include <time.h>
#elif defined( _MSC_VER)
# include <corecrt_io.h>
# include <BaseTsd.h>
  typedef SSIZE_T ssize_t;
#endif

void
Directory::Create(Path path) noexcept
{
#ifdef HAVE_POSIX
  mkdir(path.c_str(), 0777);
#else /* !HAVE_POSIX */
  CreateDirectory(path.c_str(), nullptr);
#endif /* !HAVE_POSIX */
}

bool
Directory::Exists(Path path) noexcept
{
#ifdef HAVE_POSIX
  struct stat st;
  if (stat(path.c_str(), &st) != 0)
    return false;

  return S_ISDIR(st.st_mode);
#else
  DWORD attributes = GetFileAttributes(path.c_str());
  return attributes != INVALID_FILE_ATTRIBUTES &&
    (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
#endif
}

/**
 * Checks whether the given string str equals "." or ".."
 * @param str The string to check
 * @return True if string equals "." or ".."
 */
#ifndef HAVE_POSIX
static bool
IsDots(const char *str) noexcept
{
  return StringIsEqual(str, ".") || StringIsEqual(str, "..");
}
#endif

#ifndef HAVE_POSIX /* we use fnmatch() on POSIX */

[[gnu::pure]]
static bool
checkFilter(const char *filename, const char *filter) noexcept
{
  // filter = e.g. "*.igc" or "config/*.prf"
  // todo: make filters like "config/*.prf" work

  // if invalid or short filter "*" -> return true
  // todo: check for asterisk
  if (!filter || StringIsEmpty(filter + 1))
    return true;

  return StringEndsWithIgnoreCase(filename, filter + 1);
}

static bool
ScanFiles(File::Visitor &visitor, Path sPath,
          const char* filter = "*")
{
  char DirPath[MAX_PATH];
  char FileName[MAX_PATH];

  if (sPath != nullptr)
    // e.g. "/test/data/something"
    strcpy(DirPath, sPath.c_str());
  else
    DirPath[0] = 0;

  // "/test/data/something/"
  strcat(DirPath, DIR_SEPARATOR_S);
  strcpy(FileName, DirPath);

  // "/test/data/something/*.igc"
  strcat(FileName, filter);

  // Find the first matching file
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind = FindFirstFile(FileName, &FindFileData);

  // If no matching file found -> return false
  if (hFind == INVALID_HANDLE_VALUE)
    return false;

  // Loop through remaining matching files
  while (true) {
    if (!IsDots(FindFileData.cFileName) &&
        !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
        checkFilter(FindFileData.cFileName, filter)) {
      // "/test/data/something/"
      strcpy(FileName, DirPath);
      // "/test/data/something/blubb.txt"
      strcat(FileName, FindFileData.cFileName);
      // Call visitor with the file that was found
      visitor.Visit(Path(FileName), Path(FindFileData.cFileName));
    }

    // Look for next matching file
    if (!FindNextFile(hFind, &FindFileData)) {
      if (GetLastError() == ERROR_NO_MORE_FILES)
        // No more files/folders
        // -> Jump out of the loop
        break;
      else {
        // Some error occured
        // -> Close the handle and return false
        FindClose(hFind);
        return false;
      }
    }
  }
  // Close the file handle
  FindClose(hFind);

  return true;
}
#endif /* !HAVE_POSIX */

static bool
ScanDirectories(File::Visitor &visitor, bool recursive,
                Path sPath, const char* filter = "*")
{
#ifdef HAVE_POSIX
  DIR *dir = opendir(sPath.c_str());
  if (dir == nullptr)
    return false;

  char FileName[MAX_PATH];
  strcpy(FileName, sPath.c_str());
  size_t FileNameLength = strlen(FileName);
  FileName[FileNameLength++] = '/';

  struct dirent *ent;
  while ((ent = readdir(dir)) != nullptr) {
    // omit '.', '..' and any other files/directories starting with '.'
    if (*ent->d_name == '.')
      continue;

    strcpy(FileName + FileNameLength, ent->d_name);

    struct stat st;
    if (stat(FileName, &st) < 0)
      continue;

    if (S_ISDIR(st.st_mode) && recursive)
      ScanDirectories(visitor, true, Path(FileName), filter);
    else {
      int flags = 0;
#ifdef FNM_CASEFOLD
      flags = FNM_CASEFOLD;
#endif
      if (S_ISREG(st.st_mode) && fnmatch(filter, ent->d_name, flags) == 0)
        visitor.Visit(Path(FileName), Path(ent->d_name));
    }
  }

  closedir(dir);
#else /* !HAVE_POSIX */
  char DirPath[MAX_PATH];
  char FileName[MAX_PATH];

  if (sPath != nullptr) {
    // e.g. "/test/data/something"
    strcpy(DirPath, sPath.c_str());
    strcpy(FileName, sPath.c_str());
  } else {
    DirPath[0] = 0;
    FileName[0] = 0;
  }

  // Scan for files in "/test/data/something"
  ScanFiles(visitor, Path(FileName), filter);

  // If we are not scanning recursive we are done now
  if (!recursive)
    return true;

  // "test/data/something/"
  strcat(DirPath, DIR_SEPARATOR_S);
  // "test/data/something/*"
  strcat(FileName, DIR_SEPARATOR_S "*");

  // Find the first file
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind = FindFirstFile(FileName, &FindFileData);

  // If no file found -> return false
  if (hFind == INVALID_HANDLE_VALUE)
    return false;

  // Loop through remaining files
  while (true) {
    if (!IsDots(FindFileData.cFileName) &&
        (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      // "test/data/something/"
      strcpy(FileName, DirPath);
      // "test/data/something/SUBFOLDER"
      strcat(FileName, FindFileData.cFileName);
      // Scan subfolder for matching files too
      ScanDirectories(visitor, true, Path(FileName), filter);
    }

    // Look for next file/folder
    if (!FindNextFile(hFind, &FindFileData)) {
      if (GetLastError() == ERROR_NO_MORE_FILES)
        // No more files/folders
        // -> Jump out of the loop
        break;
      else {
        // Some error occured
        // -> Close the handle and return false
        FindClose(hFind);
        return false;
      }
    }
  }
  // Close the file handle
  FindClose(hFind);

#endif /* !HAVE_POSIX */

  return true;
}

void
Directory::VisitFiles(Path path, File::Visitor &visitor, bool recursive)
{
  ScanDirectories(visitor, recursive, path);
}

void
Directory::VisitSpecificFiles(Path path, const char* filter,
                              File::Visitor &visitor, bool recursive)
{
  ScanDirectories(visitor, recursive, path, filter);
}

bool
File::ExistsAny(Path path) noexcept
{
#ifdef HAVE_POSIX
  struct stat st;
  return stat(path.c_str(), &st) == 0;
#else
  return GetFileAttributes(path.c_str()) != INVALID_FILE_ATTRIBUTES;
#endif
}

bool
File::Exists(Path path) noexcept
{
#ifdef HAVE_POSIX
  struct stat st;
  if (stat(path.c_str(), &st) != 0)
    return false;

  return (st.st_mode & S_IFREG);
#else
  DWORD attributes = GetFileAttributes(path.c_str());
  return attributes != INVALID_FILE_ATTRIBUTES &&
    (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
#endif
}

#ifdef HAVE_POSIX

bool
File::IsCharDev(Path path) noexcept
{
  struct stat st;
  return stat(path.c_str(), &st) == 0 && S_ISCHR(st.st_mode);
}

#endif // HAVE_POSIX

uint64_t
File::GetSize(Path path) noexcept
{
#ifdef HAVE_POSIX
  struct stat st;
  if (stat(path.c_str(), &st) < 0 || !S_ISREG(st.st_mode))
    return 0;

  return st.st_size;
#else
  WIN32_FILE_ATTRIBUTE_DATA data;
  if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &data) ||
      (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
    return 0;

  return data.nFileSizeLow | (uint64_t(data.nFileSizeHigh) << 32);
#endif

}

std::chrono::system_clock::time_point
File::GetLastModification(Path path) noexcept
{
#ifdef HAVE_POSIX
  struct stat st;
  if (stat(path.c_str(), &st) < 0 || !S_ISREG(st.st_mode))
    return {};

  return std::chrono::system_clock::from_time_t(st.st_mtime);
#else
  WIN32_FILE_ATTRIBUTE_DATA data;
  if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &data) ||
      (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
    return {};

  return FileTimeToChrono(data.ftLastWriteTime);
#endif
}

time_t
File::GetTime(Path path) noexcept
{
#ifdef HAVE_POSIX
  struct stat st;
  if (stat(path.c_str(), &st) < 0 || !S_ISREG(st.st_mode))
    return {};

  return st.st_mtime;
#else
  WIN32_FILE_ATTRIBUTE_DATA data;
  if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &data) ||
      (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
    return {};

  return std::chrono::system_clock::to_time_t(
    FileTimeToChrono(data.ftLastWriteTime));
#endif
}

bool
File::SetTime(Path path, time_t t) noexcept
{
#ifdef HAVE_POSIX
  struct stat foo;
  time_t mtime;
  struct utimbuf new_times;

  stat(filename, &foo);
  mtime = foo.st_mtime; /* seconds since the epoch */

  new_times.actime = foo.st_atime; /* keep atime unchanged */
  new_times.modtime = t;    /* set mtime defined time */
  return utime(filename, &new_times) == 0;
#else  // Windows:
# if 0
  // change file time without creation time
  struct _utimbuf ts;
  ts.actime = t ? t : std::time(0);
  ts.modtime = t ? t : std::time(0);
  return _utime(path.c_str(), &ts) == 0;
# else
  HANDLE handle = ::CreateFile(path.c_str(), GENERIC_WRITE, 0, nullptr,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (handle == INVALID_HANDLE_VALUE)
    return false;

  FILETIME ft = UnixEpochTimeToFileTime(t);
  bool result = ::SetFileTime(handle, &ft, &ft, &ft);

  CloseHandle(handle);

  return result;
# endif
}

bool
File::Touch(Path path) noexcept
{
#ifdef HAVE_POSIX
  return utime(path.c_str(), nullptr) == 0;
#else
  /// @see http://msdn.microsoft.com/en-us/library/windows/desktop/ms724205(v=vs.85).aspx

  // Create a file handle
  HANDLE handle = ::CreateFile(path.c_str(), GENERIC_WRITE, 0, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (handle == INVALID_HANDLE_VALUE)
    return false;

  // Gets the current system time
  SYSTEMTIME st;
  ::GetSystemTime(&st);

  // Converts the current system time to file time format
  FILETIME ft;
  ::SystemTimeToFileTime( &st, &ft);

  // Sets last-write time of the file to the converted current system time
  bool result = ::SetFileTime(handle, (LPFILETIME)nullptr, (LPFILETIME)nullptr,
                              &ft);
  CloseHandle(handle);

  return result;
# endif  // 0
#endif // POSIX / Windows
}

bool
File::ReadString(Path path, char *buffer, size_t size) noexcept
{
  assert(path != nullptr);
  assert(buffer != nullptr);
  assert(size > 0);

  int flags = O_RDONLY;
#ifdef O_NOCTTY
  flags |= O_NOCTTY;
#endif
#ifdef O_CLOEXEC
  flags |= O_CLOEXEC;
#endif

  int fd = _topen(path.c_str(), flags);
  if (fd < 0)
    return false;

  ssize_t nbytes = read(fd, buffer, size - 1);
  close(fd);
  if (nbytes < 0)
    return false;

  buffer[nbytes] = '\0';
  return true;
}

bool
File::WriteExisting(Path path, const char *value) noexcept
{
  assert(path != nullptr);
  assert(value != nullptr);

  int flags = O_WRONLY;
#ifdef O_NOCTTY
  flags |= O_NOCTTY;
#endif
#ifdef O_CLOEXEC
  flags |= O_CLOEXEC;
#endif

  int fd = _topen(path.c_str(), flags);
  if (fd < 0)
    return false;

  const size_t length = strlen(value);
  ssize_t nbytes = write(fd, value, length);
  return close(fd) == 0 && nbytes == (ssize_t)length;
}

bool
File::CreateExclusive(Path path) noexcept
{
  assert(path != nullptr);

  int flags = O_WRONLY | O_CREAT | O_EXCL;
#ifdef O_NOCTTY
  flags |= O_NOCTTY;
#endif
#ifdef O_CLOEXEC
  flags |= O_CLOEXEC;
#endif

  int fd = _topen(path.c_str(), flags, 0666);
  if (fd < 0)
    return false;

  close(fd);
  return true;
}
