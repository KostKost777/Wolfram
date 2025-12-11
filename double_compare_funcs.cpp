#include "double_compare_funcs.h"
#include <assert.h>
#include <math.h>

bool IsDoubleEqual(double num1, double num2)
{
    assert(!isnan(num1));
    assert(!isnan(num2));

    return abs(num1 - num2) < EPSILON;
}

bool IsDoubleBigger(double num1, double num2)
{
    assert(!isnan(num1));
    assert(!isnan(num2));

    return num1 - num2 > EPSILON;
}
