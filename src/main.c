#include <time.h>
#include <getopt.h>
#include <string.h>
#include "screen/DEV_Config.h"
#include "screen/EPD_7in3f.h"
#include <unistd.h>
#include <errno.h>

typedef enum {
	DRAW_IMG,
	CLEAN_WHITE,
} OpMode;

void draw(const char* img_path) {
    if (access(img_path, F_OK) == -1) {
        fprintf(stderr, "Error: File '%s' does not exist or is not accessible.\n", img_path);
		return;
    }

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_7IN3F_WIDTH % 2 == 0)? (EPD_7IN3F_WIDTH / 2 ): (EPD_7IN3F_WIDTH / 2 + 1)) * EPD_7IN3F_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return ;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_7IN3F_WIDTH, EPD_7IN3F_HEIGHT, 0, EPD_7IN3F_WHITE);
    Paint_SetScale(7);

    Paint_SelectImage(BlackImage);
    
    if (GUI_ReadBmp_RGB_7Color(img_path, 0, 0) != 0) {
        fprintf(stderr, "Error: Failed to read or display the image '%s'.\n", img_path);
		goto cleanup;
    }
    
    EPD_7IN3F_Display(BlackImage);
cleanup:
	free(BlackImage);
}

int main(int argc, char *argv[])
{
    char *img_path = NULL;
    int opt;
	OpMode op_mode = CLEAN_WHITE;

    while ((opt = getopt(argc, argv, "i:c")) != -1) {
        switch (opt) {
            case 'i':
                img_path = optarg;
				op_mode = DRAW_IMG;
                break;
			case 'c':
				op_mode = CLEAN_WHITE;
				break;
            default:
                fprintf(stderr, "Usage: %s -i <img_path>\n", argv[0]);
                return 1;
        }
    }

    if (op_mode == DRAW_IMG && img_path == NULL) {
        fprintf(stderr, "Image path is required. Usage: %s -i <img_path>\n", argv[0]);
        return 1;
    }

    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_7IN3F_Init();

	if (op_mode == CLEAN_WHITE) {
    	EPD_7IN3F_Clear(EPD_7IN3F_WHITE); // WHITE
	} else if (op_mode == DRAW_IMG) {
		draw(img_path);
	} else {
		printf("Unsupported op_mode: %d\n", op_mode);
	}

	EPD_7IN3F_Sleep();
	DEV_Delay_ms(2000);
	DEV_Module_Exit();
    return 0;
}
