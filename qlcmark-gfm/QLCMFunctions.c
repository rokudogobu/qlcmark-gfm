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

#include "QLCMFunctions.h"

// #include <AppKit/AppKit.h>
// #include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include <QuickLook/QuickLook.h>

#include "QLCMPreferences.h"

#include "cmark-gfm.h"
#include "cmark-gfm-extension_api.h"
#include "cmark-gfm-core-extensions.h"

#define kQLCMResourceNameStyleCSS       CFSTR( "style.css" )
#define kQLCMResourceNameLayoutCSS      CFSTR( "layout.css" )
#define kQLCMResourceNameColorCSS       CFSTR( "color.css" )
#define kQLCMResourceNameTaskListJS     CFSTR( "tasklist.js" )
#define kQLCMResourceNameTaskListCSS    CFSTR( "tasklist.css" )
#define kQLCMMimeTypeCSS                CFSTR( "text/css" )
#define kQLCMMimeTypeJS                 CFSTR( "text/javascript" )
#define kQLCMMimeTypeHTML               CFSTR( "text/html" )
#define kQLCMEncodingNameUTF8           CFSTR( "UTF-8" )

#define BOM_MAXLEN 4

struct unicode_bom_s {
    CFStringEncoding enc;
    uint8_t len;
    uint8_t bytes[BOM_MAXLEN];
} __boms[] = {
    { kCFStringEncodingUTF8,    3, { 0xEF, 0xBB, 0xBF } },
    { kCFStringEncodingUTF32BE, 4, { 0x00, 0x00, 0xFE, 0xFF } },
    { kCFStringEncodingUTF32LE, 4, { 0xFF, 0xFE, 0x00, 0x00 } },
    { kCFStringEncodingUTF16BE, 2, { 0xFE, 0xFF } },
    { kCFStringEncodingUTF16LE, 2, { 0xFF, 0xFE } }
};

static CFStringEncoding __encs[] = {
    kCFStringEncodingUTF8,
    kCFStringEncodingShiftJIS,
    kCFStringEncodingEUC_JP,
    kCFStringEncodingISO_2022_JP
};

static char * __exts[] = {
    // "footnotes",
    "table",
    "strikethrough",
    "autolink",
    "tagfilter",
    "tasklist"
};

static int __opts = CMARK_OPT_DEFAULT
                    | CMARK_OPT_FOOTNOTES
//                    | CMARK_OPT_FULL_INFO_STRING
                    | CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES
//                    | CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE
//                    | CMARK_OPT_SOURCEPOS
//                    | CMARK_OPT_HARDBREAKS
//                    | CMARK_OPT_NOBREAKS
//                    | CMARK_OPT_SMART
//                    | CMARK_OPT_GITHUB_PRE_LANG
//                    | CMARK_OPT_UNSAFE
//                    | CMARK_OPT_VALIDATE_UTF8
//                    | CMARK_OPT_LIBERAL_HTML_TAG
;

#define kQLCMHTMLDataAttachmentsCount 5

// struct __attachment_s {
//     CFStringRef name;
//     CFStringRef type;
//     CFStringRef encoding;
// } __atchs[] = {
//     { kQLCMResourceNameLayoutCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 },
//     { kQLCMResourceNameStyleCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 },
//     { kQLCMResourceNameColorCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 },
//     { kQLCMResourceNameTaskListJS, kQLCMMimeTypeJS, kQLCMEncodingNameUTF8 }
// };
//
// #define kQLCMHTMLDataAttachmentsCount (sizeof(__atchs)/sizeof(__attachment_s))

