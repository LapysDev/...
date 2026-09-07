#include <cstdlib> // --> ::std::size_t
#if defined _WIN32
# include <windows.h> // --> ::HANDLE, ::LPSECURITY_ATTRIBUTES, ::LPSYSTEM_INFO, ::LPVOID, ::MEM_EXTENDED_PARAMETER, ::PVOID, ::SYSTEM_INFO; FALSE, INVALID_HANDLE_VALUE, MEM_RELEASE, MEM_REPLACE_PLACEHOLDER, MEM_RESERVE, MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, PAGE_READWRITE; ::GetSystemInfo(…), ::VirtualAlloc2(…), ::VirtualFree(…)

void* allocate_circular(::std::size_t const size) {
  ::SYSTEM_INFO systemInformation = {};

  // ...
  ::GetSystemInfo(static_cast< ::LPSYSTEM_INFO>(&systemInformation));

  if (0u == size % systemInformation.dwAllocationGranularity)
  if (void *const allocation = ::VirtualAlloc2(
    static_cast< ::HANDLE>(NULL),                          // ->> This process
    static_cast< ::PVOID> (NULL),                          // ->> Any base address
    size * 2u,                                            //
    MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, // ->> Allocation policies and page security
    static_cast< ::MEM_EXTENDED_PARAMETER*>(NULL), 0u     // ->> No extended parameters
  )) {
    if (FALSE == ::VirtualFree(static_cast< ::LPVOID>(allocation), size, MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER)) {
      (void) ::VirtualFree(static_cast< ::LPVOID>(allocation), 0u, MEM_RELEASE);
      return NULL;
    }

    void    *const views[] = {allocation, static_cast<unsigned char*>(allocation) + size};
    ::HANDLE const mapping = ::CreateFileMappingW(INVALID_HANDLE_VALUE,
      static_cast< ::LPSECURITY_ATTRIBUTES>(NULL),
      PAGE_READWRITE,
      static_cast<DWORD>(static_cast<ULONGLONG>(size) >> 32),
      static_cast<DWORD>(size),
      NULL
    );

    if (!mapping) {
      ::VirtualFree(views[0], 0, MEM_RELEASE);
      ::VirtualFree(views[1], 0, MEM_RELEASE);
      return NULL;
    }

    for (::std::size_t index = 0; index < 2; ++index) {
      if (!::MapViewOfFile3(
        mapping,
        NULL,
        views[index],
        0,
        size,
        MEM_REPLACE_PLACEHOLDER,
        PAGE_READWRITE,
        NULL,
        0
      )) {
        // Failure cleanup omitted here for brevity.
        ::CloseHandle(mapping);
        return NULL;
      }
    }

    ::CloseHandle(mapping);
    return allocation;
  }

  return NULL;
}
#elif defined __APPLE__ or defined __unix__
# include <fcntl.h>    // --> O_CREAT, O_EXCL, O_RDWR
# include <sys/mman.h> // --> MAP_ANONYMOUS, MAP_FAILED, MAP_FIXED, MAP_PRIVATE, MAP_SHARED, PROT_NONE, PROT_READ, PROT_WRITE
# include <sys/stat.h>
# include <unistd.h>

void* allocate_circular(const char *name, ::std::size_t size) {
  int fd = ::shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd == -1)
    return NULL;

  ::shm_unlink(name);

  if (::ftruncate(fd, static_cast<off_t>(size)) == -1) {
    ::close(fd);
    return NULL;
  }

  void *allocation = ::mmap(
    NULL,
    size * 2,
    PROT_NONE,
    MAP_PRIVATE | MAP_ANONYMOUS,
    -1,
    0
  );

  if (allocation == MAP_FAILED) {
    ::close(fd);
    return NULL;
  }

  void *views[] = {
    allocation,
    static_cast<char *>(allocation) + size
  };

  for (::std::size_t index = 0; index < 2; ++index) {
    if (::mmap(
      views[index],
      size,
      PROT_READ | PROT_WRITE,
      MAP_FIXED | MAP_SHARED,
      fd,
      0
    ) == MAP_FAILED) {
      ::munmap(allocation, size * 2);
      ::close(fd);
      return NULL;
    }
  }

  ::close(fd);
  return allocation;
}
#endif

/* Main */
int main(int, char*[]) /* noexcept */ {
  ::std::size_t size = ;

  #if defined _WIN32
  #elif defined __APPLE__ or defined __unix__
  #endif
}
