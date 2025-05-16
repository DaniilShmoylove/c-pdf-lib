//
//  createPDFFile.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>

void myDrawContent(CGContextRef context) {
    CGContextSaveGState(context);
    
    // 1. Размеры страницы A4
    const CGFloat pageWidth = 595.0;
    const CGFloat pageHeight = 842.0;
    
    // 2. Двойное отражение: по X и Y
    CGContextTranslateCTM(context, pageWidth, pageHeight); // Смещаем начало координат
    CGContextScaleCTM(context, -1.0, -1.0);               // Отражаем обе оси
//    CGAffineTrans
    // 3. Настройка текста
    CGAffineTransform transform = CGAffineTransformMake(-1, 0, 0, -1, pageWidth, pageHeight);
    CGContextConcatCTM(context, transform);
    
    // 4. Создаём шрифт
    CTFontRef font = CTFontCreateWithName(CFSTR("Helvetica-Bold"), 36.0, NULL);
    
    // 5. Формируем атрибуты
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { font };
    CFDictionaryRef attributes = CFDictionaryCreate(
        NULL, keys, values, 1,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );
    
    // 6. Создаём строку
    CFAttributedStringRef attrString = CFAttributedStringCreate(
        NULL,
        CFSTR("TEXT"),
        attributes
    );
    
    // 7. Позиционирование с учётом двойного отражения
    CTLineRef line = CTLineCreateWithAttributedString(attrString);
    CGContextSetTextPosition(context,
        50,
        pageHeight - 100
    );
    
    // 8. Рисуем текст
    CTLineDraw(line, context);
    
    // 9. Освобождаем ресурсы
    CFRelease(line);
    CFRelease(attrString);
    CFRelease(attributes);
    CFRelease(font);
    
    CGContextRestoreGState(context);
}

void createPDFFile (CGRect pageRect, const char *filename)// 1
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
    myDrawContent (pdfContext);// 8
    CGPDFContextEndPage (pdfContext);// 9
    CGContextRelease (pdfContext);// 10
    CFRelease(pageDictionary); // 11
    CFRelease(boxData);
}
