// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// f32test\locl\t_unic.cpp
// The LCharSet object used by the default locale in the Unicode build.
// 
//

#include <kernel/localise.h>

/*
The whole file is protected by #ifdef _UNICODE so that it can be safely added to the non-Unicode build.
This is done after the include files so that _UNICODE is defined if necessary.
*/
#ifdef _UNICODE
#include <collate.h>

static const TCollationMethod TheCollationMethod[] =
	{
		{
		KUidBasicCollationMethod,				// this is the standard unlocalised method
		NULL,									// null means use the standard table
		NULL,									// there's no override table
		0										// the flags are standard
		}
	};

static const TCollationDataSet TheCollationDataSet =
	{
	TheCollationMethod,
	1
	};

// The one and only locale character set object.
const LCharSet TheCharSet =
	{
	NULL,
	&TheCollationDataSet
	};

#endif // _UNICODE
