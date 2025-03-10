//C- Copyright � 1999-2001 LizardTech, Inc. All Rights Reserved.
//C- 
//C- This software (the "Original Code") is subject to, and may be
//C- distributed under, the GNU General Public License, Version 2.
//C- You may obtain a copy of the license from the Free Software
//C- Foundation at http://www.fsf.org .
//C- 
//C- With respect to the Original Code, and subject to any third party
//C- intellectual property claims, LizardTech grants recipient a worldwide,
//C- royalty-free, non-exclusive license under patent claims infringed by
//C- making, using, or selling Original Code which are now or hereafter
//C- owned or controlled by LizardTech, but solely to the extent that any
//C- such patent is reasonably necessary to enable you to make, have made,
//C- practice, sell, or otherwise dispose of Original Code (or portions
//C- thereof) and not to any greater extent that may be necessary to utilize
//C- further modifications or combinations.
//C- 
//C- The Original Code is provided "AS IS" WITHOUT WARRANTY OF ANY KIND,
//C- EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY WARRANTY
//C- OF NON-INFRINGEMENT, OR ANY IMPLIED WARRANTY OF MERCHANTIBILITY OF
//C- FITNESS FOR A PARTICULAR PURPOSE.
//C-


#ifndef _GEXCEPTION_H_
#define _GEXCEPTION_H_

#include <stdexcept>

/** @name GException.h

    Files #"GException.h"# and #"GException.cpp"# define a portable exception
    scheme used through the DjVu Reference Library. This scheme can use native
    C++ exceptions or an exception emulation based on #longjmp#/#setjmp#. A
    particular model can be forced a compile time by defining option
    #CPP_SUPPORTS_EXCEPTIONS# or #USE_EXCEPTION_EMULATION#.
    
    This emulation code was motivated because many compilers did not properly
    support exceptions as mandated by the C++ standard documents. This
    emulation is now considered obsolete because (a) it is not able to call
    the proper destructors when an exception occurs, and (b) it is not thread
    safe.  Although all modern C++ compiler handle exception decently, the
    exception handling intrinsics are not always thread safe.  Therefore we
    urge programmers to {\em only} use exceptions to signal error conditions
    that force the library to discontinue execution.
    
    There are four macros for handling exceptions.  Macros #TRY#, #CATCH# and
    #ENDCATCH# are used to define an exception catching block.  Exceptions can
    be thrown at all times using macro #THROW(cause)#. An exception can be
    re-thrown from a catch block using macro #RETHROW#.
    
    Example:
    \begin{verbatim}
    TRY
      {
        // program lines which may result in a call to THROW()
        THROW("message");
      }
    CATCH(ex) 
      {
        // Variable ex refers to a GException object.
        ex.perror();  
        // You can rethrow the exception to an outer exception handler.
        RETHROW;
      }
    ENDCATCH;
    \end{verbatim} 

    @memo 
    Portable exceptions.
    @author 
    L\'eon Bottou <leonb@research.att.com> -- initial implementation.\\
    Andrei Erofeev <eaf@research.att.com> -- fixed message memory allocation.
    @version 
    #$Id: GException.h,v 1.2 2001-01-04 22:04:53 bcr Exp $# */
//@{


/** Exception class.  
    The library always uses macros #TRY#, #THROW#, #CATCH# and #ENDCATCH# for
    throwing and catching exceptions (see \Ref{GException.h}). These macros
    only deal with exceptions of type #GException#. */

class GException : public std::exception {
public:
  /** Constructs a GException.  This constructor is usually called by macro
      #THROW#.  Argument #cause# is a plain text error message. As a
      convention, string #"EOF"# is used when reaching an unexpected
      end-of-file condition and string #"STOP"# is used when the user
      interrupts the execution. The remaining arguments are usually provided
      by the predefined macros #__FILE__#, #__LINE__#, and (G++ and EGCS only)
      #__PRETTY_FUNCTION__#.  */
  GException (const char *cause, const char *file=0, int line=0, const char *func=0);
  /** Copy Constructor. */
  GException (const GException & exc);
  /** Null Constructor. */
  GException ();
  /** Destructor. */
  virtual ~GException(void) throw();
  /* Inherited */
  virtual const char *what() const throw();
  /** Copy Operator. */
  GException & operator=(const GException & exc);
  /** Prints an error message on stderr.
      @param msg: string incorporated into the error message. */
  void perror(const char *msg = 0) const;
  /** Returns the string describing the cause of the exception.  The returned
      pointer is never null.  Exception handlers should not rely on the value
      of the string #cause#.  As a convention however, string #"EOF"# is used
      when reaching an unexpected end-of-file condition and string #"STOP"# is
      used when the user interrupts the execution. These strings can be tested
      by the exception handlers. Similar conventional strings may be defined
      in the future. They all will be small strings with only uppercase
      characters. */
  const char* get_cause(void) const;
  /** Returns the function name from which the exception was thrown.
      A null pointer is returned if no function name is available. */
  const char* get_function(void) const { return func; };
  /** Returns the file name from which the exception was thrown.
      A null pointer is returned if no file name is available. */
  const char* get_file(void) const { return file; };
  /** Returns the line number from which the exception was thrown.
      A zero is returned if no line number is available. */
  int get_line(void) const { return line; };
  //  Magic cause string
  static const char * const outofmemory;
private:
  const char *cause;
  const char *file;
  const char *func;
  int line;
};

//@}

#undef G_TRY
#undef G_CATCH
#undef G_ENDCATCH
#undef G_RETHROW
#undef G_THROW

//LB2025// modern cpp support exceptions
#define CPP_SUPPORTS_EXCEPTIONS


// Compiler supports ANSI C++ exceptions.
// Defined exception macros accordingly.

#define G_TRY        try
#define G_CATCH(n)   catch(GException &n) { 
#define G_ENDCATCH   }
#define G_RETHROW    throw
#ifdef __GNUG__
#define G_THROW(msg) throw \
  GException(msg, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#else
#define G_THROW(msg) throw \
  GException(msg, __FILE__, __LINE__)
#endif

#define TRY G_TRY
#define CATCH G_CATCH
#define ENDCATCH G_ENDCATCH
#define RETHROW G_RETHROW
#define THROW G_THROW



// -------------- THE END
#endif
