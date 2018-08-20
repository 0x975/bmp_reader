#include <iostream>
#include "CONTROL_LIST.h"
#include "IMG.h"
#include "menu.h"

void menu() {
	puts("------------------------------");
	puts("|  Open File          -   1  |");
	puts("|  Monochrome         -   2  |");
	puts("|  Gamma              -   3  |");
	puts("|  median Filter      -   4  |");
	puts("|  Negative           -   5  |");
	puts("|  image info         -   6  |");
	puts("|  Converting(16-24)  -   7  |");
	puts("|  Save as            -   8  |");
	puts("|  Clear the screen   -   9  |");
	puts("|  For Exit           -   0  |");
	puts("------------------------------");
}
void line() {
	puts("-----------------------------");
}

void eventMenu(form &img) {
	int Button = 0;
	int Event = 0;


	// goto - just for fun :)
	flag:;
	menu();
	while(1) {
		if( Event != EVENT_OK) event_error(Event);
		Button = 0; Event = 0;
		printf("\nPleas, select an action: ");
		scanf("%d", &Button);

		if( Button == 9 ) { 
			system("cls"); 
			goto flag; 
		}
		if( Button == 0 ) {
			freePixmap(img);
			break;
		}
		Event = event_button(img, Button);
	}
}

int openFILE(form &img, int &flag) {
	char file_name[30];
	int error;
	printf("Enter file name: ");
	scanf("%s", file_name); printf("\n");

	FILE *f = fopen(file_name, "rb");
	if( !f ) return ERROR_OPEN_FILE;

	if( flag = 1 ) 	freePixmap(img);
	if( ( error = readHEADER(img.header, f)) != READ_OK ) return error;
	if( (error = readBMP(img, f)) != READ_OK ) return error;

	printf("\nFILE '%s' is open \n", file_name);
	file_info(img.header);

	flag = 1;
	return EVENT_OK;
}
int writeFILE(form &img) {
	char file_name[30];
	printf("Enter file name: ");
	scanf("%s", file_name); printf("\n");

	FILE *f = fopen(file_name, "wb");
	if( !f ) return ERROR_OPEN_FILE;

	writeBMP(img, f);

	fclose(f);
	printf("Image save as '%s'\n", file_name);
	return EVENT_OK;
}

int event_button(form &img, int action) {
	static int flag = 0;
	int error;
	if( action == 1 ) {
		error = openFILE(img, flag);
		return error;
	}
	if( flag == 0 ) {
		return UNKNOWN_EVENT;
	}
	if( action == 2 ) {
		int k = 0;
		printf("Pleas, entry number for definition: ");
		scanf("%d", &k); printf("\n");
		monochrome(img, k);	

		printf("Image converted to monochrome\n");
		return EVENT_OK;
	}
	if( action == 3 ) {
		double k = 0;
		printf("Pleas, entry number for gamma-corection: ");
		scanf("%lf", &k); printf("\n");
		gamma(img, k);

		printf("Image is corected\n");
		return EVENT_OK;
	}
	if( action == 4 ) {
		int n = 0;
		printf("Pleas, entry size of buffer for filter (3-15) : ");
		scanf("%d", &n); printf("\n");
		if( (error = medianFilter(img, n)) != EVENT_OK ) return error;

		printf("Image is filtred\n");
		return EVENT_OK;
	}
	if( action == 5 ) {
		negative(img);
		printf("Image is negative\n");
		return EVENT_OK;
	}
	if( action == 6 ) {
		return file_info(img.header);
	}
	if( action == 7 ) {
		if( img.header.bitCount == 16 ) {
			error = convertTo24(img.img16, img.img24);
			freePixmap(img);
			img.header.bitCount = 24;
			if(!error) puts("converting complited");
			return error;
		}
	}
	if( action == 8 ) {
		return writeFILE(img);
	}
	return UNKNOWN_EVENT;
}

int event_error(int error) {
	if( error == ERROR_OPEN_FILE   ) {
		puts("ERROR_OPEN_FILE");
		return ERROR_OPEN_FILE;
	}
	if( error == ERROR_READ_HEADER ) {
		puts("ERROR_READ_HEADER");
		return ERROR_READ_HEADER;
	}
	if( error == ERROR_FILE_TYPE   ) {
		puts("ERROR_FILE_TYPE");
		return ERROR_FILE_TYPE;
	}
	if( error == ERROR_MEMORY	   ) {
		puts("ERROR_MEMORY");
		return ERROR_MEMORY;
	}
	if( error == ERROR_READ_PIXMAP ) {
		puts("ERROR_READ_PIXMAP");
		return ERROR_READ_PIXMAP;
	}
	if( error == UNKNOWN_EVENT ) {
		puts("UNKNOWN_EVENT");
		return UNKNOWN_EVENT;
	}
	if( error == ERROR_WRITE ) {
		puts("ERROR_WRITET");
		return ERROR_WRITE;
	}
	return EVENT_OK;
}

int file_info(BITMAPHEADER header) {
	puts("------------------------------");
	printf("bitCount: %d \n", header.bitCount);
	printf("width: %d \n", header.width);
	printf("height: %d \n", header.height);
	printf("Compression: %d \n", header.compression);
	printf("offsetBits: %d \n", header.offsetBits);
	puts("------------------------------");
	return EVENT_OK;
}