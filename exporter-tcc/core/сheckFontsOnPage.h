//
//  сheckFontsOnPage.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 17.05.2025.
//

#ifndef FONT_CHECKER_H
#define FONT_CHECKER_H

#include <CoreGraphics/CoreGraphics.h>

#ifdef __cplusplus
extern "C" {
#endif

void checkFontsOnPage(CGPDFPageRef page);

#ifdef __cplusplus
}
#endif

#endif // FONT_CHECKER_H
