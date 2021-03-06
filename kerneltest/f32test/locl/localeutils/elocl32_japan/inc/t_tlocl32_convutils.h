/*
* Copyright (c) 2000-2010 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/



#if !defined(__CONVUTILS_H__)
#define __CONVUTILS_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__CHARCONV_H__)
#include "t_tlocl32_charconv.h"
#endif

struct SCnvConversionData;

 
class CnvUtilities
/** 
Provides static character conversion utilities for complex encodings. Its functions 
may be called from a plug-in DLL's implementation of ConvertFromUnicode() 
and ConvertToUnicode().

These utility functions are provided for use when converting to/from complex 
character set encodings, including modal encodings. Modal encodings are those 
where the interpretation of a given byte of data is dependent on the current 
mode; mode changing is performed by escape sequences which occur in the byte 
stream. A non-modal complex encoding is one in which characters are encoded 
using variable numbers of bytes. The number of bytes used to encode a character 
depends on the value of the initial byte.
@publishedAll 
@released
*/
	{
public:
	// type definitions for converting from Unicode
	
	/**  A pointer to a function which "mangles" text when converting from
	Unicode into a complex modal or non-modal foreign character set
	encoding.

	It might insert a shifting character, escape sequence, or other
	special characters.If the target character set encoding is modal, the
	implementation of this function may call the
	CnvUtilities::ConvertFromIntermediateBufferInPlace()
	utility function which is provided because many modal character sets
	require an identical implementation of this function.

	" convutils.lib " */
    typedef void (*FConvertFromIntermediateBufferInPlace)(TInt aStartPositionInDescriptor, TDes8& aDescriptor, TInt& aNumberOfCharactersThatDroppedOut);
	struct SCharacterSet
	/** Stores information about a non-Unicode character set. The information 
	is used to locate the conversion information required by 
	ConvertFromUnicode() and ConvertToUnicode().

	An array of these structs  that contains all available character sets  
	can be generated by CreateArrayOfCharacterSetsAvailableLC() and 
	CreateArrayOfCharacterSetsAvailableL(), and is used by one of the 
	overloads of PrepareToConvertToOrFromL(). */
		{
		/** The conversion data. */
		const SCnvConversionData* iConversionData; // must *not* be set to NULL
		/** A pointer to a function which "mangles" the text in a way 
		appropriate to the target complex character set. For instance it 
		might insert a shifting character, escape sequence, or other special 
		characters. */
		FConvertFromIntermediateBufferInPlace iConvertFromIntermediateBufferInPlace; // must *not* be set to NULL
		/** The escape sequence which introduces the character set, i.e. it 
		identifies this character set as the next one to use. Must not be NULL.
		If the character set is non-modal, this should be set to an empty 
		descriptor. */
		const TDesC8* iEscapeSequence; // must *not* be set to NULL
		};
	// type definitions for converting to Unicode

	/** A pointer to a function which calculates the number of consecutive
	bytes in the remainder of the foreign descriptor which can be
	converted using the current character set's conversion data.

	Called when converting from a non-modal complex character set encoding
	into Unicode. It may return a negative
	CCnvCharacterSetConverter::TError value to indicate an
	error in the encoding.

	" convutils.lib " */
	typedef TInt (*FNumberOfBytesAbleToConvert)(const TDesC8& aDescriptor); // may return negative CCnvCharacterSetConverter::TError values
	
	/** A pointer to a function which prepares the text for conversion into
	Unicode.

	For instance it might remove any shifting or other special characters.
	Called when converting from a non-modal complex character set encoding
	into Unicode.

	" convutils.lib " */
 	typedef void (*FConvertToIntermediateBufferInPlace)(TDes8& aDescriptor);

	struct SState
	/** Character conversion data for one of the character sets which is 
	specified in a modal character set encoding. An array of these structs 
	is used when converting from a modal character set into Unicode, using 
	CnvUtilities::ConvertToUnicodeFromModalForeign(). Neither of the members 
	may be NULL. */
		{
		/** The escape sequence which introduces the character set, i.e. it 
		identifies this character set as the next one to use. This must begin 
		with KControlCharacterEscape. */
		const TDesC8* iEscapeSequence; // must *not* be set to NULL and must begin with 0x1b
		/** The conversion data. */
		const SCnvConversionData* iConversionData; // must *not* be set to NULL
		};
	struct SMethod
		{
		/** A pointer to a function which calculates the number of consecutive 
		bytes in the remainder of the foreign descriptor which can be converted 
		using the current character set's conversion data. It may return a 
		negative CCnvCharacterSetConverter::TError value to indicate an error 
		in the encoding. */
		FNumberOfBytesAbleToConvert iNumberOfBytesAbleToConvert; // must *not* be set to NULL
		/** A pointer to a function which prepares the text for conversion 
		into Unicode. For instance it might remove any shifting or other 
		special characters. */
		FConvertToIntermediateBufferInPlace iConvertToIntermediateBufferInPlace; // must *not* be set to NULL
		/** The conversion data. */
		const SCnvConversionData* iConversionData; // must *not* be set to NULL
		/** The number of bytes per character. */
		TInt16 iNumberOfBytesPerCharacter;
		/** The number of core bytes per character. */
		TInt16 iNumberOfCoreBytesPerCharacter;
		};
public:
	// these functions may *not* have CCnvCharacterSetConverter::EInputConversionFlagStopAtFirstUnconvertibleCharacter set in aInputConversionFlags
	IMPORT_C static TInt ConvertFromUnicode(CCnvCharacterSetConverter::TEndianness aDefaultEndiannessOfForeignCharacters, const TDesC8& aReplacementForUnconvertibleUnicodeCharacters, TDes8& aForeign, const TDesC16& aUnicode, CCnvCharacterSetConverter::TArrayOfAscendingIndices& aIndicesOfUnconvertibleCharacters, const TArray<SCharacterSet>& aArrayOfCharacterSets);
	IMPORT_C static TInt ConvertFromUnicode(CCnvCharacterSetConverter::TEndianness aDefaultEndiannessOfForeignCharacters, const TDesC8& aReplacementForUnconvertibleUnicodeCharacters, TDes8& aForeign, const TDesC16& aUnicode, CCnvCharacterSetConverter::TArrayOfAscendingIndices& aIndicesOfUnconvertibleCharacters, const TArray<SCharacterSet>& aArrayOfCharacterSets, TUint& aOutputConversionFlags, TUint aInputConversionFlags);
	IMPORT_C static void ConvertFromIntermediateBufferInPlace(TInt aStartPositionInDescriptor, TDes8& aDescriptor, TInt& aNumberOfCharactersThatDroppedOut, const TDesC8& aEscapeSequence, TInt aNumberOfBytesPerCharacter);
	IMPORT_C static TInt ConvertToUnicodeFromModalForeign(CCnvCharacterSetConverter::TEndianness aDefaultEndiannessOfForeignCharacters, TDes16& aUnicode, const TDesC8& aForeign, TInt& aState, TInt& aNumberOfUnconvertibleCharacters, TInt& aIndexOfFirstByteOfFirstUnconvertibleCharacter, const TArray<SState>& aArrayOfStates); // the first element of aArrayOfStates is taken to be the default state
	IMPORT_C static TInt ConvertToUnicodeFromModalForeign(CCnvCharacterSetConverter::TEndianness aDefaultEndiannessOfForeignCharacters, TDes16& aUnicode, const TDesC8& aForeign, TInt& aState, TInt& aNumberOfUnconvertibleCharacters, TInt& aIndexOfFirstByteOfFirstUnconvertibleCharacter, const TArray<SState>& aArrayOfStates, TUint& aOutputConversionFlags, TUint aInputConversionFlags); // the first element of aArrayOfStates is taken to be the default state
	IMPORT_C static TInt ConvertToUnicodeFromHeterogeneousForeign(CCnvCharacterSetConverter::TEndianness aDefaultEndiannessOfForeignCharacters, TDes16& aUnicode, const TDesC8& aForeign, TInt& aNumberOfUnconvertibleCharacters, TInt& aIndexOfFirstByteOfFirstUnconvertibleCharacter, const TArray<SMethod>& aArrayOfMethods);
	IMPORT_C static TInt ConvertToUnicodeFromHeterogeneousForeign(CCnvCharacterSetConverter::TEndianness aDefaultEndiannessOfForeignCharacters, TDes16& aUnicode, const TDesC8& aForeign, TInt& aNumberOfUnconvertibleCharacters, TInt& aIndexOfFirstByteOfFirstUnconvertibleCharacter, const TArray<SMethod>& aArrayOfMethods, TUint& aOutputConversionFlags, TUint aInputConversionFlags);
private:
	static void CheckArrayOfCharacterSets(const TArray<SCharacterSet>& aArrayOfCharacterSets);
	static void CheckArrayOfStates(const TArray<SState>& aArrayOfStates);
	static void CheckArrayOfMethods(const TArray<SMethod>& aArrayOfMethods);
	static TInt LengthOfUnicodeCharacter(const TDesC16& aUnicode, TInt aIndex);
	static TBool NextHomogeneousForeignRun(const SCnvConversionData*& aConversionData, TInt& aNumberOfForeignBytesConsumed, TPtrC8& aHomogeneousRun, TPtrC8& aRemainderOfForeign, const TArray<SState>& aArrayOfStates, TUint& aOutputConversionFlags);
	static TBool MatchesEscapeSequence(TInt& aNumberOfForeignBytesConsumed, TPtrC8& aHomogeneousRun, TPtrC8& aRemainderOfForeign, const TDesC8& aEscapeSequence);
	static TBool IsStartOf(const TDesC8& aStart, const TDesC8& aPotentiallyLongerDescriptor);
	inline static TInt ReduceToNearestMultipleOf(TInt aNumber1, TInt aNumber2) {return (aNumber1/aNumber2)*aNumber2;}
	};

#endif

