#include "stdio.h"
#include "windows.h" 

typedef struct {
  int width;
  int height;
  int refreshRate;
} Resolution;

typedef struct {
  int length;
  int capacity;
  Resolution *items;
} ResolutionArray;


int resize_resolution_array(ResolutionArray *item, int capacity){
  item->capacity = capacity;
  item->items = realloc(item->items, sizeof(ResolutionArray) * capacity);
}

Resolution* get_resolution_item(ResolutionArray *arr, int index){
  if(index < arr->length){
    return &arr->items[index];
  } else {
    return NULL;
  }
}

int add_resolution_item(ResolutionArray *arr, Resolution item){
  if(arr->capacity <= arr->length){
    resize_resolution_array(arr, arr->capacity*2);
  }
  arr->items[arr->length] = item;
  arr->length++;
  return 0;
}

int get_resolutions(ResolutionArray *resolutions){
  resolutions->length = 0;
  resolutions->items = malloc(sizeof(Resolution) * 20);
  resolutions->capacity = 20;
  DEVMODE edm;
  ZeroMemory(&edm, sizeof(edm));
  edm.dmSize = sizeof(edm);
  for(int si = 0; EnumDisplaySettings(NULL, si, &edm) != 0; si++){
    Resolution i;
    i.height = edm.dmPelsHeight;
    i.width = edm.dmPelsWidth;
    i.refreshRate = edm.dmDisplayFrequency;
    
    int result = add_resolution_item(resolutions, i);
    if(result != 0){
      printf("Failed to add resolution to array\n");
      return -1;
    }
  }
  return 0;
}

int set_resolution(Resolution item){
  DEVMODE dm;
  ZeroMemory(&dm, sizeof(dm));

  if(item.width > 0){
    dm.dmPelsWidth = item.width;
    dm.dmPelsHeight = item.height;

    dm.dmFields = dm.dmFields | DM_PELSWIDTH | DM_PELSHEIGHT;
  }
  if(item.refreshRate > 0){
    dm.dmDisplayFrequency = item.refreshRate;
    dm.dmFields = dm.dmFields | DM_DISPLAYFREQUENCY;
  }
  int result = ChangeDisplaySettings(&dm, 0);
  if(result == DISP_CHANGE_SUCCESSFUL){
    return 0;
  } else {
    return result;
  }
}