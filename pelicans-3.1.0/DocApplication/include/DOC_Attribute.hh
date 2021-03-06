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

#ifndef ATTRIBUTE_HH
#define ATTRIBUTE_HH

#include <iostream>
#include <string>

#include <DOC_ClassItem.hh>


class DOC_Type ;
class DOC_Class ;
class DOC_Writer ;

// Attribute of a class representation.

class PEL_EXPORT DOC_Attribute : public DOC_ClassItem
{
   public: //-------------------------------------------------

   //-- Assignment attempt
      virtual DOC_Attribute* attribute( void ) ;
      
   //-- Creation
      static DOC_Attribute* create( std::string const& a_name,
                               DOC_Type* typ,
                               Protection laProtection,
                               DOC_Category const* laDOC_Category,
                               DOC_Text const* leComment ) ;
   //-- Modifier(10.0)
      void set_static( void ) ;
      void initialize( DOC_Symbol const* s ) ;

   //-- Status
      
      virtual std::string const& name( void ) const ;
      
      DOC_Type const* type( void ) const ;
      bool is_static( void ) const ;
      
   //-- Input - Output
      virtual std::string signature( void ) const ;
      virtual std::string prototype( DOC_Writer& sullizer ) const ;
      virtual void display( std::ostream& out ) const ;
      virtual void display_info( std::ostream& os, size_t indent_width ) const ;

   //-- Comparison
      virtual bool is_equal( PEL_Object const* other ) const ;

   protected: //-----------------------------------------------

      DOC_Attribute( std::string const& a_name,
		    DOC_Type* typ,
		    Protection laProtection,
                    DOC_Category const* laDOC_Category,
		    DOC_Text const* leComment ) ;
      virtual ~DOC_Attribute( void ) ;
   private: //-------------------------------------------------

      DOC_Attribute( void ) ;
      DOC_Attribute( DOC_Attribute const& other ) ;
      DOC_Attribute& operator=( DOC_Attribute const& DOC_Attribute ) ;
      
      std::string myName ;
      DOC_Type * myDOC_Type ;
      bool estStatique ;
      DOC_Symbol const* init ;
      
} ;

#endif
