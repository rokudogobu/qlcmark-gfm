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

#include "QLCMPreferences.h"

static struct __pref_s {
    CFStringRef key;
    CFStringRef val;
} __prefs[] = {
    { kQLCMPreferencesKeyMode    , kQLCMPreferencesDefaultValueMode },
    { kQLCMPreferencesKeyFontSize, kQLCMPreferencesDefaultValueFontSize }
};

#define kQLCMPreferencesIndexMode     0
#define kQLCMPreferencesIndexFontSize 1

void QLCMPreferencesLoadForBundle( CFBundleRef bundle ) {
    CFStringRef appid = CFBundleGetIdentifier( bundle );
    if ( !appid ) return;
    
    for ( int i = 0; i < sizeof( __prefs ) / sizeof( struct __pref_s ); i++ ) {
        CFStringRef val = (CFStringRef)CFPreferencesCopyAppValue( __prefs[i].key, appid );
        if ( val ) __prefs[i].val = val;
    }
}

bool QLCMPreferencesIsModeDark( void ) {
    CFStringRef mode = __prefs[ kQLCMPreferencesIndexMode ].val;
    if ( mode ) {
        if ( CFStringCompare( mode, kQLCMPreferencesValueModeDark, kCFCompareCaseInsensitive ) == kCFCompareEqualTo ) return true;
        if ( CFStringCompare( mode, kQLCMPreferencesValueModeLight, kCFCompareCaseInsensitive ) == kCFCompareEqualTo ) return false;
    }
    
    CFStringRef interfaceStyle = CFPreferencesCopyAppValue( CFSTR( "AppleInterfaceStyle" ), kCFPreferencesCurrentApplication );
    if ( interfaceStyle && CFStringCompare( interfaceStyle, CFSTR( "Dark" ), kCFCompareCaseInsensitive ) == kCFCompareEqualTo ) {
        return true;
    } else {
        return false;
    }
}

CFStringRef QLCMPreferencesGetFontSize( void ) {
    return __prefs[ kQLCMPreferencesIndexFontSize ].val;
}
