/*
 
 Copyright (c) 2019 rokudogobu
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 */

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

#include "QLCMFunctions.h"
#include "QLCMPreferences.h"

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options);
void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview);

/* -----------------------------------------------------------------------------
 Generate a preview for file
 
 This function's job is to create preview for designated file
 ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
    QLCMPreferencesLoadForPreviewRequest( preview );
    
    CFDataRef       data        = QLCMCreateHTMLDataFromCommonMarkFile( url );
    CFDictionaryRef properties  = QLCMCreateHTMLDataPropertiesForPreviewRequest( preview );
    if ( data && properties ) {
        QLPreviewRequestSetDataRepresentation( preview, data, kUTTypeHTML, properties );
    }

    if ( data       ) CFRelease( data );
    if ( properties ) CFRelease( properties );
    
    return kQLReturnNoError;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
    // Implement only if supported
}
