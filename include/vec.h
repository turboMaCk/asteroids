#ifndef _VEC_H_
#define _VEC_H_

typedef struct {
  double x;
  double y;
} Vec;

Vec vec_add(Vec a, Vec b)
  ;
Vec vec_scale(float f, Vec vec)
  ;
Vec vec_limit(float f, Vec vec)
  ;

#endif // _VEC_H_
