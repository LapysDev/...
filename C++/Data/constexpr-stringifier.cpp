#include <cstdio>

/* ... */
#define arity(count) arity_ ## count()
# define arity_0()
# define arity_1()   1
# define arity_2()   arity_1()   , 2
# define arity_3()   arity_2()   , 3
# define arity_4()   arity_3()   , 4
# define arity_5()   arity_4()   , 5
# define arity_6()   arity_5()   , 6
# define arity_7()   arity_6()   , 7
# define arity_8()   arity_7()   , 8
# define arity_9()   arity_8()   , 9
# define arity_10()  arity_9()   , 10
# define arity_11()  arity_10()  , 11
# define arity_12()  arity_11()  , 12
# define arity_13()  arity_12()  , 13
# define arity_14()  arity_13()  , 14
# define arity_15()  arity_14()  , 15
# define arity_16()  arity_15()  , 16
# define arity_17()  arity_16()  , 17
# define arity_18()  arity_17()  , 18
# define arity_19()  arity_18()  , 19
# define arity_20()  arity_19()  , 20
# define arity_21()  arity_20()  , 21
# define arity_22()  arity_21()  , 22
# define arity_23()  arity_22()  , 23
# define arity_24()  arity_23()  , 24
# define arity_25()  arity_24()  , 25
# define arity_26()  arity_25()  , 26
# define arity_27()  arity_26()  , 27
# define arity_28()  arity_27()  , 28
# define arity_29()  arity_28()  , 29
# define arity_30()  arity_29()  , 30
# define arity_31()  arity_30()  , 31
# define arity_32()  arity_31()  , 32
# define arity_33()  arity_32()  , 33
# define arity_34()  arity_33()  , 34
# define arity_35()  arity_34()  , 35
# define arity_36()  arity_35()  , 36
# define arity_37()  arity_36()  , 37
# define arity_38()  arity_37()  , 38
# define arity_39()  arity_38()  , 39
# define arity_40()  arity_39()  , 40
# define arity_41()  arity_40()  , 41
# define arity_42()  arity_41()  , 42
# define arity_43()  arity_42()  , 43
# define arity_44()  arity_43()  , 44
# define arity_45()  arity_44()  , 45
# define arity_46()  arity_45()  , 46
# define arity_47()  arity_46()  , 47
# define arity_48()  arity_47()  , 48
# define arity_49()  arity_48()  , 49
# define arity_50()  arity_49()  , 50
# define arity_51()  arity_50()  , 51
# define arity_52()  arity_51()  , 52
# define arity_53()  arity_52()  , 53
# define arity_54()  arity_53()  , 54
# define arity_55()  arity_54()  , 55
# define arity_56()  arity_55()  , 56
# define arity_57()  arity_56()  , 57
# define arity_58()  arity_57()  , 58
# define arity_59()  arity_58()  , 59
# define arity_60()  arity_59()  , 60
# define arity_61()  arity_60()  , 61
# define arity_62()  arity_61()  , 62
# define arity_63()  arity_62()  , 63
# define arity_64()  arity_63()  , 64
# define arity_65()  arity_64()  , 65
# define arity_66()  arity_65()  , 66
# define arity_67()  arity_66()  , 67
# define arity_68()  arity_67()  , 68
# define arity_69()  arity_68()  , 69
# define arity_70()  arity_69()  , 70
# define arity_71()  arity_70()  , 71
# define arity_72()  arity_71()  , 72
# define arity_73()  arity_72()  , 73
# define arity_74()  arity_73()  , 74
# define arity_75()  arity_74()  , 75
# define arity_76()  arity_75()  , 76
# define arity_77()  arity_76()  , 77
# define arity_78()  arity_77()  , 78
# define arity_79()  arity_78()  , 79
# define arity_80()  arity_79()  , 80
# define arity_81()  arity_80()  , 81
# define arity_82()  arity_81()  , 82
# define arity_83()  arity_82()  , 83
# define arity_84()  arity_83()  , 84
# define arity_85()  arity_84()  , 85
# define arity_86()  arity_85()  , 86
# define arity_87()  arity_86()  , 87
# define arity_88()  arity_87()  , 88
# define arity_89()  arity_88()  , 89
# define arity_90()  arity_89()  , 90
# define arity_91()  arity_90()  , 91
# define arity_92()  arity_91()  , 92
# define arity_93()  arity_92()  , 93
# define arity_94()  arity_93()  , 94
# define arity_95()  arity_94()  , 95
# define arity_96()  arity_95()  , 96
# define arity_97()  arity_96()  , 97
# define arity_98()  arity_97()  , 98
# define arity_99()  arity_98()  , 99
# define arity_100() arity_99()  , 100
# define arity_101() arity_100() , 101
# define arity_102() arity_101() , 102
# define arity_103() arity_102() , 103
# define arity_104() arity_103() , 104
# define arity_105() arity_104() , 105
# define arity_106() arity_105() , 106
# define arity_107() arity_106() , 107
# define arity_108() arity_107() , 108
# define arity_109() arity_108() , 109
# define arity_110() arity_109() , 110
# define arity_111() arity_110() , 111
# define arity_112() arity_111() , 112
# define arity_113() arity_112() , 113
# define arity_114() arity_113() , 114
# define arity_115() arity_114() , 115
# define arity_116() arity_115() , 116
# define arity_117() arity_116() , 117
# define arity_118() arity_117() , 118
# define arity_119() arity_118() , 119
# define arity_120() arity_119() , 120
# define arity_121() arity_120() , 121
# define arity_122() arity_121() , 122
# define arity_123() arity_122() , 123
# define arity_124() arity_123() , 124
# define arity_125() arity_124() , 125
# define arity_126() arity_125() , 126
# define arity_127() arity_126() , 127
# define arity_128() arity_127() , 128
# define arity_129() arity_128() , 129
# define arity_130() arity_129() , 130
# define arity_131() arity_130() , 131
# define arity_132() arity_131() , 132
# define arity_133() arity_132() , 133
# define arity_134() arity_133() , 134
# define arity_135() arity_134() , 135
# define arity_136() arity_135() , 136
# define arity_137() arity_136() , 137
# define arity_138() arity_137() , 138
# define arity_139() arity_138() , 139
# define arity_140() arity_139() , 140
# define arity_141() arity_140() , 141
# define arity_142() arity_141() , 142
# define arity_143() arity_142() , 143
# define arity_144() arity_143() , 144
# define arity_145() arity_144() , 145
# define arity_146() arity_145() , 146
# define arity_147() arity_146() , 147
# define arity_148() arity_147() , 148
# define arity_149() arity_148() , 149
# define arity_150() arity_149() , 150
# define arity_151() arity_150() , 151
# define arity_152() arity_151() , 152
# define arity_153() arity_152() , 153
# define arity_154() arity_153() , 154
# define arity_155() arity_154() , 155
# define arity_156() arity_155() , 156
# define arity_157() arity_156() , 157
# define arity_158() arity_157() , 158
# define arity_159() arity_158() , 159
# define arity_160() arity_159() , 160
# define arity_161() arity_160() , 161
# define arity_162() arity_161() , 162
# define arity_163() arity_162() , 163
# define arity_164() arity_163() , 164
# define arity_165() arity_164() , 165
# define arity_166() arity_165() , 166
# define arity_167() arity_166() , 167
# define arity_168() arity_167() , 168
# define arity_169() arity_168() , 169
# define arity_170() arity_169() , 170
# define arity_171() arity_170() , 171
# define arity_172() arity_171() , 172
# define arity_173() arity_172() , 173
# define arity_174() arity_173() , 174
# define arity_175() arity_174() , 175
# define arity_176() arity_175() , 176
# define arity_177() arity_176() , 177
# define arity_178() arity_177() , 178
# define arity_179() arity_178() , 179
# define arity_180() arity_179() , 180
# define arity_181() arity_180() , 181
# define arity_182() arity_181() , 182
# define arity_183() arity_182() , 183
# define arity_184() arity_183() , 184
# define arity_185() arity_184() , 185
# define arity_186() arity_185() , 186
# define arity_187() arity_186() , 187
# define arity_188() arity_187() , 188
# define arity_189() arity_188() , 189
# define arity_190() arity_189() , 190
# define arity_191() arity_190() , 191
# define arity_192() arity_191() , 192
# define arity_193() arity_192() , 193
# define arity_194() arity_193() , 194
# define arity_195() arity_194() , 195
# define arity_196() arity_195() , 196
# define arity_197() arity_196() , 197
# define arity_198() arity_197() , 198
# define arity_199() arity_198() , 199
# define arity_200() arity_199() , 200
# define arity_201() arity_200() , 201
# define arity_202() arity_201() , 202
# define arity_203() arity_202() , 203
# define arity_204() arity_203() , 204
# define arity_205() arity_204() , 205
# define arity_206() arity_205() , 206
# define arity_207() arity_206() , 207
# define arity_208() arity_207() , 208
# define arity_209() arity_208() , 209
# define arity_210() arity_209() , 210
# define arity_211() arity_210() , 211
# define arity_212() arity_211() , 212
# define arity_213() arity_212() , 213
# define arity_214() arity_213() , 214
# define arity_215() arity_214() , 215
# define arity_216() arity_215() , 216
# define arity_217() arity_216() , 217
# define arity_218() arity_217() , 218
# define arity_219() arity_218() , 219
# define arity_220() arity_219() , 220
# define arity_221() arity_220() , 221
# define arity_222() arity_221() , 222
# define arity_223() arity_222() , 223
# define arity_224() arity_223() , 224
# define arity_225() arity_224() , 225
# define arity_226() arity_225() , 226
# define arity_227() arity_226() , 227
# define arity_228() arity_227() , 228
# define arity_229() arity_228() , 229
# define arity_230() arity_229() , 230
# define arity_231() arity_230() , 231
# define arity_232() arity_231() , 232
# define arity_233() arity_232() , 233
# define arity_234() arity_233() , 234
# define arity_235() arity_234() , 235
# define arity_236() arity_235() , 236
# define arity_237() arity_236() , 237
# define arity_238() arity_237() , 238
# define arity_239() arity_238() , 239
# define arity_240() arity_239() , 240
# define arity_241() arity_240() , 241
# define arity_242() arity_241() , 242
# define arity_243() arity_242() , 243
# define arity_244() arity_243() , 244
# define arity_245() arity_244() , 245
# define arity_246() arity_245() , 246
# define arity_247() arity_246() , 247
# define arity_248() arity_247() , 248
# define arity_249() arity_248() , 249
# define arity_250() arity_249() , 250
# define arity_251() arity_250() , 251
# define arity_252() arity_251() , 252
# define arity_253() arity_252() , 253
# define arity_254() arity_253() , 254
# define arity_255() arity_254() , 255
#define concatenate(argument1, argument2) argument1 ## argument2
#define defer(macro, ...) macro(__VA_ARGS__)
#define empty()
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
#define repeat(...) repeat_setup(__VA_ARGS__, until, until, ~)
# define repeat_begin(function, separator, condition, argument, next, ...) \
  defer(concatenate, repeat_condition_is_, condition(argument, next, __VA_ARGS__))(repeat_continue, repeat_end)( \
    function, \
    defer(concatenate, repeat_condition_is_, condition(next, __VA_ARGS__))(separator, repeat_separator), \
    stall(repeat_recurse)()(function, separator, condition, next, __VA_ARGS__, ~), \
    argument, next, __VA_ARGS__ \
  )
