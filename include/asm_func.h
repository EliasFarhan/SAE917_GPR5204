//
// Created by efarhan on 23/12/2019.
//

#ifndef GPR5204_ASM_FUNC_H
#define GPR5204_ASM_FUNC_H
#ifdef __cplusplus
extern "C" {
#endif

//swap.asm
void swap_int(int *a, int *b);
void swap_float(float *a, float *b);

struct vec4 {
    float x,y,z,w;
};
void swap_vec4(vec4 *a, vec4 *b);

//value.asm
long bad_int2long(int);
long int2long(int);
int bad_shiftr(int);
int shiftr(int);

//array.asm
int avg(const int* v, size_t length);


#ifdef __cplusplus
}
#endif
#endif //GPR5204_ASM_FUNC_H
