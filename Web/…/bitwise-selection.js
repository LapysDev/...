var A = 0x1, B = 0x2, C = 0x4, D = 0x8;
var input = A | D;

({'A': input & A, 'B': input & B, 'C': input & C, 'D': input & D})
