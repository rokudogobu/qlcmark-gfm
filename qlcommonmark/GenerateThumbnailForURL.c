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

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize);
void CancelThumbnailGeneration(void *thisInterface, QLThumbnailRequestRef thumbnail);

/* -----------------------------------------------------------------------------
    Generate a thumbnail for file

   This function's job is to create thumbnail for designated file as fast as possible
   ----------------------------------------------------------------------------- */

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize)
{
    CFErrorRef err;
    CFURLRef file = CFURLCreateFilePathURL( kCFAllocatorDefault, url, &err );
    if ( !file ) return kQLReturnNoError;
    
    CFStringRef str = QLCMCreateStringFromFile( file );
    if ( str ) {
        CFDataRef data = CFStringCreateExternalRepresentation( kCFAllocatorDefault, str, kQLCMStringEncoding, 0 );
        if ( data ) {
            int enc = kQLCMStringEncoding;
            CFNumberRef encoding = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, &enc );
    
            CFMutableDictionaryRef prevProps = CFDictionaryCreateMutable( kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks );
            CFDictionarySetValue( prevProps, kQLPreviewPropertyStringEncodingKey, encoding );
        
            CFMutableDictionaryRef thumbProps = CFDictionaryCreateMutable( kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks );
            CFDictionarySetValue( thumbProps, kQLThumbnailPropertyExtensionKey, CFSTR( "MD" ) );
        
            QLThumbnailRequestSetThumbnailWithDataRepresentation( thumbnail, data, kUTTypePlainText, prevProps, thumbProps );
            
            CFRelease( thumbProps );
            CFRelease( prevProps );
            CFRelease( encoding );
            CFRelease( data );
        }
        CFRelease( str );
    }
    CFRelease( file );
    
    return kQLReturnNoError;
}

void CancelThumbnailGeneration(void *thisInterface, QLThumbnailRequestRef thumbnail)
{
    // Implement only if supported
}
