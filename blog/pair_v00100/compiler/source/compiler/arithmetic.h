// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H

#include "ievaluator.h"
#include <math.h>

class BinaryEvaluator : public IEvaluator
{
public:
    Object *evaluate(Object *u, IEvaluator *ev);
    
    virtual int   evaluate(int a, int b)     const = 0;
    virtual float evaluate(float a, float b) const = 0;    
};

class AddEvaluator : public BinaryEvaluator
{
public:   
    int   evaluate(int a, int b)     const { return a + b; }
    float evaluate(float a, float b) const { return a + b; }
};

class SubtractEvaluator : public BinaryEvaluator
{
public:   
    int   evaluate(int a, int b)     const { return a - b; }
    float evaluate(float a, float b) const { return a - b; }
};

class MultiplyEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a * b; }
    float evaluate(float a, float b) const { return a * b; }    
};

class DivideEvaluator : public BinaryEvaluator
{
public:   
    int   evaluate(int a, int b)     const { return a / b; }
    float evaluate(float a, float b) const { return a / b; }
};

class ModulusEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a % b; }
    float evaluate(float a, float b) const { return fmodf(a, b); }
};

class BitOrEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a | b; }
    float evaluate(float a, float b) const { return (float)evaluate((int)a, (int)b); }
};

class BitAndEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a & b; }
    float evaluate(float a, float b) const { return (float)evaluate((int)a, (int)b); }
};

class BitXorEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a ^ b; }
    float evaluate(float a, float b) const { return (float)evaluate((int)a, (int)b); }
};

class BitShiftRightEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a >> b; }
    float evaluate(float a, float b) const { return (float)evaluate((int)a, (int)b); }
};

class BitShiftLeftEvaluator : public BinaryEvaluator
{
public:
    int   evaluate(int a, int b)     const { return a << b; }
    float evaluate(float a, float b) const { return (float)evaluate((int)a, (int)b); }
};

#endif  // _ARITHMETIC_H