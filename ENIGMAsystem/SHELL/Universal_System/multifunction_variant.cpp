/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

/*
  DEVELOPERS:
  Don't use these. Really. Unless you know what you are doing. You could easily cause another
  segmentation fault if you aren't careful. And I say another because of how many I've had to
  deal with making them. Reason being, each reiteration can contain any number of pointers to
  other types or related values. For example, take hspeed, vspeed, direction, and speed: Each
  keeps tabs on the other three. When one is modified, it modifies the other three as needed.
*/

#include "var4.h"
#include "multifunction_variant.h"
#include "../Widget_Systems/widgets_mandatory.h"
#include "var_te.h"

#undef  types_extrapolate_alldec_i
#undef  types_extrapolate_alldec_ib
#define types_extrapolate_alldec_i(op, sentiments)\
 types_extrapolate_real_p  (multifunction_variant& multifunction_variant::operator op, { rval.d op x;  sentiments; return *this; } )\
 types_extrapolate_string_p(multifunction_variant& multifunction_variant::operator op, { terrortrue(); sentiments; return *this; } )\
 multifunction_variant& multifunction_variant::operator op (const variant &x)          { rval.d op x;  sentiments; return *this; }\
 multifunction_variant& multifunction_variant::operator op(const var &x)               { rval.d op x;  sentiments; return *this; }
#define types_extrapolate_alldec_ib(op, sentiments)\
 types_extrapolate_real_p  (multifunction_variant& multifunction_variant::operator op##=, { rval.d = long(rval.d) op long(x); sentiments; return *this; } )\
 types_extrapolate_string_p(multifunction_variant& multifunction_variant::operator op##=, { terrortrue();                     sentiments; return *this; } )\
 multifunction_variant& multifunction_variant::operator op##= (const variant &x)          { rval.d = long(rval.d) op long(x); sentiments; return *this; }\
 multifunction_variant& multifunction_variant::operator op##= (const var &x)              { rval.d = long(rval.d) op long(x); sentiments; return *this; }

#include <cmath> // fn

#define real enigma::vt_real
#define tstr enigma::vt_tstr

namespace enigma
{
  types_extrapolate_real_p  (multifunction_variant& multifunction_variant::operator=, { rval.d = x; type = real; function(); return *this; } )\
  types_extrapolate_string_p(multifunction_variant& multifunction_variant::operator=, { sval = x;   type = real; function(); return *this; } )\
  multifunction_variant& multifunction_variant::operator= (const variant &x)          { if ((type = x.type) == 1) sval = x.sval; else rval.d = x.rval.d; function(); return *this; }\
  multifunction_variant& multifunction_variant::operator= (const var &x)              { if ((type = (*x).type) == 1) sval = (*x).sval; else rval.d = (*x).rval.d; function(); return *this; }
  
  types_extrapolate_real_p  (multifunction_variant& multifunction_variant::operator+=, { terror(real); rval.d += x;  function(); return *this; } )\
  types_extrapolate_string_p(multifunction_variant& multifunction_variant::operator+=, { terror(tstr); sval += x;    function(); return *this; } )\
  multifunction_variant& multifunction_variant::operator+= (const variant &x)          { terror(1); if (type == 1) sval += x.sval; else rval.d += x.rval.d; function(); return *this; }\
  multifunction_variant& multifunction_variant::operator+= (const var &x)              { if (type == 1) sval += (*x).sval; else rval.d += (*x).rval.d;  function(); return *this; }
  
  types_extrapolate_alldec_i(-=,  function(); );
  types_extrapolate_alldec_i(*=,  function(); );
  
  types_extrapolate_real_p  (multifunction_variant& multifunction_variant::operator/=, { div0c(x); rval.d /= x;  function(); return *this; } )\
  types_extrapolate_string_p(multifunction_variant& multifunction_variant::operator/=, { terrortrue(); function(); return *this; } )\
  multifunction_variant& multifunction_variant::operator/= (const variant &x)          { div0c(x); rval.d /= x;  function(); return *this; }\
  multifunction_variant& multifunction_variant::operator/= (const var &x)              { div0c(x); rval.d /= x;  function(); return *this; }

  types_extrapolate_real_p  (multifunction_variant& multifunction_variant::operator%=, { div0c(x); rval.d = fmod(rval.d, x);  function(); return *this; } )\
  types_extrapolate_string_p(multifunction_variant& multifunction_variant::operator%=, { terrortrue(); function(); return *this; } )\
  multifunction_variant& multifunction_variant::operator%= (const variant &x)          { div0c(x); rval.d = fmod(rval.d, x);  function(); return *this; }\
  multifunction_variant& multifunction_variant::operator%= (const var &x)              { div0c(x); rval.d = fmod(rval.d, x);  function(); return *this; }
  
  types_extrapolate_alldec_ib(<<, function(); );
  types_extrapolate_alldec_ib(>>, function(); );
  types_extrapolate_alldec_ib(&,  function(); );
  types_extrapolate_alldec_ib(|,  function(); );
  types_extrapolate_alldec_ib(^,  function(); );
  
  #define EVCONST
  void multifunction_variant::function() {}
  
  multifunction_variant::multifunction_variant(): variant(0) {}
  types_extrapolate_real_p  (multifunction_variant::multifunction_variant,: variant(x) {})
  types_extrapolate_string_p(multifunction_variant::multifunction_variant,: variant(x) {})
  multifunction_variant::multifunction_variant(const variant &x): variant(x) {}
  multifunction_variant::multifunction_variant(const var &x): variant(x) {}
  
  multifunction_variant::~multifunction_variant() {}
}

#undef  types_extrapolate_alldec_i
