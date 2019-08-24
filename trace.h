#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define   TR_DEBUG                  0
#define   TR_INFO                   0
#define   TR_ERROR                  0
#define   __DEBUG                   1


#if TR_DEBUG
  #define tr_debug( format, args... )   fprintf( stderr, "\n %s::%s(%d) \n" format, __FILE__, __FUNCTION__,  __LINE__, ##args )
#else
  #define tr_debug( format, args... )   ((void)0)
#endif

#if TR_DEBUG
  #define tr_info( format, args... )   fprintf( stderr, "\n %s::%s(%d) \n" format, __FILE__, __FUNCTION__,  __LINE__, ##args )
#else
  #define tr_info( format, args... )   ((void)0)
#endif

#if TR_ERROR
  #define tr_error( format, args... )   fprintf( stderr, "\n %s::%s(%d) \n" format, __FILE__, __FUNCTION__,  __LINE__, ##args )
#else
  #define tr_error( format, args... )   ((void)0)
#endif

#ifndef __DEBUG
#define MBED_ASSERT(ignore)  ((void)0)
#else
#undef MBED_ASSERT
#undef __myassert
#define MBED_ASSERT(expression)  \
 ((void)((expression) ? 0 : (__myassert (#expression, __FILE__, __LINE__), 0)))

static void
__assfail(const char *format,...)
{
   va_list arg;
   static char mystderr[0x80];
   va_start(arg, format);
   (void)vsprintf(&mystderr[0], format, arg);
   va_end(arg);
}

#define __myassert(expression, file, line)  \
 __assfail("Failed assertion '%s' at line %d of '%s'.",    \
       expression, line, file)


#endif
