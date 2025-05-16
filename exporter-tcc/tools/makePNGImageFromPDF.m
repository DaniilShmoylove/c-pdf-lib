//
//  makePNGImageFromPDF.m
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <ImageIO/ImageIO.h>
#include "displayMyPDFPage.h"

void makePNGImageFromPDF(char *path) {
    const char *pdfPath = "/Users/daniilshmoylov/Desktop/ИСТОРИЯ\ ЗАЧЕТ.pdf"; // Укажите реальный путь
    const size_t pageNumber = 1; // Номер страницы (начинается с 1)
    
    // 1. Создаем контекст для растра (изображения)
    CGRect mediaBox = CGRectMake(0, 0, 595, 842); // Размер A4
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(
        NULL,
        mediaBox.size.width,
        mediaBox.size.height,
        8,
        0,
        colorSpace,
        kCGImageAlphaPremultipliedLast
    );
    
    // 2. Вызываем вашу функцию отрисовки
    MyDisplayPDFPage(context, pageNumber, pdfPath);
    
    // 3. Сохраняем результат в PNG-файл
    CGImageRef image = CGBitmapContextCreateImage(context);
    CFURLRef outputURL = CFURLCreateWithFileSystemPath(
        NULL,
        CFSTR("/Users/daniilshmoylov/Desktop/output.png"),
        kCFURLPOSIXPathStyle,
        false
    );
    
    CFStringRef fileType = CFSTR("public.png");
    
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(
        outputURL,
        fileType,
        1,
        NULL
    );
    
    CGImageDestinationAddImage(destination, image, NULL);
    CGImageDestinationFinalize(destination);
    
    // 4. Освобождаем ресурсы
    CFRelease(outputURL);
    CFRelease(destination);
    CGImageRelease(image);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    
    printf("PDF page saved to /tmp/output.png\n");
}
