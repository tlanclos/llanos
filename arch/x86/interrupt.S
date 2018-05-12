.intel_syntax noprefix

.section .text

.macro __isr_handler_generic isrnum
    pushad
    push \isrnum
    call __generic_isr
    mov %al, 0x20
    out 0x20, %al
    popad
    iret
.endm

.global __isr_handler_0
__isr_handler_0: __isr_handler_generic 0
.global __isr_handler_1
__isr_handler_1: __isr_handler_generic 1
.global __isr_handler_2
__isr_handler_2: __isr_handler_generic 2
.global __isr_handler_3
__isr_handler_3: __isr_handler_generic 3
.global __isr_handler_4
__isr_handler_4: __isr_handler_generic 4
.global __isr_handler_5
__isr_handler_5: __isr_handler_generic 5
.global __isr_handler_6
__isr_handler_6: __isr_handler_generic 6
.global __isr_handler_7
__isr_handler_7: __isr_handler_generic 7
.global __isr_handler_8
__isr_handler_8: __isr_handler_generic 8
.global __isr_handler_9
__isr_handler_9: __isr_handler_generic 9
.global __isr_handler_10
__isr_handler_10: __isr_handler_generic 10
.global __isr_handler_11
__isr_handler_11: __isr_handler_generic 11
.global __isr_handler_12
__isr_handler_12: __isr_handler_generic 12
.global __isr_handler_13
__isr_handler_13: __isr_handler_generic 13
.global __isr_handler_14
__isr_handler_14: __isr_handler_generic 14
.global __isr_handler_15
__isr_handler_15: __isr_handler_generic 15
.global __isr_handler_16
__isr_handler_16: __isr_handler_generic 16
.global __isr_handler_17
__isr_handler_17: __isr_handler_generic 17
.global __isr_handler_18
__isr_handler_18: __isr_handler_generic 18
.global __isr_handler_19
__isr_handler_19: __isr_handler_generic 19
.global __isr_handler_20
__isr_handler_20: __isr_handler_generic 20
.global __isr_handler_21
__isr_handler_21: __isr_handler_generic 21
.global __isr_handler_22
__isr_handler_22: __isr_handler_generic 22
.global __isr_handler_23
__isr_handler_23: __isr_handler_generic 23
.global __isr_handler_24
__isr_handler_24: __isr_handler_generic 24
.global __isr_handler_25
__isr_handler_25: __isr_handler_generic 25
.global __isr_handler_26
__isr_handler_26: __isr_handler_generic 26
.global __isr_handler_27
__isr_handler_27: __isr_handler_generic 27
.global __isr_handler_28
__isr_handler_28: __isr_handler_generic 28
.global __isr_handler_29
__isr_handler_29: __isr_handler_generic 29
.global __isr_handler_30
__isr_handler_30: __isr_handler_generic 30
.global __isr_handler_31
__isr_handler_31: __isr_handler_generic 31
.global __isr_handler_32
__isr_handler_32: __isr_handler_generic 32
.global __isr_handler_33
__isr_handler_33: __isr_handler_generic 33
.global __isr_handler_34
__isr_handler_34: __isr_handler_generic 34
.global __isr_handler_35
__isr_handler_35: __isr_handler_generic 35
.global __isr_handler_36
__isr_handler_36: __isr_handler_generic 36
.global __isr_handler_37
__isr_handler_37: __isr_handler_generic 37
.global __isr_handler_38
__isr_handler_38: __isr_handler_generic 38
.global __isr_handler_39
__isr_handler_39: __isr_handler_generic 39
.global __isr_handler_40
__isr_handler_40: __isr_handler_generic 40
.global __isr_handler_41
__isr_handler_41: __isr_handler_generic 41
.global __isr_handler_42
__isr_handler_42: __isr_handler_generic 42
.global __isr_handler_43
__isr_handler_43: __isr_handler_generic 43
.global __isr_handler_44
__isr_handler_44: __isr_handler_generic 44
.global __isr_handler_45
__isr_handler_45: __isr_handler_generic 45
.global __isr_handler_46
__isr_handler_46: __isr_handler_generic 46
.global __isr_handler_47
__isr_handler_47: __isr_handler_generic 47
.global __isr_handler_48
__isr_handler_48: __isr_handler_generic 48
.global __isr_handler_49
__isr_handler_49: __isr_handler_generic 49
.global __isr_handler_50
__isr_handler_50: __isr_handler_generic 50
.global __isr_handler_51
__isr_handler_51: __isr_handler_generic 51
.global __isr_handler_52
__isr_handler_52: __isr_handler_generic 52
.global __isr_handler_53
__isr_handler_53: __isr_handler_generic 53
.global __isr_handler_54
__isr_handler_54: __isr_handler_generic 54
.global __isr_handler_55
__isr_handler_55: __isr_handler_generic 55
.global __isr_handler_56
__isr_handler_56: __isr_handler_generic 56
.global __isr_handler_57
__isr_handler_57: __isr_handler_generic 57
.global __isr_handler_58
__isr_handler_58: __isr_handler_generic 58
.global __isr_handler_59
__isr_handler_59: __isr_handler_generic 59
.global __isr_handler_60
__isr_handler_60: __isr_handler_generic 60
.global __isr_handler_61
__isr_handler_61: __isr_handler_generic 61
.global __isr_handler_62
__isr_handler_62: __isr_handler_generic 62
.global __isr_handler_63
__isr_handler_63: __isr_handler_generic 63
.global __isr_handler_64
__isr_handler_64: __isr_handler_generic 64
.global __isr_handler_65
__isr_handler_65: __isr_handler_generic 65
.global __isr_handler_66
__isr_handler_66: __isr_handler_generic 66
.global __isr_handler_67
__isr_handler_67: __isr_handler_generic 67
.global __isr_handler_68
__isr_handler_68: __isr_handler_generic 68
.global __isr_handler_69
__isr_handler_69: __isr_handler_generic 69
.global __isr_handler_70
__isr_handler_70: __isr_handler_generic 70
.global __isr_handler_71
__isr_handler_71: __isr_handler_generic 71
.global __isr_handler_72
__isr_handler_72: __isr_handler_generic 72
.global __isr_handler_73
__isr_handler_73: __isr_handler_generic 73
.global __isr_handler_74
__isr_handler_74: __isr_handler_generic 74
.global __isr_handler_75
__isr_handler_75: __isr_handler_generic 75
.global __isr_handler_76
__isr_handler_76: __isr_handler_generic 76
.global __isr_handler_77
__isr_handler_77: __isr_handler_generic 77
.global __isr_handler_78
__isr_handler_78: __isr_handler_generic 78
.global __isr_handler_79
__isr_handler_79: __isr_handler_generic 79
.global __isr_handler_80
__isr_handler_80: __isr_handler_generic 80
.global __isr_handler_81
__isr_handler_81: __isr_handler_generic 81
.global __isr_handler_82
__isr_handler_82: __isr_handler_generic 82
.global __isr_handler_83
__isr_handler_83: __isr_handler_generic 83
.global __isr_handler_84
__isr_handler_84: __isr_handler_generic 84
.global __isr_handler_85
__isr_handler_85: __isr_handler_generic 85
.global __isr_handler_86
__isr_handler_86: __isr_handler_generic 86
.global __isr_handler_87
__isr_handler_87: __isr_handler_generic 87
.global __isr_handler_88
__isr_handler_88: __isr_handler_generic 88
.global __isr_handler_89
__isr_handler_89: __isr_handler_generic 89
.global __isr_handler_90
__isr_handler_90: __isr_handler_generic 90
.global __isr_handler_91
__isr_handler_91: __isr_handler_generic 91
.global __isr_handler_92
__isr_handler_92: __isr_handler_generic 92
.global __isr_handler_93
__isr_handler_93: __isr_handler_generic 93
.global __isr_handler_94
__isr_handler_94: __isr_handler_generic 94
.global __isr_handler_95
__isr_handler_95: __isr_handler_generic 95
.global __isr_handler_96
__isr_handler_96: __isr_handler_generic 96
.global __isr_handler_97
__isr_handler_97: __isr_handler_generic 97
.global __isr_handler_98
__isr_handler_98: __isr_handler_generic 98
.global __isr_handler_99
__isr_handler_99: __isr_handler_generic 99
.global __isr_handler_100
__isr_handler_100: __isr_handler_generic 100
.global __isr_handler_101
__isr_handler_101: __isr_handler_generic 101
.global __isr_handler_102
__isr_handler_102: __isr_handler_generic 102
.global __isr_handler_103
__isr_handler_103: __isr_handler_generic 103
.global __isr_handler_104
__isr_handler_104: __isr_handler_generic 104
.global __isr_handler_105
__isr_handler_105: __isr_handler_generic 105
.global __isr_handler_106
__isr_handler_106: __isr_handler_generic 106
.global __isr_handler_107
__isr_handler_107: __isr_handler_generic 107
.global __isr_handler_108
__isr_handler_108: __isr_handler_generic 108
.global __isr_handler_109
__isr_handler_109: __isr_handler_generic 109
.global __isr_handler_110
__isr_handler_110: __isr_handler_generic 110
.global __isr_handler_111
__isr_handler_111: __isr_handler_generic 111
.global __isr_handler_112
__isr_handler_112: __isr_handler_generic 112
.global __isr_handler_113
__isr_handler_113: __isr_handler_generic 113
.global __isr_handler_114
__isr_handler_114: __isr_handler_generic 114
.global __isr_handler_115
__isr_handler_115: __isr_handler_generic 115
.global __isr_handler_116
__isr_handler_116: __isr_handler_generic 116
.global __isr_handler_117
__isr_handler_117: __isr_handler_generic 117
.global __isr_handler_118
__isr_handler_118: __isr_handler_generic 118
.global __isr_handler_119
__isr_handler_119: __isr_handler_generic 119
.global __isr_handler_120
__isr_handler_120: __isr_handler_generic 120
.global __isr_handler_121
__isr_handler_121: __isr_handler_generic 121
.global __isr_handler_122
__isr_handler_122: __isr_handler_generic 122
.global __isr_handler_123
__isr_handler_123: __isr_handler_generic 123
.global __isr_handler_124
__isr_handler_124: __isr_handler_generic 124
.global __isr_handler_125
__isr_handler_125: __isr_handler_generic 125
.global __isr_handler_126
__isr_handler_126: __isr_handler_generic 126
.global __isr_handler_127
__isr_handler_127: __isr_handler_generic 127
.global __isr_handler_128
__isr_handler_128: __isr_handler_generic 128
.global __isr_handler_129
__isr_handler_129: __isr_handler_generic 129
.global __isr_handler_130
__isr_handler_130: __isr_handler_generic 130
.global __isr_handler_131
__isr_handler_131: __isr_handler_generic 131
.global __isr_handler_132
__isr_handler_132: __isr_handler_generic 132
.global __isr_handler_133
__isr_handler_133: __isr_handler_generic 133
.global __isr_handler_134
__isr_handler_134: __isr_handler_generic 134
.global __isr_handler_135
__isr_handler_135: __isr_handler_generic 135
.global __isr_handler_136
__isr_handler_136: __isr_handler_generic 136
.global __isr_handler_137
__isr_handler_137: __isr_handler_generic 137
.global __isr_handler_138
__isr_handler_138: __isr_handler_generic 138
.global __isr_handler_139
__isr_handler_139: __isr_handler_generic 139
.global __isr_handler_140
__isr_handler_140: __isr_handler_generic 140
.global __isr_handler_141
__isr_handler_141: __isr_handler_generic 141
.global __isr_handler_142
__isr_handler_142: __isr_handler_generic 142
.global __isr_handler_143
__isr_handler_143: __isr_handler_generic 143
.global __isr_handler_144
__isr_handler_144: __isr_handler_generic 144
.global __isr_handler_145
__isr_handler_145: __isr_handler_generic 145
.global __isr_handler_146
__isr_handler_146: __isr_handler_generic 146
.global __isr_handler_147
__isr_handler_147: __isr_handler_generic 147
.global __isr_handler_148
__isr_handler_148: __isr_handler_generic 148
.global __isr_handler_149
__isr_handler_149: __isr_handler_generic 149
.global __isr_handler_150
__isr_handler_150: __isr_handler_generic 150
.global __isr_handler_151
__isr_handler_151: __isr_handler_generic 151
.global __isr_handler_152
__isr_handler_152: __isr_handler_generic 152
.global __isr_handler_153
__isr_handler_153: __isr_handler_generic 153
.global __isr_handler_154
__isr_handler_154: __isr_handler_generic 154
.global __isr_handler_155
__isr_handler_155: __isr_handler_generic 155
.global __isr_handler_156
__isr_handler_156: __isr_handler_generic 156
.global __isr_handler_157
__isr_handler_157: __isr_handler_generic 157
.global __isr_handler_158
__isr_handler_158: __isr_handler_generic 158
.global __isr_handler_159
__isr_handler_159: __isr_handler_generic 159
.global __isr_handler_160
__isr_handler_160: __isr_handler_generic 160
.global __isr_handler_161
__isr_handler_161: __isr_handler_generic 161
.global __isr_handler_162
__isr_handler_162: __isr_handler_generic 162
.global __isr_handler_163
__isr_handler_163: __isr_handler_generic 163
.global __isr_handler_164
__isr_handler_164: __isr_handler_generic 164
.global __isr_handler_165
__isr_handler_165: __isr_handler_generic 165
.global __isr_handler_166
__isr_handler_166: __isr_handler_generic 166
.global __isr_handler_167
__isr_handler_167: __isr_handler_generic 167
.global __isr_handler_168
__isr_handler_168: __isr_handler_generic 168
.global __isr_handler_169
__isr_handler_169: __isr_handler_generic 169
.global __isr_handler_170
__isr_handler_170: __isr_handler_generic 170
.global __isr_handler_171
__isr_handler_171: __isr_handler_generic 171
.global __isr_handler_172
__isr_handler_172: __isr_handler_generic 172
.global __isr_handler_173
__isr_handler_173: __isr_handler_generic 173
.global __isr_handler_174
__isr_handler_174: __isr_handler_generic 174
.global __isr_handler_175
__isr_handler_175: __isr_handler_generic 175
.global __isr_handler_176
__isr_handler_176: __isr_handler_generic 176
.global __isr_handler_177
__isr_handler_177: __isr_handler_generic 177
.global __isr_handler_178
__isr_handler_178: __isr_handler_generic 178
.global __isr_handler_179
__isr_handler_179: __isr_handler_generic 179
.global __isr_handler_180
__isr_handler_180: __isr_handler_generic 180
.global __isr_handler_181
__isr_handler_181: __isr_handler_generic 181
.global __isr_handler_182
__isr_handler_182: __isr_handler_generic 182
.global __isr_handler_183
__isr_handler_183: __isr_handler_generic 183
.global __isr_handler_184
__isr_handler_184: __isr_handler_generic 184
.global __isr_handler_185
__isr_handler_185: __isr_handler_generic 185
.global __isr_handler_186
__isr_handler_186: __isr_handler_generic 186
.global __isr_handler_187
__isr_handler_187: __isr_handler_generic 187
.global __isr_handler_188
__isr_handler_188: __isr_handler_generic 188
.global __isr_handler_189
__isr_handler_189: __isr_handler_generic 189
.global __isr_handler_190
__isr_handler_190: __isr_handler_generic 190
.global __isr_handler_191
__isr_handler_191: __isr_handler_generic 191
.global __isr_handler_192
__isr_handler_192: __isr_handler_generic 192
.global __isr_handler_193
__isr_handler_193: __isr_handler_generic 193
.global __isr_handler_194
__isr_handler_194: __isr_handler_generic 194
.global __isr_handler_195
__isr_handler_195: __isr_handler_generic 195
.global __isr_handler_196
__isr_handler_196: __isr_handler_generic 196
.global __isr_handler_197
__isr_handler_197: __isr_handler_generic 197
.global __isr_handler_198
__isr_handler_198: __isr_handler_generic 198
.global __isr_handler_199
__isr_handler_199: __isr_handler_generic 199
.global __isr_handler_200
__isr_handler_200: __isr_handler_generic 200
.global __isr_handler_201
__isr_handler_201: __isr_handler_generic 201
.global __isr_handler_202
__isr_handler_202: __isr_handler_generic 202
.global __isr_handler_203
__isr_handler_203: __isr_handler_generic 203
.global __isr_handler_204
__isr_handler_204: __isr_handler_generic 204
.global __isr_handler_205
__isr_handler_205: __isr_handler_generic 205
.global __isr_handler_206
__isr_handler_206: __isr_handler_generic 206
.global __isr_handler_207
__isr_handler_207: __isr_handler_generic 207
.global __isr_handler_208
__isr_handler_208: __isr_handler_generic 208
.global __isr_handler_209
__isr_handler_209: __isr_handler_generic 209
.global __isr_handler_210
__isr_handler_210: __isr_handler_generic 210
.global __isr_handler_211
__isr_handler_211: __isr_handler_generic 211
.global __isr_handler_212
__isr_handler_212: __isr_handler_generic 212
.global __isr_handler_213
__isr_handler_213: __isr_handler_generic 213
.global __isr_handler_214
__isr_handler_214: __isr_handler_generic 214
.global __isr_handler_215
__isr_handler_215: __isr_handler_generic 215
.global __isr_handler_216
__isr_handler_216: __isr_handler_generic 216
.global __isr_handler_217
__isr_handler_217: __isr_handler_generic 217
.global __isr_handler_218
__isr_handler_218: __isr_handler_generic 218
.global __isr_handler_219
__isr_handler_219: __isr_handler_generic 219
.global __isr_handler_220
__isr_handler_220: __isr_handler_generic 220
.global __isr_handler_221
__isr_handler_221: __isr_handler_generic 221
.global __isr_handler_222
__isr_handler_222: __isr_handler_generic 222
.global __isr_handler_223
__isr_handler_223: __isr_handler_generic 223
.global __isr_handler_224
__isr_handler_224: __isr_handler_generic 224
.global __isr_handler_225
__isr_handler_225: __isr_handler_generic 225
.global __isr_handler_226
__isr_handler_226: __isr_handler_generic 226
.global __isr_handler_227
__isr_handler_227: __isr_handler_generic 227
.global __isr_handler_228
__isr_handler_228: __isr_handler_generic 228
.global __isr_handler_229
__isr_handler_229: __isr_handler_generic 229
.global __isr_handler_230
__isr_handler_230: __isr_handler_generic 230
.global __isr_handler_231
__isr_handler_231: __isr_handler_generic 231
.global __isr_handler_232
__isr_handler_232: __isr_handler_generic 232
.global __isr_handler_233
__isr_handler_233: __isr_handler_generic 233
.global __isr_handler_234
__isr_handler_234: __isr_handler_generic 234
.global __isr_handler_235
__isr_handler_235: __isr_handler_generic 235
.global __isr_handler_236
__isr_handler_236: __isr_handler_generic 236
.global __isr_handler_237
__isr_handler_237: __isr_handler_generic 237
.global __isr_handler_238
__isr_handler_238: __isr_handler_generic 238
.global __isr_handler_239
__isr_handler_239: __isr_handler_generic 239
.global __isr_handler_240
__isr_handler_240: __isr_handler_generic 240
.global __isr_handler_241
__isr_handler_241: __isr_handler_generic 241
.global __isr_handler_242
__isr_handler_242: __isr_handler_generic 242
.global __isr_handler_243
__isr_handler_243: __isr_handler_generic 243
.global __isr_handler_244
__isr_handler_244: __isr_handler_generic 244
.global __isr_handler_245
__isr_handler_245: __isr_handler_generic 245
.global __isr_handler_246
__isr_handler_246: __isr_handler_generic 246
.global __isr_handler_247
__isr_handler_247: __isr_handler_generic 247
.global __isr_handler_248
__isr_handler_248: __isr_handler_generic 248
.global __isr_handler_249
__isr_handler_249: __isr_handler_generic 249
.global __isr_handler_250
__isr_handler_250: __isr_handler_generic 250
.global __isr_handler_251
__isr_handler_251: __isr_handler_generic 251
.global __isr_handler_252
__isr_handler_252: __isr_handler_generic 252
.global __isr_handler_253
__isr_handler_253: __isr_handler_generic 253
.global __isr_handler_254
__isr_handler_254: __isr_handler_generic 254
.global __isr_handler_255
__isr_handler_255: __isr_handler_generic 255


.global __interrupt_table_descriptor

.global __load_idt
__load_idt:
    push %ebp
    mov %ebp, %esp
    lidt [__interrupt_table_descriptor]
    sti
    mov %esp, %ebp
    pop %ebp
    ret
