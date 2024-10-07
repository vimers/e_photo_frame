#ifndef GUI_JPEGFILE_H
#define GUI_JPEGFILE_H

#include "screen/DEV_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

UBYTE GUI_ReadJpeg_RGB_7Color(const char *path, UWORD Xstart, UWORD Ystart);

#ifdef __cplusplus
}
#endif

#endif // GUI_JPEGFILE_H
