public class Program {
  /* Main */
  static void Main(string[] arguments) {
    #if NET20
      System.Console.WriteLine(".NET Framework 2.0");
    #endif
    #if NET20_OR_GREATER
      System.Console.WriteLine(".NET Framework 2.0+");
    #endif
    #if NET35
      System.Console.WriteLine(".NET Framework 3.5");
    #endif
    #if NET35_OR_GREATER
      System.Console.WriteLine(".NET Framework 3.5+");
    #endif
    #if NET40
      System.Console.WriteLine(".NET Framework 4.0");
    #endif
    #if NET40_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.0+");
    #endif
    #if NET45
      System.Console.WriteLine(".NET Framework 4.5");
    #endif
    #if NET451
      System.Console.WriteLine(".NET Framework 4.5.1");
    #endif
    #if NET451_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.5.1+");
    #endif
    #if NET452
      System.Console.WriteLine(".NET Framework 4.5.2");
    #endif
    #if NET452_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.5.2+");
    #endif
    #if NET45_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.5+");
    #endif
    #if NET46
      System.Console.WriteLine(".NET Framework 4.6");
    #endif
    #if NET461
      System.Console.WriteLine(".NET Framework 4.6.1");
    #endif
    #if NET461_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.6.1+");
    #endif
    #if NET462
      System.Console.WriteLine(".NET Framework 4.6.2");
    #endif
    #if NET462_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.6.2+");
    #endif
    #if NET46_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.6+");
    #endif
    #if NET47
      System.Console.WriteLine(".NET Framework 4.7");
    #endif
    #if NET471
      System.Console.WriteLine(".NET Framework 4.7.1");
    #endif
    #if NET471_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.7.1+");
    #endif
    #if NET472
      System.Console.WriteLine(".NET Framework 4.7.2");
    #endif
    #if NET472_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.7.2+");
    #endif
    #if NET47_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.7+");
    #endif
    #if NET48
      System.Console.WriteLine(".NET Framework 4.8");
    #endif
    #if NET481
      System.Console.WriteLine(".NET Framework 4.8.1");
    #endif
    #if NET48_OR_GREATER
      System.Console.WriteLine(".NET Framework 4.8+");
    #endif
    #if NETFRAMEWORK
      System.Console.WriteLine(".NET Framework");
    #endif

    #if NETSTANDARD
      System.Console.WriteLine(".NET Standard");
    #endif
    #if NETSTANDARD1_0
      System.Console.WriteLine(".NET Standard 1.0");
    #endif
    #if NETSTANDARD1_0_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.0+");
    #endif
    #if NETSTANDARD1_1
      System.Console.WriteLine(".NET Standard 1.1");
    #endif
    #if NETSTANDARD1_1_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.1+");
    #endif
    #if NETSTANDARD1_2
      System.Console.WriteLine(".NET Standard 1.2");
    #endif
    #if NETSTANDARD1_2_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.2+");
    #endif
    #if NETSTANDARD1_3
      System.Console.WriteLine(".NET Standard 1.3");
    #endif
    #if NETSTANDARD1_3_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.3+");
    #endif
    #if NETSTANDARD1_4
      System.Console.WriteLine(".NET Standard 1.4");
    #endif
    #if NETSTANDARD1_4_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.4+");
    #endif
    #if NETSTANDARD1_5
      System.Console.WriteLine(".NET Standard 1.5");
    #endif
    #if NETSTANDARD1_5_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.5+");
    #endif
    #if NETSTANDARD1_6
      System.Console.WriteLine(".NET Standard 1.6");
    #endif
    #if NETSTANDARD1_6_OR_GREATER
      System.Console.WriteLine(".NET Standard 1.6+");
    #endif
    #if NETSTANDARD2_0
      System.Console.WriteLine(".NET Standard 2.0");
    #endif
    #if NETSTANDARD2_0_OR_GREATER
      System.Console.WriteLine(".NET Standard 2.0+");
    #endif
    #if NETSTANDARD2_1
      System.Console.WriteLine(".NET Standard 2.1");
    #endif
    #if NETSTANDARD2_1_OR_GREATER
      System.Console.WriteLine(".NET Standard 2.1+");
    #endif

    #if NETCOREAPP
      System.Console.WriteLine(".NET Core");
    #endif
    #if NETCOREAPP1_0
      System.Console.WriteLine(".NET Core 1.0");
    #endif
    #if NETCOREAPP1_0_OR_GREATER
      System.Console.WriteLine(".NET Core 1.0+");
    #endif
    #if NETCOREAPP1_1
      System.Console.WriteLine(".NET Core 1.1");
    #endif
    #if NETCOREAPP1_1_OR_GREATER
      System.Console.WriteLine(".NET Core 1.1+");
    #endif
    #if NETCOREAPP2_0
      System.Console.WriteLine(".NET Core 2.0");
    #endif
    #if NETCOREAPP2_0_OR_GREATER
      System.Console.WriteLine(".NET Core 2.0+");
    #endif
    #if NETCOREAPP2_1
      System.Console.WriteLine(".NET Core 2.1");
    #endif
    #if NETCOREAPP2_1_OR_GREATER
      System.Console.WriteLine(".NET Core 2.1+");
    #endif
    #if NETCOREAPP2_2
      System.Console.WriteLine(".NET Core 2.2");
    #endif
    #if NETCOREAPP2_2_OR_GREATER
      System.Console.WriteLine(".NET Core 2.2+");
    #endif
    #if NETCOREAPP3_0
      System.Console.WriteLine(".NET Core 3.0");
    #endif
    #if NETCOREAPP3_0_OR_GREATER
      System.Console.WriteLine(".NET Core 3.0+");
    #endif
    #if NETCOREAPP3_1
      System.Console.WriteLine(".NET Core 3.1");
    #endif
    #if NETCOREAPP3_1_OR_GREATER
      System.Console.WriteLine(".NET Core 3.1+");
    #endif

    #if NET
      System.Console.WriteLine(".NET");
    #endif
    #if NET5_0
      System.Console.WriteLine(".NET 5.0");
    #endif
    #if NET5_0_OR_GREATER
      System.Console.WriteLine(".NET 5.0+");
    #endif
    #if NET6_0
      System.Console.WriteLine(".NET 6.0");
    #endif
    #if NET6_0_OR_GREATER
      System.Console.WriteLine(".NET 6.0+");
    #endif
    #if NET7_0
      System.Console.WriteLine(".NET 7.0");
    #endif
    #if NET7_0_OR_GREATER
      System.Console.WriteLine(".NET 7.0+");
    #endif
    #if NET8_0
      System.Console.WriteLine(".NET 8.0");
    #endif
    #if NET8_0_OR_GREATER
      System.Console.WriteLine(".NET 8.0+");
    #endif
    #if NET9_0
      System.Console.WriteLine(".NET 9.0");
    #endif
    #if NET9_0_OR_GREATER
      System.Console.WriteLine(".NET 9.0+");
    #endif

    // ...
    #if ANDROID21_0_OR_GREATER
      System.Console.WriteLine("Google Android 5.0+ (API level 21) operating system");
    #elif ANDROID22_0_OR_GREATER
      System.Console.WriteLine("Google Android 5.1+ (API level 22) operating system");
    #elif ANDROID23_0_OR_GREATER
      System.Console.WriteLine("Google Android 6.0+ (API level 23) operating system");
    #elif ANDROID24_0_OR_GREATER
      System.Console.WriteLine("Google Android 7.0+ (API level 24) operating system");
    #elif ANDROID25_0_OR_GREATER
      System.Console.WriteLine("Google Android 7.1+ (API level 25) operating system");
    #elif ANDROID26_0_OR_GREATER
      System.Console.WriteLine("Google Android 8.0+ (API level 26) operating system");
    #elif ANDROID27_0_OR_GREATER
      System.Console.WriteLine("Google Android 8.1+ (API level 27) operating system");
    #elif ANDROID28_0_OR_GREATER
      System.Console.WriteLine("Google Android 9.0+ (API level 28) operating system");
    #elif ANDROID29_0_OR_GREATER
      System.Console.WriteLine("Google Android 10.0+ (API level 29) operating system");
    #elif ANDROID30_0_OR_GREATER
      System.Console.WriteLine("Google Android 11.0+ (API level 30) operating system");
    #elif ANDROID31_0_OR_GREATER
      System.Console.WriteLine("Google Android 12.0+ (API level 31) operating system");
    #elif ANDROID32_0_OR_GREATER
      System.Console.WriteLine("Google Android 12L+ (API level 32) operating system");
    #elif ANDROID33_0_OR_GREATER
      System.Console.WriteLine("Google Android 13.0+ (API level 33) operating system");
    #elif ANDROID34_0_OR_GREATER
      System.Console.WriteLine("Google Android 14.0+ (API level 34) operating system");
    #elif ANDROID
      System.Console.WriteLine("Google Android operating system");
    #endif

    #if IOS13_0_OR_GREATER
      System.Console.WriteLine("Apple iOS 13.0+ operating system");
    #elif IOS14_0_OR_GREATER
      System.Console.WriteLine("Apple iOS 14.0+ operating system");
    #elif IOS15_0_OR_GREATER
      System.Console.WriteLine("Apple iOS 15.0+ operating system");
    #elif IOS15_1_OR_GREATER
      System.Console.WriteLine("Apple iOS 15.1+ operating system");
    #elif IOS16_0_OR_GREATER
      System.Console.WriteLine("Apple iOS 16.0+ operating system");
    #elif IOS17_0_OR_GREATER
      System.Console.WriteLine("Apple iOS 17.0+ operating system");
    #elif IOS
      System.Console.WriteLine("Apple iOS operating system");
    #elif MACOS10_15_OR_GREATER
      System.Console.WriteLine("Apple macOS 10.5+ (Catalina) operating system");
    #elif MACOS11_0_OR_GREATER
      System.Console.WriteLine("Apple macOS 11.0+ (Big Sur) operating system");
    #elif MACOS12_0_OR_GREATER
      System.Console.WriteLine("Apple macOS 12.0+ (Monterey) operating system");
    #elif MACOS13_0_OR_GREATER
      System.Console.WriteLine("Apple macOS 13.0+ (Ventura) operating system");
    #elif MACOS14_0_OR_GREATER
      System.Console.WriteLine("Apple macOS 14.0+ (Sonoma) operating system");
    #elif MACOS
      System.Console.WriteLine("Apple macOS operating system");
    #elif MACCATALYST13_0_OR_GREATER
      System.Console.WriteLine("Apple Mac Catalyst 13.0+ operating system");
    #elif MACCATALYST14_0_OR_GREATER
      System.Console.WriteLine("Apple Mac Catalyst 14.0+ operating system");
    #elif MACCATALYST15_0_OR_GREATER
      System.Console.WriteLine("Apple Mac Catalyst 15.0+ operating system");
    #elif MACCATALYST
      System.Console.WriteLine("Apple Mac Catalyst operating system");
    #elif TVOS13_0_OR_GREATER
      System.Console.WriteLine("Apple tvOS 13.0+ operating system");
    #elif TVOS14_0_OR_GREATER
      System.Console.WriteLine("Apple tvOS 14.0+ operating system");
    #elif TVOS15_0_OR_GREATER
      System.Console.WriteLine("Apple tvOS 15.0+ operating system");
    #elif TVOS
      System.Console.WriteLine("Apple tvOS operating system");
    #elif WATCHOS6_0_OR_GREATER
      System.Console.WriteLine("Apple watchOS 6.0+ operating system");
    #elif WATCHOS7_0_OR_GREATER
      System.Console.WriteLine("Apple watchOS 7.0+ operating system");
    #elif WATCHOS8_0_OR_GREATER
      System.Console.WriteLine("Apple watchOS 8.0+ operating system");
    #elif WATCHOS
      System.Console.WriteLine("Apple watchOS operating system");
    #endif

    #if WINDOWS7_0_OR_GREATER
      System.Console.WriteLine("Microsoft Windows 7 operating system");
    #elif WINDOWS8_0_OR_GREATER
      System.Console.WriteLine("Microsoft Windows 8 operating system");
    #elif WINDOWS10_0_OR_GREATER
      System.Console.WriteLine("Microsoft Windows 10 operating system");
    #elif WINDOWS11_0_OR_GREATER
      System.Console.WriteLine("Microsoft Windows 11 operating system");
    #elif WINDOWS
      System.Console.WriteLine("Microsoft Windows operating system");
    #endif

    // ...
    #if BROWSER
      System.Console.WriteLine("WebAssembly");
    #endif

    // ...
    #if ARM
      System.Console.WriteLine("ARM (Advanced RISC Machines) 32-bit processor architecture");
    #elif ARM64
      System.Console.WriteLine("ARM (Advanced RISC Machines) 64-bit processor architecture");
    #endif

    #if X64
      System.Console.WriteLine("Intel x64 processor architecture");
    #elif X86
      System.Console.WriteLine("Intel x86 processor architecture");
    #endif

    // ...
    System.Console.WriteLine(System.Runtime.InteropServices.RuntimeInformation.FrameworkDescription);
  }
}
