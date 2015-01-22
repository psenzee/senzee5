#include "opcode.h"
#include <stdio.h>
#include <conio.h>

const unsigned char _test_code1[] =
{
    0x05, 0x07, 0x00, 0x00, 0x00, 0x1c, 0x50, 0x0e, 0x05, 0x08, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00,
    0x00, 0x00, 0x09, 0x03, 0x1d, 0x00, 0x6a, 0x00, 0x05, 0x16, 0x00, 0x00, 0x00, 0x6a, 0x01, 0x68,
    0x01, 0x51, 0x6a, 0x01, 0x68, 0x01, 0x4d, 0x07, 0x20, 0x00, 0x68, 0x01, 0x4e, 0x6a, 0x02, 0x05,
    0x17, 0x00, 0x00, 0x00, 0x68, 0x00, 0x2b, 0x6a, 0x00, 0x68, 0x00, 0x68, 0x02, 0x68, 0x00, 0x66,
    0x03, 0x00, 0x00, 0x00, 0x09, 0x03, 0x0c, 0x08, 0xda, 0xff, 0x05, 0x18, 0x00, 0x00, 0x00, 0x6a,
    0x03, 0x68, 0x03, 0x51, 0x6a, 0x03, 0x68, 0x03, 0x4d, 0x07, 0x1c, 0x00, 0x68, 0x03, 0x4e, 0x6a,
    0x02, 0x05, 0x1a, 0x00, 0x00, 0x00, 0x68, 0x02, 0x05, 0x19, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00,
    0x00, 0x00, 0x09, 0x03, 0x0c, 0x08, 0xde, 0xff, 0x05, 0x07, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00,
    0x00, 0x00, 0x09, 0x01, 0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00,
    0x09, 0x02, 0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x00, 0x71, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05,
    0x2b, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x26,
    0x00, 0x00, 0x00, 0x71, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x28, 0x00, 0x00, 0x00, 0x05, 0x1c,
    0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x23, 0x00, 0x00, 0x00, 0x71, 0x05, 0x1c,
    0x00, 0x00, 0x00, 0x05, 0x25, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x1b, 0x00,
    0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x00,
    0x78, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x30, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00,
    0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x26, 0x00, 0x00, 0x00, 0x78, 0x05, 0x1c, 0x00, 0x00, 0x00,
    0x05, 0x2f, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05,
    0x23, 0x00, 0x00, 0x00, 0x78, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x2e, 0x00, 0x00, 0x00, 0x05,
    0x1c, 0x00, 0x00, 0x00, 0x05, 0x2c, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10,
    0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x00, 0x72, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x35, 0x00,
    0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x26, 0x00, 0x00,
    0x00, 0x72, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x34, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00,
    0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x23, 0x00, 0x00, 0x00, 0x72, 0x05, 0x1c, 0x00, 0x00,
    0x00, 0x05, 0x33, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x31, 0x00, 0x00, 0x00,
    0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x00, 0x6d, 0x05,
    0x1c, 0x00, 0x00, 0x00, 0x05, 0x42, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x07,
    0x00, 0x00, 0x00, 0x05, 0x26, 0x00, 0x00, 0x00, 0x6d, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x41,
    0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x3e, 0x00,
    0x00, 0x00, 0x6d, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x40, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00,
    0x00, 0x00, 0x05, 0x36, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x05, 0x43,
    0x00, 0x00, 0x00, 0x66, 0x44, 0x00, 0x00, 0x00, 0x09, 0x01, 0x66, 0x44, 0x00, 0x00, 0x00, 0x6a,
    0x02, 0x05, 0x48, 0x00, 0x00, 0x00, 0x6a, 0x04, 0x05, 0x4a, 0x00, 0x00, 0x00, 0x68, 0x04, 0x68,
    0x02, 0x67, 0x05, 0x49, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x03, 0x05, 0x4c,
    0x00, 0x00, 0x00, 0x05, 0x4b, 0x00, 0x00, 0x00, 0x66, 0x4d, 0x00, 0x00, 0x00, 0x09, 0x02, 0x6a,
    0x05, 0x05, 0x4e, 0x00, 0x00, 0x00, 0x68, 0x05, 0x66, 0x02, 0x00, 0x00, 0x00, 0x09, 0x02, 0x66,
    0x4f, 0x00, 0x00, 0x00, 0x09, 0x00, 0x6a, 0x06, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x52, 0x00,
    0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x05, 0x51, 0x00, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00,
    0x00, 0x05, 0x50, 0x00, 0x00, 0x00, 0x68, 0x06, 0x66, 0x02, 0x00, 0x00, 0x00, 0x09, 0x07, 0x68,
    0x06, 0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x01, 0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x02, 0x05,
    0x54, 0x00, 0x00, 0x00, 0x05, 0x1e, 0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x66, 0x01,
    0x00, 0x00, 0x00, 0x09, 0x00, 0x05, 0x54, 0x00, 0x00, 0x00, 0x05, 0x56, 0x00, 0x00, 0x00, 0x05,
    0x07, 0x00, 0x00, 0x00, 0x66, 0x44, 0x00, 0x00, 0x00, 0x05, 0x54, 0x00, 0x00, 0x00, 0x05, 0x55,
    0x00, 0x00, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x68, 0x05, 0x05, 0x54, 0x00, 0x00, 0x00, 0x05,
    0x53, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x09, 0x10, 0x68, 0x05, 0x66, 0x57, 0x00,
    0x00, 0x00, 0x09, 0x01, 0x66, 0x58, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0e
};

