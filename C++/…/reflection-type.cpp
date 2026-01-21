#include <ciso646>
#include <cstddef>
#include <new>
#if defined _MSVC_LANG
# if _MSVC_LANG +0 >= 202002L
#   include <version>
# endif
#elif __cplusplus >= 202002L or defined __apple_build_version__ or defined __clang__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__ or defined __CUDACC_VER_BUILD__ or defined __CUDACC_VER_MAJOR__ or defined __CUDACC_VER_MINOR__ or defined __ECC or defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__ or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER or defined __NVCC__ or defined __NVCOMPILER or defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER
# include <version>
#endif
#define private public

/* ... */
struct reflect /* final */ {
  enum /* : unsigned char */ { c      = 0x1u, v         = 0x2u };                               // --> reflinfo::cv
  enum /* : unsigned char */ { copy   = 0x1u, construct = 0x2u, destruct = 0x4u, move = 0x8u }; // --> reflinfo::trivial
  enum /* : unsigned char */ { lvalue = 0x1u, rvalue    = 0x2u };                               // --> reflinfo::reference

  union _ {
    template <typename, bool>
    friend union reflinfo;

    private:
      static void  operator delete(void*);
      static void* operator new   (std::size_t);
      static void* operator new   (std::size_t, _);
      #ifdef __cpp_rvalue_references // --> 200610L
        static void* operator new(std::size_t, _&);
      #endif
  }; // ->> See `reflproxy::operator []`

  struct proxy /* final */ {
    bool (*add)                       (void*, void const*, void**); // -->   x   + y
    bool (*address)                   (void*,              void**); // -->  &x
    bool (*assign)                    (void*, void const*, void**); // -->   x   = y
    bool (*assign_add)                (void*, void const*, void**); // -->   x  += y
    bool (*assign_bitwise_and)        (void*, void const*, void**); // -->   x  &= y
    bool (*assign_bitwise_or)         (void*, void const*, void**); // -->   x  |= y
    bool (*assign_bitwise_shift_left) (void*, void const*, void**); // -->   x <<= y
    bool (*assign_bitwise_shift_right)(void*, void const*, void**); // -->   x >>= y
    bool (*assign_bitwise_xor)        (void*, void const*, void**); // -->   x  ^= y
    bool (*assign_divide)             (void*, void const*, void**); // -->   x  /= y
    bool (*assign_modulo)             (void*, void const*, void**); // -->   x  %= y
    bool (*assign_multiply)           (void*, void const*, void**); // -->   x  *= y
    bool (*assign_subtract)           (void*, void const*, void**); // -->   x  -= y
    bool (*bitwise_and)               (void*, void const*, void**); // -->   x   & y
    bool (*bitwise_or)                (void*, void const*, void**); // -->   x   | y
    bool (*bitwise_shift_left)        (void*, void const*, void**); // -->   x  << y
    bool (*bitwise_shift_right)       (void*, void const*, void**); // -->   x  >> y
    bool (*bitwise_xor)               (void*, void const*, void**); // -->   x   ^ y
    bool (*boolean_and)               (void*, void const*, void**); // -->   x  && y
    bool (*boolean_or)                (void*, void const*, void**); // -->   x  || y
    bool (*comma)                     (void*, void const*, void**); // -->   x   , y
    bool (*compare)                   (void*, void const*, void**); // -->   x <=> y
    bool (*complement)                (void*,              void**); // -->  ~x
    bool (*dereference)               (void*,              void**); // -->  *x
    bool (*divide)                    (void*, void const*, void**); // -->   x   / y
    bool (*equals)                    (void*, void const*, void**); // -->   x  == y
    bool (*greater)                   (void*, void const*, void**); // -->   x   > y
    bool (*greater_equals)            (void*, void const*, void**); // -->   x  >= y
    bool (*lesser)                    (void*, void const*, void**); // -->   x   < y
    bool (*lesser_equals)             (void*, void const*, void**); // -->   x  <= y
    bool (*minus)                     (void*,              void**); // -->  -x
    bool (*modulo)                    (void*, void const*, void**); // -->   x   % y
    bool (*multiply)                  (void*, void const*, void**); // -->   x   * y
    bool (*negate)                    (void*,              void**); // -->  !x
    bool (*plus)                      (void*,              void**); // -->  +x
    bool (*post_decrement)            (void*,              void**); // -->   x--
    bool (*post_increment)            (void*,              void**); // -->   x++
    bool (*pre_decrement)             (void*,              void**); // --> --x
    bool (*pre_increment)             (void*,              void**); // --> ++x
    bool (*subtract)                  (void*, void const*, void**); // -->   x   - y
    bool (*unequals)                  (void*, void const*, void**); // -->   x  != y

