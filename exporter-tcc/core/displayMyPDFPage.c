//
//  displayMyPDFPage.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include "getDPFDocumentRef.h"

void MyDisplayPDFPage(CGContextRef myContext,
                    size_t pageNumber,
                    const char *filename)
{
    CGPDFDocumentRef document;
    CGPDFPageRef page;
 
    document = MyGetPDFDocumentRef (filename);
    page = CGPDFDocumentGetPage (document, pageNumber);
    CGContextDrawPDFPage (myContext, page);
    CGPDFDocumentRelease (document);
}
