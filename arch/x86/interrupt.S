.intel_syntax noprefix

.section .text

.global __interrupt_load_idtr
.align 4
__interrupt_load_idtr:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, [%ebp+8]
    lidt [%eax]
    sti
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret

.global interrupt_store_idtr
.align 4
interrupt_store_idtr:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, [%ebp+8]
    sidt [%eax]
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret


.macro __isr_handler isrnum
.align 4
.global __isr_handler_\isrnum
__isr_handler_\isrnum:
    pushad
    cmpw __generic_interrupt_handler, 0
    je __isr_handle_nocall_\isrnum
    push \isrnum
    cld
    call [__generic_interrupt_handler]
    add %esp, 4
__isr_handle_nocall_\isrnum:
    popad
    iretd
.endm

__isr_handler 0
__isr_handler 1
__isr_handler 2
__isr_handler 3
__isr_handler 4
__isr_handler 5
__isr_handler 6
__isr_handler 7
__isr_handler 8
__isr_handler 9
__isr_handler 10
__isr_handler 11
__isr_handler 12
__isr_handler 13
__isr_handler 14
__isr_handler 15
__isr_handler 16
__isr_handler 17
__isr_handler 18
__isr_handler 19
__isr_handler 20
__isr_handler 21
__isr_handler 22
__isr_handler 23
__isr_handler 24
__isr_handler 25
__isr_handler 26
__isr_handler 27
__isr_handler 28
__isr_handler 29
__isr_handler 30
__isr_handler 31
__isr_handler 32
__isr_handler 33
__isr_handler 34
__isr_handler 35
__isr_handler 36
__isr_handler 37
__isr_handler 38
__isr_handler 39
__isr_handler 40
__isr_handler 41
__isr_handler 42
__isr_handler 43
__isr_handler 44
__isr_handler 45
__isr_handler 46
__isr_handler 47
__isr_handler 48
__isr_handler 49
__isr_handler 50
__isr_handler 51
__isr_handler 52
__isr_handler 53
__isr_handler 54
__isr_handler 55
__isr_handler 56
__isr_handler 57
__isr_handler 58
__isr_handler 59
__isr_handler 60
__isr_handler 61
__isr_handler 62
__isr_handler 63
__isr_handler 64
__isr_handler 65
__isr_handler 66
__isr_handler 67
__isr_handler 68
__isr_handler 69
__isr_handler 70
__isr_handler 71
__isr_handler 72
__isr_handler 73
__isr_handler 74
__isr_handler 75
__isr_handler 76
__isr_handler 77
__isr_handler 78
__isr_handler 79
__isr_handler 80
__isr_handler 81
__isr_handler 82
__isr_handler 83
__isr_handler 84
__isr_handler 85
__isr_handler 86
__isr_handler 87
__isr_handler 88
__isr_handler 89
__isr_handler 90
__isr_handler 91
__isr_handler 92
__isr_handler 93
__isr_handler 94
__isr_handler 95
__isr_handler 96
__isr_handler 97
__isr_handler 98
__isr_handler 99
__isr_handler 100
__isr_handler 101
__isr_handler 102
__isr_handler 103
__isr_handler 104
__isr_handler 105
__isr_handler 106
__isr_handler 107
__isr_handler 108
__isr_handler 109
__isr_handler 110
__isr_handler 111
__isr_handler 112
__isr_handler 113
__isr_handler 114
__isr_handler 115
__isr_handler 116
__isr_handler 117
__isr_handler 118
__isr_handler 119
__isr_handler 120
__isr_handler 121
__isr_handler 122
__isr_handler 123
__isr_handler 124
__isr_handler 125
__isr_handler 126
__isr_handler 127
__isr_handler 128
__isr_handler 129
__isr_handler 130
__isr_handler 131
__isr_handler 132
__isr_handler 133
__isr_handler 134
__isr_handler 135
__isr_handler 136
__isr_handler 137
__isr_handler 138
__isr_handler 139
__isr_handler 140
__isr_handler 141
__isr_handler 142
__isr_handler 143
__isr_handler 144
__isr_handler 145
__isr_handler 146
__isr_handler 147
__isr_handler 148
__isr_handler 149
__isr_handler 150
__isr_handler 151
__isr_handler 152
__isr_handler 153
__isr_handler 154
__isr_handler 155
__isr_handler 156
__isr_handler 157
__isr_handler 158
__isr_handler 159
__isr_handler 160
__isr_handler 161
__isr_handler 162
__isr_handler 163
__isr_handler 164
__isr_handler 165
__isr_handler 166
__isr_handler 167
__isr_handler 168
__isr_handler 169
__isr_handler 170
__isr_handler 171
__isr_handler 172
__isr_handler 173
__isr_handler 174
__isr_handler 175
__isr_handler 176
__isr_handler 177
__isr_handler 178
__isr_handler 179
__isr_handler 180
__isr_handler 181
__isr_handler 182
__isr_handler 183
__isr_handler 184
__isr_handler 185
__isr_handler 186
__isr_handler 187
__isr_handler 188
__isr_handler 189
__isr_handler 190
__isr_handler 191
__isr_handler 192
__isr_handler 193
__isr_handler 194
__isr_handler 195
__isr_handler 196
__isr_handler 197
__isr_handler 198
__isr_handler 199
__isr_handler 200
__isr_handler 201
__isr_handler 202
__isr_handler 203
__isr_handler 204
__isr_handler 205
__isr_handler 206
__isr_handler 207
__isr_handler 208
__isr_handler 209
__isr_handler 210
__isr_handler 211
__isr_handler 212
__isr_handler 213
__isr_handler 214
__isr_handler 215
__isr_handler 216
__isr_handler 217
__isr_handler 218
__isr_handler 219
__isr_handler 220
__isr_handler 221
__isr_handler 222
__isr_handler 223
__isr_handler 224
__isr_handler 225
__isr_handler 226
__isr_handler 227
__isr_handler 228
__isr_handler 229
__isr_handler 230
__isr_handler 231
__isr_handler 232
__isr_handler 233
__isr_handler 234
__isr_handler 235
__isr_handler 236
__isr_handler 237
__isr_handler 238
__isr_handler 239
__isr_handler 240
__isr_handler 241
__isr_handler 242
__isr_handler 243
__isr_handler 244
__isr_handler 245
__isr_handler 246
__isr_handler 247
__isr_handler 248
__isr_handler 249
__isr_handler 250
__isr_handler 251
__isr_handler 252
__isr_handler 253
__isr_handler 254
__isr_handler 255