    bool (*operator [](void (&operation)(_))    const volatile)(void*,              void**);
    bool (*operator [](void (&operation)(_, _)) const volatile)(void*, void const*, void**);
  };

  /* ... */
  proxy                              proxy;                              //
  reflect                          (*baseof)          ();                //
  void*                            (*constructor)     (void*,      ...); // ->> Placement `new`
  void*                            (*copy_constructor)(void*, void*...); //
  void                             (*destructor)      (void*,      ...); // ->> Placement `delete`
  void                             (*id)              ();                //
  void*                            (*move_constructor)(void*, void*...); //
  std::size_t /* std::align_val_t */ alignment;                          // --> alignof base
  std::size_t                        size;                               // --> sizeof base
  std::size_t                        length;                             // ->> Zero for unbounded arrays, non-zero for bounded arrays
  unsigned char                      trivial   : 4;                      //
  unsigned char                      cv        : 2;                      //
  unsigned char                      reference : 2;                      //
  bool                               array     : 1;                      // ->> Falsy for pointer-/ reference-to-arrays
  bool                               except    : 1;                      //
  bool                               function  : 1;                      // ->> Falsy for pointer-/ reference-to-functions
  bool                               member    : 1;                      //
  bool                               pointer   : 1;                      //

  // ->> `refl(…)` for non-`void` types (2nd step)
  template <typename type> operator type               &() { return static_cast<type               &>(*(type*) static_cast<void*>(this)); }
  template <typename type> operator type const         &() { return static_cast<type const         &>(*(type*) static_cast<void*>(this)); }
  template <typename type> operator type const volatile&() { return static_cast<type const volatile&>(*(type*) static_cast<void*>(this)); }
  template <typename type> operator type       volatile&() { return static_cast<type       volatile&>(*(type*) static_cast<void*>(this)); }
  #ifdef __cpp_rvalue_references // --> 200610L
    template <typename type> operator type               &&() { return static_cast<type               &&>(*(type*) static_cast<void*>(this)); }
    template <typename type> operator type const         &&() { return static_cast<type const         &&>(*(type*) static_cast<void*>(this)); }
    template <typename type> operator type const volatile&&() { return static_cast<type const volatile&&>(*(type*) static_cast<void*>(this)); }
    template <typename type> operator type       volatile&&() { return static_cast<type       volatile&&>(*(type*) static_cast<void*>(this)); }
  #endif
};
  void operator   !(reflect::_)             {}
  void operator   %(reflect::_, reflect::_) {}
  void operator   &(reflect::_)             {}
  void operator   &(reflect::_, reflect::_) {}
  void operator   *(reflect::_)             {}
  void operator   *(reflect::_, reflect::_) {}
  void operator   +(reflect::_)             {}
  void operator   +(reflect::_, reflect::_) {}
  void operator   ,(reflect::_, reflect::_) {}
  void operator   -(reflect::_)             {}
  void operator   -(reflect::_, reflect::_) {}
  void operator   /(reflect::_, reflect::_) {}
  void operator   <(reflect::_, reflect::_) {}
  void operator   >(reflect::_, reflect::_) {}
  void operator   ^(reflect::_, reflect::_) {}
  void operator   |(reflect::_, reflect::_) {}
  void operator   ~(reflect::_)             {}
  void operator  !=(reflect::_, reflect::_) {}
  void operator  %=(reflect::_, reflect::_) {}
  void operator  &&(reflect::_, reflect::_) {}
  void operator  &=(reflect::_, reflect::_) {}
  void operator  *=(reflect::_, reflect::_) {}
  void operator  ++(reflect::_)             {}
  void operator  ++(reflect::_, int)        {}
  void operator  +=(reflect::_, reflect::_) {}
  void operator  --(reflect::_)             {}
  void operator  --(reflect::_, int)        {}
  void operator  -=(reflect::_, reflect::_) {}
  void operator  /=(reflect::_, reflect::_) {}
  void operator  <<(reflect::_, reflect::_) {}
  void operator  <=(reflect::_, reflect::_) {}
  void operator  ==(reflect::_, reflect::_) {}
  void operator  >=(reflect::_, reflect::_) {}
  void operator  >>(reflect::_, reflect::_) {}
  void operator  ^=(reflect::_, reflect::_) {}
  void operator  |=(reflect::_, reflect::_) {}
  void operator  ||(reflect::_, reflect::_) {}
  void operator <<=(reflect::_, reflect::_) {}
  void operator >>=(reflect::_, reflect::_) {}
  #if defined __cpp_impl_three_way_comparison // --> 201907L
    void operator <=>(reflect::_, reflect::_) {}
  #endif

