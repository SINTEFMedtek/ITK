/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTranslationRegistrationTransform.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef _itkTranslationRegistrationTransform_txx
#define _itkTranslationRegistrationTransform_txx

#include <itkExceptionObject.h>
#include "itkTranslationRegistrationTransform.h"


namespace itk
{

/**
 * Constructor
 */
template <class TScalarType,unsigned int NDimensions, class TParameters>
TranslationRegistrationTransform<TScalarType,NDimensions,TParameters>
::TranslationRegistrationTransform()
{ 

}


/**
 * Constructor
 */
template <class TScalarType,unsigned int NDimensions, class TParameters>
TranslationRegistrationTransform<TScalarType,NDimensions,TParameters>
::TranslationRegistrationTransform( const Self & other )
{
  m_TranslationTransform = other.m_TranslationTransform;
}


/**
 * Assignment Operator
 */
template <class TScalarType,unsigned int NDimensions, class TParameters>
const TranslationRegistrationTransform<TScalarType,NDimensions,TParameters> &
TranslationRegistrationTransform<TScalarType,NDimensions,TParameters>
::operator=( const Self & other )
{
  m_TranslationTransformation = other.m_TranslationTransformation;
  return *this;
}


/**
 * Transform a Point
 */
template <class TScalarType,unsigned int NDimensions, class TParameters>
TranslationRegistrationTransform<TScalarType,NDimensions,TParameters>::OutputPointType
TranslationRegistrationTransform<TScalarType,NDimensions,TParameters>
::Transform( const InputPointType & point ) const
{
  return m_TranslationTransform.TransformPoint( point );
}



/**
 * Set the transformation parameters
 */
template <class TScalarType,unsigned int NDimensions, class TParameters>
void
TranslationRegistrationTransform<TScalarType,NDimensions,TParameters>
::SetParameters(const ParametersType & parameters )
{

  m_Parameters = parameters;
  
  typename TranslationTransformType::VectorType  constant;
  
  // Transfer the constant part
  unsigned int par = 0;
  for(unsigned int i=0; i<NDimensions; i++) 
  {
    constant[i] = m_Parameters[par];
    ++par;
  }

  m_TranslationTransform.SetOffset( constant );

}


// Compute the Jacobian of the transformation
// It follows the same order of Parameters vector 
template<class ScalarType, unsigned int NDimensions, class TParameters>
const TranslationRegistrationTransform<ScalarType, NDimensions,TParameters>::JacobianType &
TranslationRegistrationTransform<ScalarType, NDimensions,TParameters>::
GetJacobian( const InputPointType & p ) const
{
  
  // The Jacobian of the Translation transform is an identity matrix
  m_Jacobian.SetIdentity();
  return m_Jacobian;

}



} // end namespace itk

#endif

