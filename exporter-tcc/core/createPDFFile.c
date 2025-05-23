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

// Вспомогательная функция для создания CFString из строки
CFStringRef CreateCFStringFromCString(const char* cstr) {
    return CFStringCreateWithCString(NULL, cstr, kCFStringEncodingUTF8);
}

// Функция для отрисовки текстового элемента
CGFloat DrawTextField(CGContextRef context, CGFloat yPos, CFDictionaryRef attrs, CFStringRef label, CFStringRef value) {
    // Создаем строку вида "Label: Value"
    CFStringRef fullString = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@: %@"), label, value);
    
    // Создаем атрибутированную строку
    CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, fullString, attrs);
    
    // Создаем и рисуем линию
    CTLineRef line = CTLineCreateWithAttributedString(attrString);
    CGContextSetTextPosition(context, 50, yPos);
    CTLineDraw(line, context);
    
    // Рассчитываем высоту строки
    CGFloat ascent, descent, leading;
    CTLineGetTypographicBounds(line, &ascent, &descent, &leading);
    CGFloat lineHeight = ascent + descent + leading + 10; // Добавляем отступ 10pt
    
    // Освобождаем ресурсы
    CFRelease(line);
    CFRelease(attrString);
    CFRelease(fullString);
    
    return yPos - lineHeight; // Возвращаем новую позицию Y
}

// Основная функция отрисовки
void myDrawContent(CGContextRef context, User userInfo) {
    CGContextSaveGState(context);
    
    const CGFloat pageWidth = 595.0;
    const CGFloat pageHeight = 842.0;
    
    // Применяем трансформации
    CGContextTranslateCTM(context, pageWidth, pageHeight);
    CGContextScaleCTM(context, -1.0, -1.0);
    CGAffineTransform transform = CGAffineTransformMake(-1, 0, 0, -1, pageWidth, pageHeight);
    CGContextConcatCTM(context, transform);

    // Начальная позиция
    CGFloat currentY = pageHeight - 100;
    
    // Стиль для заголовка
    CTFontRef titleFont = CTFontCreateWithName(CFSTR("Helvetica-Bold"), 24.0, NULL);
    CFStringRef titleKeys[] = { kCTFontAttributeName };
    CFTypeRef titleValues[] = { titleFont };
    CFDictionaryRef titleAttrs = CFDictionaryCreate(NULL, titleKeys, titleValues, 1,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    // Стиль для основного текста
    CTFontRef bodyFont = CTFontCreateWithName(CFSTR("Helvetica"), 14.0, NULL);
    CFStringRef bodyKeys[] = { kCTFontAttributeName };
    CFTypeRef bodyValues[] = { bodyFont };
    CFDictionaryRef bodyAttrs = CFDictionaryCreate(NULL, bodyKeys, bodyValues, 1,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    // Заголовок
    currentY = DrawTextField(context, currentY, titleAttrs,
        CFSTR("User Profile"), CFSTR(""));

    // Основные поля
    char buffer[50];
    
    // ID
    snprintf(buffer, sizeof(buffer), "%d", userInfo.id);
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("ID"), CreateCFStringFromCString(buffer));
    
    // Username
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Username"), CreateCFStringFromCString(userInfo.username));
    
    // Email
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Email"), CreateCFStringFromCString(userInfo.email));
    
    // Age
    snprintf(buffer, sizeof(buffer), "%d years", userInfo.age);
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Age"), CreateCFStringFromCString(buffer));
    
    // Registration Date
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%d",
           userInfo.registration_date.day,
           userInfo.registration_date.month,
           userInfo.registration_date.year);
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Registration Date"), CreateCFStringFromCString(buffer));
    
    // Status
    const char* status = userInfo.is_active ? "Active" : "Inactive";
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Status"), CreateCFStringFromCString(status));
    
    // Rating
    snprintf(buffer, sizeof(buffer), "%.1f/5.0", userInfo.rating);
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Rating"), CreateCFStringFromCString(buffer));
    
    // Salary
    snprintf(buffer, sizeof(buffer), "$%.2f", userInfo.salary);
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Salary"), CreateCFStringFromCString(buffer));
    
    // Gender
    const char* genderStr = "Other";
    switch(userInfo.gender) {
        case MALE: genderStr = "Male"; break;
        case FEMALE: genderStr = "Female"; break;
        case OTHER: genderStr = "Not Found"; break;
    }
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Gender"), CreateCFStringFromCString(genderStr));
    
    // Address
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Address"), CFSTR(""));
    
    // Street
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("  Street"), CreateCFStringFromCString(userInfo.address.street));
    
    // City
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("  City"), CreateCFStringFromCString(userInfo.address.city));
    
    // Postal Code
    currentY = DrawTextField(context, currentY, bodyAttrs,
        CFSTR("  Postal Code"), CreateCFStringFromCString(userInfo.address.postal_code));

    CFRelease(titleAttrs);
    CFRelease(titleFont);
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
    free_user(&userInfo);
}
