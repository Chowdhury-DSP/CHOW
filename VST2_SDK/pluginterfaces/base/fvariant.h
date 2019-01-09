//-----------------------------------------------------------------------------
// Project     : SDK Core
//
// Category    : SDK Core Interfaces
// Filename    : pluginterfaces/base/fvariant.h
// Created by  : Steinberg, 01/2004
// Description : Basic Interface
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2014, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety
// without prior written agreement by Steinberg Media Technologies GmbH.
// This SDK must not be used to re-engineer or manipulate any technology used
// in any Steinberg or Third-party application or software module,
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/fstrdefs.h"

//------------------------------------------------------------------------
namespace Steinberg {

//------------------------------------------------------------------------
//  FVariant struct declaration
//------------------------------------------------------------------------
/** A Value of variable type.
 \ingroup pluginBase
 */
class FVariant
{
//------------------------------------------------------------------------
public:
	enum
	{
		kEmpty = 0,
		kInteger = 1 << 0,
		kFloat = 1 << 1,
		kString8 = 1 << 2,
		kObject = 1 << 3,
		kOwner = 1 << 4,
		kString16 = 1 << 5
	};

//------------------------------------------------------------------------
	// ctors
	inline FVariant () { memset (this, 0, sizeof (FVariant)); }
	inline FVariant (const FVariant& variant);

	inline FVariant (int64 v) : type (kInteger), intValue (v) {}
	inline FVariant (double v) : type (kFloat), floatValue (v) {}
	inline FVariant (const char8* str) : type (kString8), string8 (str) {}
	inline FVariant (const char16* str) : type (kString16), string16 (str) {}
	inline FVariant (FUnknown* obj, bool owner = false) : type (kObject), object (obj)
	{
		setOwner (owner);
	}
	inline ~FVariant () { empty (); }

//------------------------------------------------------------------------
	inline FVariant& operator= (const FVariant& variant);

	inline void setInt (int64 v)
	{
		empty ();
		type = kInteger;
		intValue = v;
	}
	inline void setFloat (double v)
	{
		empty ();
		type = kFloat;
		floatValue = v;
	}
	inline void setString8 (const char8* v)
	{
		empty ();
		type = kString8;
		string8 = v;
	}
	inline void setString16 (const char16* v)
	{
		empty ();
		type = kString16;
		string16 = v;
	}

	inline void setObject (FUnknown* obj)
	{
		empty ();
		type = kObject;
		object = obj;
	}

	inline int64 getInt () const { return (type & kInteger) ? intValue : 0; }
	inline double getFloat () const { return (type & kFloat) ? floatValue : 0.; }
	inline double getNumber () const
	{
		return (type & kInteger) ? static_cast<double> (intValue) : (type & kFloat) ? floatValue :
																					  0.;
	}
	inline const char8* getString8 () const { return (type & kString8) ? string8 : 0; }
	inline const char16* getString16 () const { return (type & kString16) ? string16 : 0; }

	inline FUnknown* getObject () const { return (type & kObject) ? object : 0; }

	inline uint16 getType () const { return static_cast<uint16> (type & ~(kOwner)); }
	inline bool isEmpty () const { return getType () == kEmpty; }
	inline bool isOwner () const { return (type & kOwner) != 0; }
	inline bool isString () const { return (type & (kString8 | kString16)) != 0; }
	inline void setOwner (bool state)
	{
		if (state)
			type |= kOwner;
		else
			type &= ~kOwner;
	}

	void empty ();
//------------------------------------------------------------------------
	uint16 type;
	union
	{
		int64 intValue;
		double floatValue;
		const char8* string8;
		const char16* string16;
		FUnknown* object;
	};
};

//------------------------------------------------------------------------
inline bool operator== (const FVariant& v1, const FVariant& v2)
{
#if PLATFORM_64
	return v1.type == v2.type && v1.intValue == v2.intValue;
#else
	if (v1.type != v2.type)
		return false;
	if (v1.type & (FVariant::kString8 | FVariant::kString16 | FVariant::kObject))
		return v1.string8 == v2.string8; // pointer type comparisons
	return v1.intValue == v2.intValue; // intValue & double comparison

#endif
}

//------------------------------------------------------------------------
inline bool operator!= (const FVariant& v1, const FVariant& v2) { return !(v1 == v2); }

//------------------------------------------------------------------------
inline FVariant::FVariant (const FVariant& variant) : type (kEmpty) { *this = variant; }

//------------------------------------------------------------------------
inline void FVariant::empty ()
{
	if (type & kOwner)
	{
		if ((type & kString8) && string8)
			delete[] string8;
		else if ((type & kString16) && string16)
			delete[] string16;

		else if ((type & kObject) && object)
			object->release ();
	}
	memset (this, 0, sizeof (FVariant));
}

//------------------------------------------------------------------------
inline FVariant& FVariant::operator= (const FVariant& variant)
{
	empty ();

	type = variant.type;

	if ((type & kString8) && variant.string8)
	{
		string8 = new char8[strlen (variant.string8) + 1];
		strcpy (const_cast<char8*> (string8), variant.string8);
		type |= kOwner;
	}
	else if ((type & kString16) && variant.string16)
	{
		int32 len = strlen16 (variant.string16);
		string16 = new char16[len + 1];
		char16* tmp = const_cast<char16*> (string16);
		memcpy (tmp, variant.string16, len * sizeof (char16));
		tmp[len] = 0;
		type |= kOwner;
	}
	else if ((type & kObject) && variant.object)
	{
		object = variant.object;
		object->addRef ();
		type |= kOwner;
	}
	else
		intValue = variant.intValue; // copy memory

	return *this;
}

//------------------------------------------------------------------------
} // Steinberg
