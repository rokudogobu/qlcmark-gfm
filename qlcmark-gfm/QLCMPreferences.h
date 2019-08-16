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

#ifndef QLCMPreferences_h
#define QLCMPreferences_h

#include <CoreFoundation/CoreFoundation.h>

#define kQLCMPreferencesKeyMode                 CFSTR( "mode" )
#define kQLCMPreferencesKeyFontSize             CFSTR( "font-size" )

#define kQLCMPreferencesValueModeDark           CFSTR( "dark" )
#define kQLCMPreferencesValueModeLight          CFSTR( "light" )

#define kQLCMPreferencesDefaultValueMode        CFSTR( "auto" )
#define kQLCMPreferencesDefaultValueFontSize    CFSTR( "14px" )

void               QLCMPreferencesLoadForBundle( CFBundleRef bundle );
#define            QLCMPreferencesLoadForPreviewRequest( prev ) QLCMPreferencesLoadForBundle(QLPreviewRequestGetGeneratorBundle((prev)))

bool               QLCMPreferencesIsModeDark( void );

CFStringRef        QLCMPreferencesGetFontSize( void );

#endif /* QLCMPreferences_h */
