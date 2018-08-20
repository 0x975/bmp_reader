#pragma once

void menu();
void eventMenu(form &img);

int event_button(form &img, int action);
int button_error(int button);
int event_error(int error);
int file_info(BITMAPHEADER header);
bool bufferSize(int size);