template <typename base, bool = true>
union reflinfo {
  typedef base       type;
  typedef reflect::_ _;

  private:
    /* ... */
    template <typename subbase>
    union cast {
      void *const value;

      cast(void* const value = NULL) : value(value) {}
      subbase operator *() { return *(subbase*) this -> value; }
    };

    template <typename subbase>
    union cast<subbase&> {
      void *const value;

      cast(void* const value = NULL) : value(value) {}
      subbase& operator *() { return *(subbase*) this -> value; }
    };

    #ifdef __cpp_rvalue_references // --> 200610L
      template <typename subbase>
      union cast<subbase&&> {
        void *const value;

        cast(void* const value = NULL) : value(value) {}
        subbase&& operator *() { return static_cast<subbase&&>(*(subbase*) this -> value); }
      };
    #endif

    // ...
    template <typename op, op& operation, typename subbase = base>
    union can {
      template <typename>      static bool (&valueof(...))                                                             [false                                                                           + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator      !, subbase>*))[sizeof(static_cast<void> (!*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator      &, subbase>*))[sizeof(static_cast<void> (&*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator      *, subbase>*))[sizeof(static_cast<void> (**cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator      +, subbase>*))[sizeof(static_cast<void> (+*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator      -, subbase>*))[sizeof(static_cast<void> (-*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator      ~, subbase>*))[sizeof(static_cast<void> (~*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator     ++, subbase>*))[sizeof(static_cast<void>(++*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_),                        operator     --, subbase>*))[sizeof(static_cast<void>(--*cast<type> ()),                                 00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      %, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   % *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      &, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   & *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      *, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   * *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      +, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   + *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      ,, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   , *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      -, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   - *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      /, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   / *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      <, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   < *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      >, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   > *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      ^, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   ^ *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator      |, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()   | *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     !=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  != *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     %=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  %= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     &&, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  && *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     &=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  &= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     *=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  *= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     +=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  += *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     -=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  -= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     /=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  /= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     <<, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  << *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     <=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  <= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     ==, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  == *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     >=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  >= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     >>, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  >> *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     ^=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  ^= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     |=, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  |= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator     ||, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()  || *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator    <<=, subbase>*))[sizeof(static_cast<void>  (*cast<type> () <<= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, _),                     operator    >>=, subbase>*))[sizeof(static_cast<void>  (*cast<type> () >>= *cast<type const>()),         00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, int),                   operator     ++, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()++),                               00) + 1];
      template <typename type> static bool (&valueof(can<void  (_, int),                   operator     --, subbase>*))[sizeof(static_cast<void>  (*cast<type> ()--),                               00) + 1];
      template <typename type> static bool (&valueof(can<void  (void*),                 _::operator delete, subbase>*))[sizeof(                   (*cast<type> ())    .~type(),                     00) + 1];
      template <typename type> static bool (&valueof(can<void* (std::size_t),           _::operator    new, subbase>*))[sizeof       (::new (static_cast<void*>(NULL))  type(),                     00) + 1];
      template <typename type> static bool (&valueof(can<void* (std::size_t, _ const&), _::operator    new, subbase>*))[sizeof       (::new (static_cast<void*>(NULL))  type(*cast<type const&>()), 00) + 1];
      #if defined __cpp_impl_three_way_comparison // --> 201907L
        template <typename type> static bool (&valueof(can<void (_, _), operator <=>, subbase>*))[sizeof(static_cast<void>(*cast<type>() <=> *cast<type const>()), 00) + 1];
      #endif
      #ifdef __cpp_rvalue_references // --> 200610L
        template <typename type> static bool (&valueof(can<void* (std::size_t, _&&), _::operator new, subbase>*))[sizeof(::new (static_cast<void*>(NULL)) type(*cast<type&&>()), 00) + 1];
      #endif

      enum /* : bool */ {
        value = sizeof(bool[false + 1]) != sizeof valueof<subbase>(static_cast<can<op, operation, subbase>*>(NULL))
      };
    };

    // ...
    struct subreflect /* final */ {
      template <typename subbase, bool = can<void* (std::size_t, _ const&), _::operator new, subbase>::value>
      struct evaluation {
        static bool value(void** const, void const* const...) {
          return false;
        }
      };

      template <typename subbase>
      struct evaluation<subbase, true> {
        static bool value(void** const address, void const* const value...) {
          return *address == ::new (*address) subbase(*static_cast<subbase const*>(value));
        }
      };

      template <typename subbase>
      struct evaluation<subbase&, false> {
        static bool value(void** const address, void const* const value...) {
          *address = const_cast<void*>(value);
          return true;
        }
      };

      #ifdef __cpp_rvalue_references // --> 200610L
        template <typename subbase>
        struct evaluation<subbase&&, false> : public evaluation<subbase&, false> {};
      #endif

      /* ... */
      bool (*evaluator)(void**, void const*...); // ->> Function to copy the reflected expression value to an addressed pointer
      void  *value;                              // ->> Reflected expression value; Null for `void` expressions

      /* ... */
      subreflect() :
        evaluator(NULL),
        value    (NULL)
      {}

      /* ... */
      bool evaluate(void** const address) {
        return NULL == address or NULL == this -> value or this -> evaluator(address, this -> value);
      }

      // ...
      subreflect operator ,(subreflect const) {
        return *this;
      }

      template <typename type>
      subreflect operator ,(type& value) {
        this -> evaluator = &evaluation<type>::value;
        this -> value     = &const_cast<unsigned char&>(reinterpret_cast<unsigned char const volatile&>(value));

        return *this;
      }

      #ifdef __cpp_rvalue_references // --> 200610L
        template <typename type>
        subreflect operator ,(type&& value) {
          return this -> operator ,(static_cast<type&>(value));
        }
      #endif
    };

    // ...
    template <typename op, op& operation, typename subbase = base, bool = not can<op, operation, subbase>::value>
    union subproxy {
      union {
        template <typename type> // ->> `bool (void*, void*)` or `bool (void*, void const*, void*)`
        #ifdef __cpp_constexpr   // --> 200704L
          constexpr
        #endif
        operator type*() const { return NULL; }
      } value;
    };

    template <typename subbase>
    union subproxy<void (_, _), operator +, subbase, true> {
      static bool value(void* const address, void const* const operand, void** const result = NULL) {
        // TODO: always a reference to `subreflect`, so use decltype to deduce for `reflect` and `subreflect`
        return NULL != address and NULL != operand and (subreflect(), (*cast<subbase>(address) + *cast<subbase const>(operand)), subreflect()).evaluate(result);
      }
    };
    struct A {};
    struct B : A {};
      template <typename T> void operator ,(A const&, T) { std::puts("noref"); }

      template <typename T> void operator ,(B, T&)                { std::puts("lvalue ref"); }

    // ...
    // template <typename subbase = base>
    // struct subreflinfo {
    //   static reflect baseof          ()                                               { return reflinfo<void, false>(); }
    //   static void*   constructor     (void* const address, ...)                       { return ::new (address) subbase(); }
    //   static void    copy_constructor(void* const address, void const* const copy...) { return ::new (address) subbase(static_cast<subbase const&>(*static_cast<subbase const*>(copy))); }
    //   static void    destructor      (void* const address, ...)                       { address -> ~subbase(); }
    //   static void    id              ()                                               { /* … */ }
    //   static void    move_constructor(void* const address, void const* const move...) { return ::new (address) subbase(static_cast<subbase&&>(*static_cast<subbase const*>(move))); }

    //   static bool          const array  = false;
    //   static unsigned char const cv     = 0x00u;
    //   static std::size_t   const length = 1u;
    //   static std::size_t   const size   = sizeof(subbase);
    //   #if __cplusplus >= 201103L // ->> `__alignof_is_defined` from `<cstdalign>` or `<stdalign.h>` headers
    //     static std::size_t const alignment = alignof(subbase);
    //   #elif defined __apple_build_version__ or defined __clang__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__
    //   # if defined __has_extension
    //   #   if __has_extension(cxx_alignof)
    //         static std::size_t const alignment = alignof(subbase);
    //   #   elif __has_extension(c_alignof)
    //         static std::size_t const alignment = _Alignof(subbase);
    //   #   else
    //         static std::size_t const alignment = 0u;
    //   #   endif
    //   # elif defined __has_feature
    //   #   if __has_feature(cxx_alignof)
    //         static std::size_t const alignment = alignof(subbase);
    //   #   elif __has_feature(c_alignof)
    //         static std::size_t const alignment = _Alignof(subbase);
    //   #   else
    //         static std::size_t const alignment = 0u;
    //   #   endif
    //   # endif
    //   #elif defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__
    //     static std::size_t const alignment = __alignof__(subbase);
    //   #elif defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER
    //     static std::size_t const alignment = __alignof(subbase);
    //   #else
    //     static std::size_t const alignment = 0u;
    //   #endif
    // };
    //   template <typename subbase>
    //   struct subreflinfo<subbase*> : public subreflinfo<subbase> {
    //     static reflect baseof() { return reflid(subbase); }
    //   };

    //   template <typename subbase>
    //   struct subreflinfo<subbase* const> : public subreflinfo<subbase> {
    //     static reflect baseof() { return reflid(subbase); }
    //     static unsigned char const cv = reflect::c;
    //   };

    //   template <typename subbase>
    //   struct subreflinfo<subbase* const volatile> : public subreflinfo<subbase> {
    //     static reflect baseof() { return reflid(subbase); }
    //     static unsigned char const cv = reflect::c | reflect::v;
    //   };

    //   template <typename subbase>
    //   struct subreflinfo<subbase* volatile> : public subreflinfo<subbase> {
    //     static reflect baseof() { return reflid(subbase); }
    //     static unsigned char const cv = reflect::v;
    //   };

    //   template <typename subbase>
    //   struct subreflinfo<subbase&> : public subreflinfo<subbase> {
    //     static reflect baseof() { return reflid(subbase); }

    //     nullfunc                   constructor;
    //     nullfunc                   copy_constructor;
    //     nullfunc                   destructor;
    //     nullfunc                   move_constructor;
    //     static unsigned char const reference = reflect::lvalue;
    //   };

    //   #ifdef __cpp_rvalue_references // --> 200610L
    //     template <typename subbase>
    //     struct subreflinfo<subbase&&> : public subreflinfo<subbase> {
    //       static reflect baseof() { return reflid(subbase); }

    //       nullfunc                   constructor;
    //       nullfunc                   copy_constructor;
    //       nullfunc                   destructor;
    //       nullfunc                   move_constructor;
    //       static unsigned char const reference = reflect::rvalue;
    //     };
    //   #endif

    //   template <typename subbase>
    //   struct subreflinfo<subbase[]> : public subreflinfo<subbase> {
    //     static reflect baseof() { return reflid(subbase); }

    //     static bool        const array = true;
    //     nullfunc                 constructor;
    //     nullfunc                 copy_constructor;
    //     nullfunc                 destructor;
    //     static std::size_t const length = 0u;
    //     nullfunc                 move_constructor;
    //   };

    //   template <typename subbase, std::size_t capacity>
    //   struct subreflinfo<subbase[capacity]> : public subreflinfo<subbase[]> {
    //     static reflect baseof() { return reflid(subbase); }
    //     static std::size_t const length = capacity;
    //   };

  public:
    /* constexpr */ operator reflect() const {
      reflect reflection = {
        // {
        //   subproxy<void (_, _), operator +>().value, // --> reflproxy::add
        //   NULL, // --> reflproxy::address
        //   NULL, // --> reflproxy::assign
        //   NULL, // --> reflproxy::assign_add
        //   NULL, // --> reflproxy::assign_bitwise_and
        //   NULL, // --> reflproxy::assign_bitwise_or
        //   NULL, // --> reflproxy::assign_bitwise_shift_left
        //   NULL, // --> reflproxy::assign_bitwise_shift_right
        //   NULL, // --> reflproxy::assign_bitwise_xor
        //   NULL, // --> reflproxy::assign_divide
        //   NULL, // --> reflproxy::assign_modulo
        //   NULL, // --> reflproxy::assign_multiply
        //   NULL, // --> reflproxy::assign_subtract
        //   NULL, // --> reflproxy::bitwise_and
        //   NULL, // --> reflproxy::bitwise_or
        //   NULL, // --> reflproxy::bitwise_shift_left
        //   NULL, // --> reflproxy::bitwise_shift_right
        //   NULL, // --> reflproxy::bitwise_xor
        //   NULL, // --> reflproxy::boolean_and
        //   NULL, // --> reflproxy::boolean_or
        //   NULL, // --> reflproxy::comma
        //   NULL, // --> reflproxy::compare
        //   NULL, // --> reflproxy::complement
        //   NULL, // --> reflproxy::dereference
        //   NULL, // --> reflproxy::divide
        //   NULL, // --> reflproxy::equals
        //   NULL, // --> reflproxy::greater
        //   NULL, // --> reflproxy::greater_equals
        //   NULL, // --> reflproxy::lesser
        //   NULL, // --> reflproxy::lesser_equals
        //   NULL, // --> reflproxy::minus
        //   NULL, // --> reflproxy::modulo
        //   NULL, // --> reflproxy::multiply
        //   NULL, // --> reflproxy::negate
        //   NULL, // --> reflproxy::plus
        //   NULL, // --> reflproxy::post_decrement
        //   NULL, // --> reflproxy::post_increment
        //   NULL, // --> reflproxy::pre_decrement
        //   NULL, // --> reflproxy::pre_increment
        //   NULL, // --> reflproxy::subtract
        //   NULL  // --> reflproxy::unequals
        // },
        // subreflinfo<>().baseof,           // --> reflect::baseof
        // subreflinfo<>().constructor,      // --> reflect::constructor
        // subreflinfo<>().copy_constructor, // --> reflect::copy_constructor
        // subreflinfo<>().destructor,       // --> reflect::destructor
        // subreflinfo<>().id,               // --> reflect::id
        // subreflinfo<>().move_constructor, // --> reflect::move_constructor
        // subreflinfo<>().alignment,        // --> reflect::alignment
        // subreflinfo<>().size,             // --> reflect::size
        // subreflinfo<>().length,           // --> reflect::length
        // 0x00u,                            // --> reflect::trivial
        // subreflinfo<>().cv,               // --> reflect::cv
        // subreflinfo<>().reference,        // --> reflect::reference
        // subreflinfo<>().array,            // --> reflect::array
        // false,                            // --> reflect::except
        // false,                            // --> reflect::function
        // false,                            // --> reflect::member
        // false                             // --> reflect::pointer
      };

      // reflproxy proxy = refl(int).proxy;
      // → proxy[operator +=](&A{}, &A{}, &…)
      // → B operator +(A, A)
      // → reflexpr(sus = *(A*) … + *(A const*) …).constructor(&…, &sus)
      return reflection;
    }
};

template <>
union reflinfo<void, false> {
  private:
    union subreflinfo {
      static reflect baseof() { return reflinfo<void, false>(); }
    };

  public:
    operator reflect() const {
      reflect reflection = {
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        &subreflinfo::baseof,             // --> reflect::baseof
        NULL, NULL, NULL, NULL, NULL,     // --> reflect::constructor | copy_constructor | destructor | id | move_constructor
        0u, 0u, 0u,                       // --> reflect::alignment | size | length
        0x00u, 0x00u, 0x00u,              // --> reflect::trivial | cv | reference
        false, false, false, false, false // --> reflect::array | except | function | member | pointer
      };

      return reflection;
    }
};
  // ->> `refl(…)` for non-`void` types (1st step)
  reflect operator +(reflect);

  // ->> `refl(…)` for non-`void` types (3rd step)
  #ifdef __cpp_rvalue_references // --> 200610L
    template <typename type>
    reflinfo<type&&> operator ,(reflect, type&&) {
      return reflinfo<type&&>();
    }
  #else
    template <typename type> reflinfo<type               &> operator ,(reflect, type               &) { return reflinfo<type               &>(); }
    template <typename type> reflinfo<type const         &> operator ,(reflect, type const         &) { return reflinfo<type const         &>(); }
    template <typename type> reflinfo<type const volatile&> operator ,(reflect, type const volatile&) { return reflinfo<type const volatile&>(); }
    template <typename type> reflinfo<type       volatile&> operator ,(reflect, type       volatile&) { return reflinfo<type       volatile&>(); }
  #endif

  // ->> `refl(…)` for non-`void` types (4th step)
  template <typename type>
  reflinfo<type> operator ,(reflinfo<type>, reflinfo<void>) {
    return reflinfo<type>();
  }

  // ->> `refl(…)` for expressions (1st step)
  #ifdef __cpp_rvalue_references // --> 200610L
    template <typename type>
    reflinfo<type&&> operator +(type&&, reflect);
  #else
    template <typename type> reflinfo<type               &> operator +(type               &, reflect);
    template <typename type> reflinfo<type const         &> operator +(type const         &, reflect);
    template <typename type> reflinfo<type const volatile&> operator +(type const volatile&, reflect);
    template <typename type> reflinfo<type       volatile&> operator +(type       volatile&, reflect);
  #endif

  // ->> `refl(…)` for expressions (2nd step)
  template <typename type>
  reflinfo<type> operator ,(reflect, reflinfo<type>);

typedef struct reflect::proxy reflproxy;
  bool (*reflproxy::operator [](void (&operation)(reflect::_)) const volatile)(void*, void**) {
    if (operation == static_cast<void (&)(reflect::_)>(operator  !)) return this -> negate;
    if (operation == static_cast<void (&)(reflect::_)>(operator  &)) return this -> address;
    if (operation == static_cast<void (&)(reflect::_)>(operator  *)) return this -> dereference;
    if (operation == static_cast<void (&)(reflect::_)>(operator  +)) return this -> plus;
    if (operation == static_cast<void (&)(reflect::_)>(operator  -)) return this -> minus;
    if (operation == static_cast<void (&)(reflect::_)>(operator  ~)) return this -> complement;
    if (operation == static_cast<void (&)(reflect::_)>(operator ++)) return this -> pre_increment;
    if (operation == static_cast<void (&)(reflect::_)>(operator --)) return this -> pre_decrement;

    return NULL;
  }

  bool (*reflproxy::operator [](void (&operation)(reflect::_, reflect::_)) const volatile)(void*, void const*, void**) {
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   %)) return this -> modulo;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   &)) return this -> bitwise_and;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   *)) return this -> multiply;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   +)) return this -> add;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   ,)) return this -> comma;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   -)) return this -> subtract;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   /)) return this -> divide;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   <)) return this -> lesser;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   >)) return this -> greater;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   ^)) return this -> bitwise_xor;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator   |)) return this -> bitwise_or;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  !=)) return this -> unequals;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  %=)) return this -> assign_modulo;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  &&)) return this -> boolean_and;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  &=)) return this -> assign_bitwise_and;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  *=)) return this -> assign_multiply;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  +=)) return this -> assign_add;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  -=)) return this -> assign_subtract;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  /=)) return this -> assign_divide;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  <<)) return this -> bitwise_shift_left;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  <=)) return this -> bitwise_shift_right;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  ==)) return this -> equals;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  >=)) return this -> greater_equals;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  >>)) return this -> bitwise_shift_right;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  ^=)) return this -> assign_bitwise_xor;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  |=)) return this -> assign_bitwise_or;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator  ||)) return this -> boolean_or;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator <<=)) return this -> assign_bitwise_shift_left;
    if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator >>=)) return this -> assign_bitwise_shift_right;
    #ifdef __cpp_impl_three_way_comparison // --> 201907L
      if (operation == static_cast<void (&)(reflect::_, reflect::_)>(operator <=>)) return this -> compare;
    #endif

    return NULL;
  }

