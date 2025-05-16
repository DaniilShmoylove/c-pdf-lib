//
//  createPDFFile.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>
#include "user.h"

void myDrawContent(CGContextRef context, User userInfo) {
    CGContextSaveGState(context);
    
    const CGFloat pageWidth = 595.0;
    const CGFloat pageHeight = 842.0;
    
    // Ваши оригинальные трансформации
    CGContextTranslateCTM(context, pageWidth, pageHeight);
    CGContextScaleCTM(context, -1.0, -1.0);
    CGAffineTransform transform = CGAffineTransformMake(-1, 0, 0, -1, pageWidth, pageHeight);
    CGContextConcatCTM(context, transform);

    // 1. Заголовок (36pt)
    CTFontRef titleFont = CTFontCreateWithName(CFSTR("Helvetica-Bold"), 36.0, NULL);
    CFStringRef titleKeys[] = { kCTFontAttributeName };
    CFTypeRef titleValues[] = { titleFont };
    CFDictionaryRef titleAttrs = CFDictionaryCreate(
        NULL, titleKeys, titleValues, 1,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );
    
    char* username = userInfo.username;
    CFStringRef cfUsername = CFStringCreateWithCString(NULL, username, kCFStringEncodingUTF8);
    CFAttributedStringRef titleString = CFAttributedStringCreate(
        NULL, cfUsername, titleAttrs
    );
    
    CTLineRef titleLine = CTLineCreateWithAttributedString(titleString);
    CGContextSetTextPosition(context, 50, pageHeight - 100);
    CTLineDraw(titleLine, context);

    // 2. Основной текст (18pt)
    CTFontRef bodyFont = CTFontCreateWithName(CFSTR("Helvetica"), 18.0, NULL);
    CFStringRef bodyKeys[] = { kCTFontAttributeName };
    CFTypeRef bodyValues[] = { bodyFont };
    CFDictionaryRef bodyAttrs = CFDictionaryCreate(
        NULL, bodyKeys, bodyValues, 1,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );
    
    char* email = userInfo.email;
    CFStringRef cfemail = CFStringCreateWithCString(NULL, email, kCFStringEncodingUTF8);
    
    CFAttributedStringRef bodyString = CFAttributedStringCreate(
        NULL, cfemail, bodyAttrs
    );
    
    CTLineRef bodyLine = CTLineCreateWithAttributedString(bodyString);
    CGContextSetTextPosition(context, 50, pageHeight - 140); // Смещаем на 60pt ниже заголовка
    CTLineDraw(bodyLine, context);
    

    // Освобождаем все ресурсы
    CFRelease(titleLine);
    CFRelease(titleString);
    CFRelease(titleAttrs);
    CFRelease(titleFont);
    
    CFRelease(bodyLine);
    CFRelease(bodyString);
    CFRelease(bodyAttrs);
    CFRelease(bodyFont);
    
    CGContextRestoreGState(context);
}

void createPDFFile (CGRect pageRect, const char *filename, User userInfo)// 1
{
    CGContextRef pdfContext;
    CFStringRef path;
    CFURLRef url;
    CFDataRef boxData = NULL;
    CFMutableDictionaryRef myDictionary = NULL;
    CFMutableDictionaryRef pageDictionary = NULL;
 
    path = CFStringCreateWithCString (NULL, filename, // 2
                                kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath (NULL, path, // 3
                     kCFURLPOSIXPathStyle, 0);
    CFRelease (path);
    myDictionary = CFDictionaryCreateMutable(NULL, 0,
                        &kCFTypeDictionaryKeyCallBacks,
                        &kCFTypeDictionaryValueCallBacks); // 4
    CFDictionarySetValue(myDictionary, kCGPDFContextTitle, CFSTR("My PDF File"));
    CFDictionarySetValue(myDictionary, kCGPDFContextCreator, CFSTR("My Name"));
    pdfContext = CGPDFContextCreateWithURL (url, &pageRect, myDictionary); // 5
    CFRelease(myDictionary);
    CFRelease(url);
    pageDictionary = CFDictionaryCreateMutable(NULL, 0,
                        &kCFTypeDictionaryKeyCallBacks,
                        &kCFTypeDictionaryValueCallBacks); // 6
    boxData = CFDataCreate(NULL,(const UInt8 *)&pageRect, sizeof (CGRect));
    CFDictionarySetValue(pageDictionary, kCGPDFContextMediaBox, boxData);
    CGPDFContextBeginPage (pdfContext, pageDictionary); // 7
    myDrawContent (pdfContext, userInfo);// 8
    CGPDFContextEndPage (pdfContext);// 9
    CGContextRelease (pdfContext);// 10
    CFRelease(pageDictionary); // 11
    CFRelease(boxData);
}
