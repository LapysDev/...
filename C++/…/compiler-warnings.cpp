union bounds {
  RECT bounds;
  #if defined __ECC || defined __ICC || defined __ICL || defined __INTEL_COMPILER || defined __INTEL_COMPILER_BUILD_DATE || defined __INTEL_LLVM_COMPILER
  # pragma warning(push)
  # pragma warning(disable: 161)
  # pragma clang diagnostic push
  # pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
  #elif defined __CUDACC_VER_BUILD__ || defined __CUDACC_VER_MAJOR__ || defined __CUDACC_VER_MINOR__ || defined __NVCC__ || defined __NVCOMPILER
  # pragma diag_suppress nonstd_unnamed_member
  #elif defined __clang__
  # pragma clang diagnostic push
  # pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
  #elif defined __GNUC__
  # pragma GCC diagnostic push
  # pragma GCC diagnostic ignored "-Wpedantic"
  #elif defined _MSC_VER
  # pragma warning(push)
  # pragma warning(disable: 4201)
  #endif
    struct /* final */ { POINT minimum, maximum; };
  #if defined __ECC || defined __ICC || defined __ICL || defined __INTEL_COMPILER || defined __INTEL_COMPILER_BUILD_DATE || defined __INTEL_LLVM_COMPILER
  # pragma clang diagnostic pop
  # pragma warning(pop)
  #elif defined __CUDACC_VER_BUILD__ || defined __CUDACC_VER_MAJOR__ || defined __CUDACC_VER_MINOR__ || defined __NVCC__ || defined __NVCOMPILER
  #elif defined __clang__
  # pragma clang diagnostic pop
  #elif defined __GNUC__
  # pragma GCC diagnostic pop
  #elif defined _MSC_VER
  # pragma warning(pop)
  #endif
};
