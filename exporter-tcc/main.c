//
//  main.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <stdio.h>
#include "user.h"
#include "createPDFFile.h"
//int main(int argc, const char * argv[]) {
//    User user = create_user();
//    
//    printf("%s", user.username);
//    
//    free_user(&user);
//    
//    MyGetPDFDocumentRef("/Users/daniilshmoylov/Desktop/ИСТОРИЯ\ ЗАЧЕТ.pdf");
//    
//    return 0;
//}

//#include <UIKit/kUTTypePNG.h>

int main(int argc, const char * argv[]) {
    CGRect pdfRect = CGRectMake(0, 0, 595, 842); // A4 size
    const char *filename = "/Users/daniilshmoylov/Downloads/TCC.pdf";
//    createPDFFIle(CGRectMake(300, 500, 100, 100), "/Users/daniilshmoylov/Desktop/ИСТОРИЯ\ ЗАЧЕТ.pdf");
    createPDFFile(pdfRect, filename);
    return 0;
}
