/*
 *  Copyright 1995-2010 by IRSN
 *
 *  This software is an application framework, with a set of integrated  
 *  reusable components, whose purpose is to simplify the task of developing 
 *  softwares of numerical mathematics and scientific computing.
 * 
 *  This software is governed by the CeCILL-C license under French law and 
 *  abiding by the rules of distribution of free software. You can use, modify 
 *  and/or redistribute the software under the terms of the CeCILL-C license  
 *  as circulated by CEA, CNRS and INRIA at the following URL 
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and rights to copy,  
 *  modify and redistribute granted by the license, users are provided only 
 *  with a limited warranty and the software's author, the holder of the  
 *  economic rights, and the successive licensors have only limited liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated  
 *  with loading, using, modifying and/or developing or reproducing the  
 *  software by the user in light of its specific status of free software,
 *  that may mean that it is complicated to manipulate, and that also  
 *  therefore means that it is reserved for developers and experienced 
 *  professionals having in-depth computer knowledge. Users are therefore 
 *  encouraged to load and test the software's suitability as regards their 
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and, more generally, to use and operate it in the same 
 *  conditions as regards security. 
 *
 *  The fact that you are presently reading this means that you have had 
 *  knowledge of the CeCILL-C license and that you accept its terms.
 */

#ifndef PEL_HH
#define PEL_HH

#include <cstddef>
#include <iosfwd>
#include <string>

#include <PEL_export.hh>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

/*
PUBLISHED
*/

class PEL_EXPORT PEL
{
   public: //-----------------------------------------------------------------

   //-- Usefull numeric constants
      
      static double pi( void ) ;

      static double e( void ) ;

      static double euler( void ) ;
      
      // difference between 1 and the least value greater than 1 that is
      // representable
      // IMPLEMENTATION : std::numeric_limits<double>::epsilon()
      //    (eg on some machines : 2.22044...e-16)
      static double epsilon_double( void ) ;
      
      // minimal representable no zero double value
      // IMPLEMENTATION : std::numeric_limits<double>::min()
      //    (eg on some machines : 2.22507...e-308)
      static double min_double( void ) ;

      // maximum finite double value
      // IMPLEMENTATION : std::numeric_limits<double>::max()
      //    (eg on some machines : 1.79769...e+308)
      static double max_double( void ) ;

      // maximum finite int value
      // IMPLEMENTATION : std::numeric_limits<int>::max()
      static int max_int( void ) ;
      
      // IMPLEMENTATION : static_cast<size_t>(~0)
      static size_t bad_index( void ) ;

      // IMPLEMENTATION : std::numeric_limits<int>::max()
      static int bad_int( void ) ;

      // IMPLEMENTATION : std::numeric_limits<double>::max()
      static double bad_double( void ) ;

   //-- Power functions

      // `x'*`x'
      static double sqr( double const x ) ;

      // IMPLEMENTATION : std::sqrt( `x' )
      static double sqrt( double const x ) ;

      // `x' power `exp'
      // IMPLEMENTATION : std::pow( `x', `exp' )
      static double pow( double const x, double const exp ) ;

      // IMPLEMENTATION : std::exp( `x' )
      static double exp( double const x ) ;

      // IMPLEMENTATION : std::log( `x' )
      static double log( double const x ) ;

      // IMPLEMENTATION : std::log10( `x' )
      static double log10( double const x ) ;
      
   //-- Trigonometric functions (in radians)

      // IMPLEMENTATION : std::sin( `x' )
      static double sin( double const x ) ;

      // IMPLEMENTATION : std::cos( `x' )
      static double cos( double const x ) ;

      // IMPLEMENTATION : std::tan( `x' )
      static double tan( double const x ) ;

      // IMPLEMENTATION : std::asin( `x' )
      static double asin( double const x ) ;

      // IMPLEMENTATION : std::acos( `x' )
      static double acos( double const x ) ;

      // IMPLEMENTATION : std::atan( `x' )
      static double atan( double const x ) ;

      // angle in radian of bidimensional vector (`x',`y')
      // IMPLEMENTATION : std::atan2( `x', `y' )
      static double atan2( double const x, double const y ) ;

   //-- Hyperbolic functions (in radians)

      // IMPLEMENTATION : std::sinh( `x' )
      static double sinh( double const x ) ;

      // IMPLEMENTATION : std::cosh( `x' )
      static double cosh( double const x ) ;

      // IMPLEMENTATION : std::tanh( `x' )
      static double tanh( double const x ) ;

   //-- Comparison functions
      
      static int    max( int const& x, int const& y ) ;
      static size_t max( size_t const& x, size_t const& y ) ;
      static double max( double const& x, double const& y ) ;
      
      static int    min( int const& x, int const& y ) ;
      static size_t min( size_t const& x, size_t const& y ) ;
      static double min( double const& x, double const& y ) ;

      /*
      Are `x' and `y' close enough ? 
         (1) if( x == y ) return true 
             (in that case `a_dbl_min' and `a_dbl_max' can be exactly zero)
         (2) if |`x'|<`a_dbl_min' and |`y'|<`a_dbl_min' return true
         (3) if |`x'|>=`a_dbl_min' and |`y'|>=`a_dbl_min' :
                . return true if  1.0-`a_dbl_eps'<`x'/`y'< 1.0+`a_dbl_eps'
                  (overflow and underflow when evaluating `x'/`y' are handled)
                . return false otherwise
         (4) if |`x'|<`a_dbl_min' and |`y'|>=`a_dbl_min'
             return result given by (3) with `x' replaced by `a_dbl_min'
         (5) if |`y'|<`a_dbl_min' and |`x'|>=`a_dbl_min'
             return result given by (3) with `y' replaced by `a_dbl_min'

      stated differently :

         [1] `a_dbl_min' represents the lower bound under which `x' or `y'
             are undistinguishable from 0

         [2] if both `x' and `y' are undistinguishable from 0, they
             are close enough

         [3] if both `x' and `y' are distinguishable from 0, they are close 
             enough provided that |`x'/`y'-1.0| is lower than `a_dbl_eps'

         [4] if one of `x' or `y' is undistinguishable from 0, the other is 
             compared to `a_dbl_min' as in [3]
      */
      static bool double_equality( double const x, double const y,
                                   double const a_dbl_eps, 
                                   double const a_dbl_min ) ;