# define repeat_condition(argument, ...) defer(concatenate, repeat_condition_, defer(second, repeat_condition_found_ ## argument, pass, ~))(argument)
#   define repeat_condition_accept(condition)   condition
#   define repeat_condition_fallback(condition) repeat_condition
#   define repeat_condition_fail(argument) false
#   define repeat_condition_pass(argument) true
#   define repeat_condition_is_false(truthy, falsy) falsy
#   define repeat_condition_is_true(truthy, falsy)  truthy
#     define repeat_condition_found_until       ~, fail
#     define repeat_condition_selection_default ~, fallback
# define repeat_continue(function, separator, repeater, argument, ...) function(argument) separator(argument, __VA_ARGS__) repeater
# define repeat_end(function, separator, repeater, argument, ...)
# define repeat_function(argument) argument
#   define repeat_function_accept(function)   function
#   define repeat_function_fallback(function) repeat_function
#     define repeat_function_selection_default ~, fallback
# define repeat_separator(...)
#   define repeat_separator_accept(separator)   separator
#   define repeat_separator_fallback(separator) repeat_separator
#     define repeat_separator_selection_default ~, fallback
# define repeat_setup(function, condition, separator, ...) parse(repeat_begin( \
  defer(concatenate, repeat_function_ , defer(second, repeat_function_selection_  ## function , accept, ~))(function ), \
  defer(concatenate, repeat_separator_, defer(second, repeat_separator_selection_ ## separator, accept, ~))(separator), \
  defer(concatenate, repeat_condition_, defer(second, repeat_condition_selection_ ## condition, accept, ~))(condition), \
  __VA_ARGS__ \
))
# define repeat_recurse() repeat_begin
#define second(argument1, argument2, ...) argument2
#define stall(argument) stall_12(argument)
# define stall_1(argument)  argument empty()
# define stall_2(argument)  argument empty empty()()
# define stall_3(argument)  argument empty empty empty()()()
# define stall_4(argument)  argument empty empty empty empty()()()()
# define stall_5(argument)  argument empty empty empty empty empty()()()()()
# define stall_6(argument)  argument empty empty empty empty empty empty()()()()()()
# define stall_7(argument)  argument empty empty empty empty empty empty empty()()()()()()()
# define stall_8(argument)  argument empty empty empty empty empty empty empty empty()()()()()()()()
# define stall_9(argument)  argument empty empty empty empty empty empty empty empty empty()()()()()()()()()
# define stall_10(argument) argument empty empty empty empty empty empty empty empty empty empty()()()()()()()()()()
# define stall_11(argument) argument empty empty empty empty empty empty empty empty empty empty empty()()()()()()()()()()()
# define stall_12(argument) argument empty empty empty empty empty empty empty empty empty empty empty empty()()()()()()()()()()()()

/* Main */
#define pass(x) union type
repeat(pass, default, default, arity(255))

int main() {
  std::puts("Hello, World!");
}