const unsigned char _test_code2[] = 
{
  0x05, 0x04, 0x00, 0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x05, 0x02, 0x00, 0x00, 0x00, 0x05, 
  0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x66, 0x05, 0x00, 0x00, 0x00, 0x09, 0x05, 
  0x6a, 0x00, 0x05, 0x07, 0x00, 0x00, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x05, 0x06, 0x00, 0x00, 
  0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x66, 0x05, 0x00, 0x00, 0x00, 
  0x09, 0x05, 0x6a, 0x01, 0x05, 0x0b, 0x00, 0x00, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x05, 0x0a, 
  0x00, 0x00, 0x00, 0x05, 0x09, 0x00, 0x00, 0x00, 0x05, 0x08, 0x00, 0x00, 0x00, 0x66, 0x05, 0x00, 
  0x00, 0x00, 0x09, 0x05, 0x6a, 0x02, 0x05, 0x0d, 0x00, 0x00, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 
  0x05, 0x0c, 0x00, 0x00, 0x00, 0x05, 0x09, 0x00, 0x00, 0x00, 0x05, 0x08, 0x00, 0x00, 0x00, 0x66, 
  0x05, 0x00, 0x00, 0x00, 0x09, 0x05, 0x6a, 0x03, 0x05, 0x0e, 0x00, 0x00, 0x00, 0x68, 0x02, 0x09, 
  0x01, 0x05, 0x1c, 0x00, 0x00, 0x00, 0x6a, 0x04, 0x68, 0x04, 0x51, 0x6a, 0x04, 0x68, 0x04, 0x4d, 
  0x07, 0x10, 0x00, 0x68, 0x04, 0x4e, 0x6a, 0x05, 0x1c, 0xf4, 0x01, 0x68, 0x00, 0x09, 0x01, 0x0c, 
  0x08, 0xea, 0xff, 0x05, 0x1e, 0x00, 0x00, 0x00, 0x68, 0x00, 0x66, 0x1d, 0x00, 0x00, 0x00, 0x09, 
  0x00, 0x66, 0x1f, 0x00, 0x00, 0x00, 0x09, 0x03, 0x05, 0x1e, 0x00, 0x00, 0x00, 0x68, 0x01, 0x66, 
  0x1d, 0x00, 0x00, 0x00, 0x09, 0x00, 0x66, 0x1f, 0x00, 0x00, 0x00, 0x09, 0x03, 0x05, 0x20, 0x00, 
  0x00, 0x00, 0x6a, 0x05, 0x05, 0x1e, 0x00, 0x00, 0x00, 0x68, 0x05, 0x66, 0x1d, 0x00, 0x00, 0x00, 
  0x09, 0x00, 0x66, 0x1f, 0x00, 0x00, 0x00, 0x09, 0x03, 0x05, 0x21, 0x00, 0x00, 0x00, 0x6a, 0x05, 
  0x05, 0x1e, 0x00, 0x00, 0x00, 0x68, 0x05, 0x66, 0x1d, 0x00, 0x00, 0x00, 0x09, 0x00, 0x66, 0x1f, 
  0x00, 0x00, 0x00, 0x09, 0x03, 0x68, 0x05, 0x1d, 0x00, 0x2c, 0x6a, 0x05, 0x05, 0x1e, 0x00, 0x00, 
  0x00, 0x68, 0x05, 0x66, 0x1d, 0x00, 0x00, 0x00, 0x09, 0x00, 0x66, 0x1f, 0x00, 0x00, 0x00, 0x09, 
  0x03, 0x05, 0x22, 0x00, 0x00, 0x00, 0x6a, 0x06, 0x68, 0x03, 0x09, 0x00, 0x0e
};

unsigned char *ref_expand(const unsigned char *code, unsigned sz, unsigned *out_sz)
{
    std::vector<Instruction> ins;
    instructions(code, sz, ins);
    expand(ins);
    *out_sz = size(ins);
    return write(ins);
}

void test_expands(const unsigned char *code, unsigned sz)
{
    unsigned out_sz1 = 0, out_sz2 = 0;
    unsigned char *a = ref_expand(code, sz, &out_sz1),
                  *b = _expand   (code, sz, &out_sz2);
    printf("testing expand (in size %u, out size %u[1] %u[2])\n", sz, out_sz1, out_sz2);
    bool r = out_sz1 == out_sz2 && memcmp(a, b, out_sz1) == 0;
    printf("test %s\n", r ? "passed" : "failed");
    std::string s;
    printf("disassemble input? (y/n)\n");
    char c = getch();
    if (tolower(c) == 'y')
        printf("%s\n", (s = Opcode::disassemble(code, sz)).c_str());
    printf("disassemble output? (y/n)\n");
    c = getch();
    if (tolower(c) == 'y')
    {
        printf("%s\n", (s = Opcode::disassemble(b, out_sz2)).c_str());
        getch();
    }
}

void main()
{
    test_expands(_test_code1, sizeof(_test_code1));
    test_expands(_test_code2, sizeof(_test_code2));
/*
    std::string s = Opcode::disassemble(_test_code1, sizeof(_test_code1));
    printf("%s\n", s.c_str());
    std::vector<Instruction> ins;
    instructions(_test_code1, sizeof(_test_code1), ins);

    print(ins);
    printf("COUNT %u\n", instruction_count(_test_code1, sizeof(_test_code1)));
    getch();
    compress(ins);
    print(ins);
    getch();
    expand(ins);
    print(ins);
    unsigned char *expanded = write(ins);
    getch();
    delete [] expanded;
*/
    printf("Hit any key..\n");
    getch();
}