struct __colorrgb_s {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static struct __colorrgb_s __rgb_white  = { 255,255,255 };
static struct __colorrgb_s __rgb_dark   = {  30, 30, 30 };
static struct __colorrgb_s __rgb_black  = {   0,  0,  0 };

#define BUFFER_SIZE 2048

CFDataRef QLCMCreateDataFromFile( CFURLRef file ) {
    CFMutableDataRef buffer = CFDataCreateMutable( kCFAllocatorDefault, 0 );
    if ( !buffer ) return NULL;
    
    CFReadStreamRef readStream = CFReadStreamCreateWithFile( kCFAllocatorDefault, file );
    if ( readStream ) {
        CFIndex ret = 0;
        if ( CFReadStreamOpen( readStream ) ) {
            UInt8 buf[BUFFER_SIZE];
            while ( ( ret = CFReadStreamRead( readStream, buf, BUFFER_SIZE ) ) > 0 ) {
                CFDataAppendBytes( buffer, buf, ret );
            }
            CFReadStreamClose( readStream );
        }
        CFRelease( readStream );
        
        if ( ret < 0 ) CFDataSetLength( buffer, 0 );
    }
    
    CFDataRef data = NULL;
    if ( CFDataGetLength( buffer ) ) {
        data = CFDataCreateCopy( kCFAllocatorDefault, buffer );
    }
    CFRelease( buffer );
    
    return data;
}

CFStringRef QLCMCreateStringFromData( CFDataRef data, CFStringEncoding * encoding ) {
    if ( !data ) return NULL;
    
    CFStringRef str = NULL;
    
    CFStringEncoding enc = kCFStringEncodingInvalidId;
    for ( int i = 0; i < sizeof(__boms) / sizeof( struct unicode_bom_s ); i++ ) {
        if ( CFDataGetLength( data ) < __boms[i].len ) continue;
        if ( memcmp( CFDataGetBytePtr( data ), __boms[i].bytes, __boms[i].len ) != 0 ) continue;
        
        str = CFStringCreateFromExternalRepresentation( kCFAllocatorDefault, data, __boms[i].enc );
        if ( str ) {
            enc = __boms[i].enc;
            break;
        }
    }
    
    if ( enc == kCFStringEncodingInvalidId ) {
        for (  int i = 0; i < sizeof(__encs) / sizeof( CFStringEncoding ); i++ ) {
            str = CFStringCreateFromExternalRepresentation( kCFAllocatorDefault, data, __encs[i] );
            if ( str ) {
                enc = __encs[i];
                break;
            }
        }
    }
    
    if ( encoding ) *encoding = enc;
    
    return str;
}

CFStringRef QLCMCreateStringFromFile( CFURLRef file ) {
    CFStringRef str = NULL;
    
    CFDataRef data = QLCMCreateDataFromFile( file );
    if ( data ) {
        str = QLCMCreateStringFromData( data, NULL );
        CFRelease( data );
    }
    
    return str;
}

CFDataRef QLCMCreateDataFromResource( CFBundleRef bundle, CFStringRef name ) {
    CFURLRef url  = CFBundleCopyResourceURL( bundle, name, NULL, NULL );
    CFDataRef data = NULL;
    if ( url ) {
        data = QLCMCreateDataFromFile( url );
        CFRelease( url );
    }
    return data;
}

CFDictionaryRef QLCMCreateAttachmentFromResource( CFBundleRef bundle, CFStringRef name, CFStringRef mimetype, CFStringRef encname ) {
    CFDictionaryRef attachment = NULL;
    
    CFDataRef data = QLCMCreateDataFromResource( bundle, name );
    if ( data ) {
        void * keys[] = {
            ( void * )kQLPreviewPropertyMIMETypeKey,
            ( void * )kQLPreviewPropertyAttachmentDataKey,
            ( void * )kQLPreviewPropertyTextEncodingNameKey
        };
        
        void * values[] = {
            ( void * ) mimetype,
            ( void * ) data,
            encname ? ( void * ) encname: NULL
        };
        
        attachment = CFDictionaryCreate( kCFAllocatorDefault,
                                        ( const void ** )keys,
                                        ( const void ** )values,
                                        encname ? 3: 2,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks );
        CFRelease( data );
    }
    
    return attachment;
}



CFDictionaryRef QLCMCreateHTMLDataPropertiesFromGeneratorBundle( CFBundleRef bundle ) {
    if ( !bundle ) return NULL;
    
    void * atchKeys[kQLCMHTMLDataAttachmentsCount] = {
        ( void * )kQLCMResourceNameLayoutCSS,
        ( void * )kQLCMResourceNameStyleCSS,
        ( void * )kQLCMResourceNameColorCSS,
        ( void * )kQLCMResourceNameTaskListJS,
        ( void * )kQLCMResourceNameTaskListCSS
    };
    
    void * atchValues[kQLCMHTMLDataAttachmentsCount] = {
        ( void * )QLCMCreateAttachmentFromResource( bundle, kQLCMResourceNameLayoutCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 ),
        ( void * )QLCMCreateAttachmentFromResource( bundle, kQLCMResourceNameStyleCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 ),
        ( void * )QLCMCreateAttachmentFromResource( bundle, kQLCMResourceNameColorCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 ),
        ( void * )QLCMCreateAttachmentFromResource( bundle, kQLCMResourceNameTaskListJS, kQLCMMimeTypeJS, kQLCMEncodingNameUTF8 ),
        ( void * )QLCMCreateAttachmentFromResource( bundle, kQLCMResourceNameTaskListCSS, kQLCMMimeTypeCSS, kQLCMEncodingNameUTF8 ),
    };
    
    CFDictionaryRef atchs =  CFDictionaryCreate( kCFAllocatorDefault,
                                                 ( const void ** )atchKeys,
                                                 ( const void ** )atchValues,
                                                 kQLCMHTMLDataAttachmentsCount,
                                                 &kCFTypeDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks );
    
    void * keys[] = {
        ( void * ) kQLPreviewPropertyMIMETypeKey,
        ( void * ) kQLPreviewPropertyTextEncodingNameKey,
        ( void * ) kQLPreviewPropertyAttachmentsKey
    };
    
    void * values[] = {
        ( void * ) kQLCMMimeTypeHTML,
        ( void * ) kQLCMTextEncodingName,
        ( void * ) atchs
    };
    
    CFDictionaryRef props =  CFDictionaryCreate( kCFAllocatorDefault,
                                                 ( const void ** )keys,
                                                 ( const void ** )values,
                                                 3,
                                                 &kCFTypeDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks );

    bool is_ok = true;
    for ( int i = 0; i < kQLCMHTMLDataAttachmentsCount; i++ ) {
        if ( atchValues[i] ) {
            // CFRelease( atchValues[i] );
        } else {
            is_ok = false;
        }
    }
    CFRelease( atchs );
    
    if ( !is_ok ) {
        CFRelease( props );
        props = NULL;
    }
    
    return props;
}

CFStringRef QLCMCreateHTMLContentStringFromCommonMarkString( CFStringRef str ) {
    if ( !str || CFStringGetLength( str ) == 0 ) return NULL;
    
    CFDataRef data = CFStringCreateExternalRepresentation( kCFAllocatorDefault, str, kCFStringEncodingUTF8, 0 );
    if ( !data ) return NULL;
    
    cmark_gfm_core_extensions_ensure_registered();
    
    cmark_parser *parser = cmark_parser_new( __opts );
    for ( int i = 0; i < sizeof( __exts ) / sizeof( char * ); i++ ) {
        cmark_syntax_extension *syntax_extension = cmark_find_syntax_extension( __exts[i] );
        if ( syntax_extension ) {
            cmark_parser_attach_syntax_extension( parser, syntax_extension );
        } else {
#ifdef DEBUG
            fprintf( stderr, "syntax extension '%s' not found.\n", __exts[i] );
#endif
        }
    }
    
    cmark_parser_feed( parser, ( const char *)CFDataGetBytePtr( data ) , CFDataGetLength( data ) );
    
    cmark_node * document = cmark_parser_finish( parser );
    char * content = cmark_render_html( document, __opts, cmark_parser_get_syntax_extensions( parser ) );
    
    cmark_parser_free( parser );
    cmark_node_free( document );
    CFRelease( data );
    
    CFStringRef contentStr = NULL;
    if ( content ) {
        contentStr = CFStringCreateWithCString( kCFAllocatorDefault, content, kCFStringEncodingUTF8 );
        free( content );
    }
    return contentStr;
}

CFStringRef QLCMCreateHTMLStringFromCommonMarkString( CFStringRef str ) {
    
    struct __colorrgb_s fontColor, bgColor;
    
    if ( QLCMPreferencesIsModeDark() ) {
        fontColor = __rgb_white;
        bgColor   = __rgb_dark;
    } else {
        fontColor = __rgb_black;
        bgColor   = __rgb_white;
    }
    
    CFStringRef content = QLCMCreateHTMLContentStringFromCommonMarkString( str );
    
    CFStringRef html = NULL;
    if ( content ) {
        
        html = CFStringCreateWithFormat( kCFAllocatorDefault, NULL,
                                        CFSTR(
                                            "<!DOCTYPE html>"
                                            "<html>"
                                              "<head>"
                                                "<meta charset=\"UTF-8\" />"
                                                "<style type=\"text/css\">"
                                                  ":root {"
                                                    "--font-size: %@;"
                                                    "--r-font: %.2f%%;"
                                                    "--g-font: %.2f%%;"
                                                    "--b-font: %.2f%%;"
                                                    "--r-bg: %.2f%%;"
                                                    "--g-bg: %.2f%%;"
                                                    "--b-bg: %.2f%%;"
                                                  "}"
                                                "</style>"
                                                "<link rel=\"stylesheet\" type=\"%@\" href=\"%@:%@\" />"
                                                "<link rel=\"stylesheet\" type=\"%@\" href=\"%@:%@\" />"
                                                "<link rel=\"stylesheet\" type=\"%@\" href=\"%@:%@\" />"
                                                "<script type=\"%@\" src=\"%@:%@\"></script>"
                                                "<link rel=\"stylesheet\" type=\"%@\" href=\"%@:%@\" />"
                                              "</head>"
                                              "<body>"
                                                "%@"
                                              "</body>"
                                            "</html>"
                                        ),
                                        QLCMPreferencesGetFontSize(),
                                        ((float)fontColor.r)/255*100, ((float)fontColor.g)/255*100, ((float)fontColor.b)/255*100,
                                        ((float)bgColor.r)/255*100, ((float)bgColor.g)/255*100, ((float)bgColor.b)/255*100,
                                        kQLCMMimeTypeCSS, kQLPreviewContentIDScheme, kQLCMResourceNameStyleCSS,
                                        kQLCMMimeTypeCSS, kQLPreviewContentIDScheme, kQLCMResourceNameLayoutCSS,
                                        kQLCMMimeTypeCSS, kQLPreviewContentIDScheme, kQLCMResourceNameColorCSS,
                                        kQLCMMimeTypeJS, kQLPreviewContentIDScheme, kQLCMResourceNameTaskListJS,
                                        kQLCMMimeTypeCSS, kQLPreviewContentIDScheme, kQLCMResourceNameTaskListCSS,
                                        content );
                                        
        CFRelease( content );
    }
    return html;
}

CFDataRef QLCMCreateHTMLDataFromCommonMarkFile( CFURLRef file ) {
    CFDataRef data = NULL;
    CFStringRef str = QLCMCreateStringFromFile( file );
    if ( str ) {
        CFStringRef html = QLCMCreateHTMLStringFromCommonMarkString( str );
        if ( html ) {
            data = CFStringCreateExternalRepresentation( kCFAllocatorDefault, html, kQLCMStringEncoding, 0 );
            CFRelease( html );
        }
        CFRelease( str );
    }
    return data;
}


