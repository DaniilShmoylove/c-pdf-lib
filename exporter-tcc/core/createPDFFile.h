//
//  createDPFFile.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#ifndef PDF_CREATOR_H
#define PDF_CREATOR_H

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

void myDrawContent(CGContextRef context);

void createPDFFile(CGRect pageRect, const char *filename);

#endif
