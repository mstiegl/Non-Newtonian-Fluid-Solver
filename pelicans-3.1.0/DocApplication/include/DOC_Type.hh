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

#ifndef TYPE_HH
#define TYPE_HH

#include <iostream>
#include <string>
#include <DOC_Writer.hh>
#include <DOC_Symbol.hh>

// Definition d'un type.

class PEL_EXPORT DOC_Type : public DOC_Symbol
{
   public: //--------------------------------------------------

  //-- Assignment attempt
      
      virtual DOC_Type* to_type( void ) ;
      virtual void display( ostream& out ) const ;

   //-- Creation
      
      static DOC_Type* create( PEL_Object* a_owner,
                           string const& a_name ) ;
   //-- Status
      
      std::string const& name( void ) const ;
      std::string const& full_type_name( void ) const ;     
      std::string modifiers( void ) const ;
      std::string type_reference( DOC_Writer const& sullizer ) const ;
      
   //-- Modifier
      
      void set_pointer( void ) ;
      void set_reference( void ) ;
      void set_constant( void ) ;
      
   //-- Comparison
      virtual bool is_equal( PEL_Object const* other ) const ;
      
   protected: //-----------------------------------------------

   private: //-------------------------------------------------

      DOC_Type( PEL_Object* a_owner,
            std::string const& a_name ) ;
      ~DOC_Type( void ) ;

      DOC_Type& operator=( DOC_Type const& t ) ;
      DOC_Type( DOC_Type const& t ) ;
      DOC_Type( void ) ;
      
      string identificateur ;
      bool estPointeur ;
      bool estReference ;
      bool is_constant ;
      mutable string type_complet ;

} ;

#endif

      