#if defined _MSC_VER or /* --> 200707L */ defined __cpp_decltype
# define     refl(value) decltype(reflect(), (value) +reflect(), reflinfo<void>())().operator reflect() //
# define reflexpr(value) reflinfo<decltype   (value)>                             ().operator reflect() // decltype(reflect(), (value),            reflinfo<void>())().operator reflect()
# define   reflid(value) reflinfo            <value>                              ().operator reflect() // decltype(reflect(), (value)  reflect(), reflinfo<void>())().operator reflect()
#elif defined __GNUC__
# define     refl(value) __decltype(reflect(), (value) +reflect(), reflinfo<void>())().operator reflect() //
# define reflexpr(value) reflinfo<__decltype   (value)>                             ().operator reflect() // __decltype(reflect(), (value),            reflinfo<void>())().operator reflect()
# define   reflid(value) reflinfo              <value>                              ().operator reflect() // __decltype(reflect(), (value)  reflect(), reflinfo<void>())().operator reflect()
#elif defined __clang__
# define     refl(value) typename reflinfo<__typeof__(reflect(), (value) +reflect(), reflinfo<void>())>::type().operator reflect() //
# define reflexpr(value) reflinfo         <__typeof__            (value)>                                    ().operator reflect() // typename reflinfo<__typeof__(reflect(), (value),            reflinfo<void>())>::type().operator reflect()
# define   reflid(value) reflinfo                                <value>                                     ().operator reflect() // typename reflinfo<__typeof__(reflect(), (value)  reflect(), reflinfo<void>())>::type().operator reflect()
#else
# define     refl(value) reflinfo<void, false>().operator reflect() //
# define reflexpr(value) reflinfo<void, false>().operator reflect() //
# define   reflid(value) reflinfo<value>      ().operator reflect() // (reflect(), (value) reflect(), reflinfo<void>()).operator reflect()
#endif

/* Main */
int main(int, char*[]) /* noexcept */ {}