      /*
      Are `x' and `y' close enough, according to
         `::double_equality'( `x', `y', `a_dbl_eps', `a_dbl_min' ) ?
      if true, `status' is equal to 0.0
      if false because of an unsuccessful comparision of a relative
         error to `a_dbl_eps', `status' is equal to that relative error ;
         if false for another reason, `status' is equal to -1.0
      */
      static bool double_equality( double const x, double const y,
                                   double const a_dbl_eps, 
                                   double const a_dbl_min,
                                   double& status ) ;

   //-- Rounding functions
      
      // smallest integer not less than `x'
      // IMPLEMENTATION : std::ceil( `x' )
      static double ceil( double const x ) ;

      // largest integer not greater than `x'
      // IMPLEMENTATION : std::floor( `x' )
      static double floor( double const x ) ;

      static double abs( double const& x ) ;
      static size_t abs( int const& x ) ;

   //-- Extended mathematic functions (not supported on all system)

      // IMPLEMENTATION : ::asinh( `x' )
      static double asinh( double const x ) ;

      // IMPLEMENTATION : ::acosh( `x' )
      static double acosh( double const x ) ;

      // IMPLEMENTATION : ::atanh( `x' )
      static double atanh( double const x ) ;

      // First order Bessel function : n=0
      // IMPLEMENTATION : ::j0( `x' )
      static double j0( double const x ) ;
      
      // First order Bessel function : n=1
      // IMPLEMENTATION : ::j1( `x' )
      static double j1( double const x ) ;
      
      // First order Bessel function
      // IMPLEMENTATION : ::jn( `n', `x' )
      static double jn( int const n, double const x ) ;

      // Second order Bessel function : n=0
      // IMPLEMENTATION : ::y0( `x' )
      static double y0( double const x ) ;
      
      // Second order Bessel function : n=1
      // IMPLEMENTATION : ::y1( `x' )
      static double y1( double const x ) ;
      
      // Second order Bessel function
      // IMPLEMENTATION : ::yn( `n', `x' )
      static double yn( int const n, double const x ) ;

      // gamma function, ie sum(0,+oo){ t^(x-1) . exp( -t ) }
      // IMPLEMENTATION : compute from ::lgamma( `x' )
      static double gamma( double const x ) ;
      
      // logarithm of gamma function absolute value
      // IMPLEMENTATION : ::lgamma( `x' )
      static double lgamma( double const x ) ;

      // IMPLEMENTATION : ::erf( `x' )
      static double erf( double const x ) ;

      // IMPLEMENTATION : ::erfc( `x' )
      static double erfc( double const x ) ;

   //-- Special Functions (from Numerical Recipies)

      // incomplete gamma function, ie sum(t=0,x){ t^(a-1) . exp( -t ) }
      static double incomplete_gamma( double const a, double const x,
                                      double const epsilon_min=1.E-30,
                                      double const error_bound=1.E-14,
                                      size_t const iteration_max=100 ) ;

      // En(x), ie sum(t=1,+oo){ exp( -xt )/t^n }
      static double En( size_t const n, double const x,
                        double const epsilon_min=1.E-30,
                        double const error_bound=1.E-7,
                        size_t const iteration_max=100 ) ;
      
      // Ei(x), ie sum(t=-x,+oo){ exp( -t )/t }
      static double Ei( double const x,
                        double const epsilon_min=1.E-30,
                        double const error_bound=6.E-8,
                        size_t const iteration_max=100 ) ;
      
      // random number between 0.0 and 1.0
      static double random_double( void ) ;
      
      // random integer between 0 and max_int()
      static int rand( void ) ;
      
   //-- String functions

      // Replace each occurence of `src' in `str' by `dest'.
      static void replace( std::string& str,
                           std::string const& src,
                           std::string const& dest ) ;

      // Remove first and last character of `str' if equal to `car'.
      static void remove_enclosing_characters( std::string& str,
                                               char const car ) ;
      
   //-- Input - Output
      
      // Pelicans standard output stream.
      static std::ostream& out( void ) ;
      
      // Pelicans standard error stream.
      static std::ostream& err( void ) ;
      
      // Pelicans standard input stream.
      static std::istream& in( void ) ;

      // Print `data' in `os' stream.
      static void print_double( std::ostream& os, double data ) ;
      
   //-- Hidden

      // legacy and obsolete function : should not be used
      static double relative( double const v1, double const v2 ) ;
      
      // legacy and obsolete function : should not be used
      static bool toler( double const val,
                         double const eps = 1.E-14 ) ;

      // legacy and obsolete function : should not be used
      // (use double_equality instead)
      static bool equal( double const val1, double const val2,
                         double const eps = 1.E-14 ) ;

   protected: //--------------------------------------------------------------

   private: //----------------------------------------------------------------

      PEL( void ) ;
     ~PEL( void ) ;
      PEL( PEL const& other ) ;
      PEL& operator=( PEL const& other ) ;

   //-- Internals

      static bool ratio_close_to_one( double const x, double const y,
                                      double const a_dbl_eps,
                                      double& status ) ;
      
} ;

#ifndef OUTLINE
   #include <PEL.icc>
#endif

#endif
