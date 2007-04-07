/* 64bit counter. */

typedef struct Counter_Struct COUNTER;

struct Counter_Struct
{
  unsigned long high, low;
};

#ifndef DBL
#define DBL double
#endif

#define Max_Distance 1.0e7
#define Increase_Counter(x) { if ((++(x).low) == 0L) { (x).high++; } }
