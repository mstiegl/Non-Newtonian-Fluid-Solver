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

#ifndef GE_REFERENCE_TRIANGLE
#define GE_REFERENCE_TRIANGLE

#include <GE_ReferencePolyhedron.hh>

/*
Reference triangle.
   
Dimension : 2

3 vertices :  index     coordinates
              -----    -------------
               0       ( 0.0, 0.0 )
               1       ( 1.0, 0.0 )
               2       ( 0.0, 1.0 )

3 faces :     index      vertices
              -----      --------
               0           0 1
               1           1 2
               2           0 2

Measure : 0.5

PUBLISHED */

class PEL_EXPORT GE_ReferenceTriangle : public GE_ReferencePolyhedron
{
   public: //------------------------------------------------------------------

   //-- Instance delivery and initialization

      static GE_ReferenceTriangle const* object( void ) ; 

   //-- Geometrical properties

      virtual bool contains( GE_Point const* pt_ref,
                             double tol = epsilon() ) const ;

      virtual bool face_contains( size_t i_face, 
                                  GE_Point const* pt_ref,
                                  double tol = epsilon() ) const ;

      virtual void project( GE_Point* pt_ref ) const ;
      
      virtual void build_neighbor( GE_Point const* pt_ref,
                                   size_t ic,
                                   GE_Point* neighbor,
                                   double& delta ) const ;

   protected: //---------------------------------------------------------------

   private: //-----------------------------------------------------------------

      GE_ReferenceTriangle( void ) ;
     ~GE_ReferenceTriangle( void ) ;
      GE_ReferenceTriangle( GE_ReferenceTriangle const& other ) ;
      GE_ReferenceTriangle& operator=( GE_ReferenceTriangle const& other ) ;

   //-- Class attributes

      static GE_ReferenceTriangle* UNIQUE_INSTANCE ;    
      
} ;

#endif