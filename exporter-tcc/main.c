//
//  main.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <stdio.h>
#include "user.h"
#include "createPDFFile.h"
#include "displayMyPDFPage.h"
#include "printPDFContent.h"

void performPDF(void) {
    CGRect pdfRect = CGRectMake(0, 0, 595, 842); // A4 size
    const char *filename = "/Users/daniilshmoylov/Downloads/TCC.pdf";
    User userInfo = create_user();
    createPDFFile(pdfRect, filename, userInfo);
    
    const size_t pageNumber = 1; // Номер страницы (начинается с 1)
    
    CGRect mediaBox = CGRectMake(0, 0, 595, 842);
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
    MyDisplayPDFPage(context, pageNumber, filename);
    
    
    printPDFContent(filename);
}

int main(int argc, const char * argv[]) {
    performPDF();
    return 0;
}
