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

#ifndef QLCMFunctions_h
#define QLCMFunctions_h

#include <CoreFoundation/CoreFoundation.h>
#include <QuickLook/QuickLook.h>

#define kQLCMStringEncoding             kCFStringEncodingUTF8
#define kQLCMTextEncodingName           CFSTR( "UTF-8" )

CFDataRef       QLCMCreateHTMLDataFromCommonMarkFile( CFURLRef file );
CFDictionaryRef QLCMCreateHTMLDataPropertiesFromGeneratorBundle( CFBundleRef bundle );

#define         QLCMCreateHTMLDataPropertiesForPreviewRequest(prev)     QLCMCreateHTMLDataPropertiesFromGeneratorBundle(QLPreviewRequestGetGeneratorBundle((prev)))
#define         QLCMCreateHTMLDataPropertiesForThumbnailRequest(thumb)  QLCMCreateHTMLDataPropertiesFromGeneratorBundle(QLThumbnailRequestGetGeneratorBundle((thumb)))

CFDataRef       QLCMCreateDataFromFile( CFURLRef file );
CFDataRef       QLCMCreateDataFromResource( CFBundleRef bundle, CFStringRef name );
CFStringRef     QLCMCreateStringFromFile( CFURLRef file );
CFStringRef     QLCMCreateStringFromData( CFDataRef data, CFStringEncoding * encoding );
CFStringRef     QLCMCreateHTMLStringFromCommonMarkString( CFStringRef str );

#endif
