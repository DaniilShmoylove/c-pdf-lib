//
//  printPDFContent.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 17.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include "getPDFDocumentRef.h"
#include "displayMyPDFPage.h"

void printPDFContent(const char* filePath) {
    CGPDFDocumentRef document;
 
    document = MyGetPDFDocumentRef(filePath);
    CGPDFPageRef page = CGPDFDocumentGetPage(document, 1);
    CFStringRef extractedText = ExtractTextFromPDFPage(page);
    
    char buffer[4096];
    CFStringGetCString(extractedText, buffer, sizeof(buffer), kCFStringEncodingUTF8);
    printf("Extracted text:\n%s\n", buffer);
    
    CFRelease(extractedText);
    
    CGPDFDocumentRelease (document);
}
