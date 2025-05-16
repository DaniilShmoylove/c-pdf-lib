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
    // Устанавливаем чёрный цвет
    CGContextSetRGBFillColor(context, 0.0, 0.0, 0.0, 1.0);

    // Создаём шрифт
    CTFontRef font = CTFontCreateWithName(CFSTR("Helvetica-Bold"), 36.0, NULL);
    
    // Создаём атрибуты текста
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { font };
    CFDictionaryRef attributes = CFDictionaryCreate(
        NULL,
        (const void**)keys,
        (const void**)values,
        1,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );
    
    // Создаём строку
    CFStringRef text = CFSTR("Hello, PDF World!");
    CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, text, attributes);
    
    // Корректируем координаты
    CGFloat pageHeight = 842.0; // Высота A4
    CGFloat textY = 50.0; // 50pt от верхнего края
    
    // Настраиваем трансформацию контекста
    CGContextSetTextMatrix(context, CGAffineTransformIdentity);
    CGContextTranslateCTM(context, 0, pageHeight); // Перемещаем в верхний левый угол
    CGContextScaleCTM(context, 1.0, -1.0); // Отражаем ось Y
    
    // Рисуем текст
    CTLineRef line = CTLineCreateWithAttributedString(attrString);
    CGContextSetTextPosition(context, 50.0, textY); // 50pt слева, 50pt сверху
    CTLineDraw(line, context);
    
    // Освобождаем ресурсы
    CFRelease(line);
    CFRelease(attrString);
    CFRelease(attributes);
    CFRelease(font);
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
