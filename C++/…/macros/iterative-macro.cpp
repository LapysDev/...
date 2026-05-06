#include <cstdio>

/* ... */
#define PREPROCESSOR_FORMAT PREPROCESSOR_FORMAT_CHECK(~, __MSVC__)
# define PREPROCESSOR_FORMAT_CHECK(...) PREPROCESSOR_FORMAT_SELECT(__VA_ARGS__)
# define PREPROCESSOR_FORMAT_SELECT(arguments, ...) defer(concatenate, defer(concatenate, PREPROCESSOR_, second(arguments, STANDARD, ~)), _FORMAT)
# define PREPROCESSOR_STANDARD_FORMAT 0
#   define PREPROCESSOR___MSVC___FORMAT 1

#define concatenate(argument1, argument2) argument1 ## argument2
#define defer(function, ...) defer_parse(function, (__VA_ARGS__))
# define defer_parse(function, call) function call
#define empty()
#define first(argument1, ...) argument1
#define parse(...) parse_12(__VA_ARGS__)
# define parse_1(...)  __VA_ARGS__
# define parse_2(...)  parse_1(parse_1(__VA_ARGS__))
# define parse_3(...)  parse_2(parse_2(__VA_ARGS__))
# define parse_4(...)  parse_3(parse_3(__VA_ARGS__))
# define parse_5(...)  parse_4(parse_4(__VA_ARGS__))
# define parse_6(...)  parse_5(parse_5(__VA_ARGS__))
# define parse_7(...)  parse_6(parse_6(__VA_ARGS__))
# define parse_8(...)  parse_7(parse_7(__VA_ARGS__))
# define parse_9(...)  parse_8(parse_8(__VA_ARGS__))
# define parse_10(...) parse_9(parse_9(__VA_ARGS__))
# define parse_11(...) parse_10(parse_10(__VA_ARGS__))
# define parse_12(...) parse_11(parse_11(__VA_ARGS__))
#define second(argument1, argument2, ...) argument2
#define stall(macro) stall_2(macro)
# define stall_1(macro) macro empty()
# define stall_2(macro) macro empty empty()()

#undef repeat
# define repeat_begin(function, separator, condition, argument, next, ...)                                       \
  defer(concatenate, repeat_condition_is_, condition(argument, next, __VA_ARGS__))(repeat_continue, repeat_end)( \
    function,                                                                                                    \
    defer(concatenate, repeat_condition_is_, condition(next, __VA_ARGS__))(separator, repeat_separator),         \
    stall(repeat_recurse)()(function, separator, condition, next, __VA_ARGS__, until),                           \
    argument, next, __VA_ARGS__                                                                                  \
  )
# undef  repeat_condition
#   define repeat_condition_accept(condition)   condition
#   define repeat_condition_fallback(condition) repeat_condition
#   define repeat_condition_fail() false
#   define repeat_condition_pass() true
#   define repeat_condition_is_false(truthy, falsy) falsy
#   define repeat_condition_is_true(truthy, falsy)  truthy
#     define repeat_condition_found_until       ~, fail
#     define repeat_condition_selection_default ~, fallback
# define repeat_continue(function, separator, repeater, argument, ...) function(argument) separator(argument, __VA_ARGS__) repeater
# define repeat_end(...)
# define repeat_function(argument) argument
#   define repeat_function_accept(function)   function
#   define repeat_function_fallback(function) repeat_function
#     define repeat_function_selection_default ~, fallback
# define repeat_separator(...)
#   define repeat_separator_accept(separator)   separator
#   define repeat_separator_fallback(separator) repeat_separator
#     define repeat_separator_selection_default ~, fallback
# undef  repeat_setup
# define repeat_recurse() repeat_begin
# if PREPROCESSOR_FORMAT == PREPROCESSOR_STANDARD_FORMAT
#   define repeat(...) parse(repeat_setup(__VA_ARGS__, until, until, until))
#     define repeat_condition(argument, ...) defer(concatenate, repeat_condition_, defer(second, repeat_condition_found_ ## argument, pass, ~))()
#     define repeat_setup(function, condition, separator, ...)                                                                  \
        repeat_begin(                                                                                                           \
          defer(concatenate, repeat_function_ , defer(second, repeat_function_selection_  ## function , accept, ~))(function ), \
          defer(concatenate, repeat_separator_, defer(second, repeat_separator_selection_ ## separator, accept, ~))(separator), \
          defer(concatenate, repeat_condition_, defer(second, repeat_condition_selection_ ## condition, accept, ~))(condition), \
          __VA_ARGS__                                                                                                           \
        )
# elif PREPROCESSOR_FORMAT == PREPROCESSOR___MSVC___FORMAT
#   define repeat(...) parse(defer(repeat_setup, __VA_ARGS__))
#     define repeat_condition(argument, ...) defer(concatenate, repeat_condition_, stall(defer(second, first(repeat_condition_found_ ## argument, ~), pass)))()
#     define repeat_setup(function, condition, separator, ...)                                                                                   \
        defer(repeat_begin,                                                                                                                      \
          stall(defer(concatenate, repeat_function_ , defer(second, first(repeat_function_selection_  ## function , ~), accept, ~))(function )), \
          stall(defer(concatenate, repeat_separator_, defer(second, first(repeat_separator_selection_ ## separator, ~), accept, ~))(separator)), \
          stall(defer(concatenate, repeat_condition_, defer(second, first(repeat_condition_selection_ ## condition, ~), accept, ~))(condition)), \
          __VA_ARGS__, until, until                                                                                                              \
        )
# endif

/* Main */
#define add(...) +
#define pass(integer) integer
#define stringify(argument) # argument

int main() {
  std::printf("[0x00]: %u" "\r\n", repeat(pass, default, add) 0u);
  std::printf("[0x01]: %u" "\r\n", repeat(pass, default, add, 1u));
  std::printf("[0x02]: %u" "\r\n", repeat(pass, default, add, 1u, 1u));
  std::printf("[0x03]: %u" "\r\n", repeat(pass, default, add, 1u, 1u, 1u));
  std::printf("[0XFF]: %u" "\r\n", repeat(pass, default, add, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u));
  std::puts(repeat(stringify, default, default, Hello, _, World));
}
