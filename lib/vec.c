#include <vec.h>
#include <math.h>

Vec vec_add(Vec a, Vec b)
{
  Vec res = { a.x + b.x, a.y + b.y };
  return res;
}

Vec vec_scale(float f, Vec vec)
{
  Vec res = { vec.x * f, vec.y * f };
  return res;
}

Vec vec_limit(float f, Vec vec)
{
  float sum = fabs(vec.x) + fabs(vec.y);
  if (sum > f) {
    float k = f / sum;
    Vec res = { vec.x * k, vec.y * k };
    return res;
  } else {
    return vec;
  }
}
