#include "stdio.h"
#include "stdbool.h"
#include "Windows.h"

#include "src/resolution.c"

typedef struct {
  char *name;
  bool help;
  int refreshRate;
  int width;
  int height;
  bool valid;
  bool list;
} Arguments;

Arguments parse_args(int argc, char **argv) {
  Arguments result;
  result.valid = true;
  result.help = false;
  result.list = false;

  result.name = argv[0];
  if(argc > 1){
    for(int i = 1; i < argc; i++){
      char *arg = argv[i];
      #ifdef DEBUG
        printf("%d: %s\n", i, arg);
      #endif
      if(strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0){
        result.help = true;
        return result;
      }

      if(strcmp(arg, "-l") == 0 || strcmp(arg, "--list") == 0){
        result.list = true;
      }

      if(strcmp(arg, "-s") == 0 || strcmp(arg, "--resolution") == 0){
        if(i+1 < argc){
          // parse resolution
          int w, h;
          if(sscanf(argv[i+1], "%dx%d", &w, &h) == 2) {
            result.width = w;
            result.height = h;
          } else {
            printf("Error parsing the resolution. %s\nshould be in format WIDTHxHEIGHT\n", argv[i+1]);
            result.valid = false;
          }
        } else {
          printf("Error: the parameter --resolution/-s requires a value\n");
          result.valid = false;
        }
      }

      if(strcmp(arg, "-r") == 0 || strcmp(arg, "--refresh-rate") == 0){
        if(i+1 < argc){
          // parse 
          int val = atoi(argv[i+1]);
          #ifdef DEBUG
            printf("parsing refresh\nval: %d\n", val);
          #endif
          if(val == 0){
            printf("Failed to parse refresh-rate. %d\n", val);
            result.valid = false;
          } else {
            result.refreshRate = val;
          }
        } else {
          printf("Error: the parameter --refresh-rate/-r requires a value\n");
          result.valid = false;
        }
      }
    }
  } else {
    result.help = true;
  }
  return result;
}


const char *helpMsg = 
"Marvins resolution changer:\n"
"===========================\n"
"Usage:\n"
"--help:            Show help message\n"
"   -h\n"
"\n"
"--resolution:      Change the resolution.\n"
"    -s             format: WxH eg. 1920x1080\n"
"\n"
"--refresh-rate:    Change the refresh rate.\n"
"    -r             format: xx eg. 120\n"
"\n"
"--list:            List valid resolutions\n"
"    -l             \n"
"";

int main(int argc, char **argv){
  Arguments data = parse_args(argc, argv);
  if(!data.valid){
    printf("invalid parameter");
    return 1;
  }
  if(data.help){
    #ifdef DEBUG
    printf("help: %d\nvalid: %d\n", data.help, data.valid);
    #endif
    printf("%s", helpMsg);
    return 0;
  }

  #ifdef DEBUG
    printf("Resolution: %dx%d\nRefresh: %d\n", data.width, data.height, data.refreshRate);
  #endif

  DEVMODE dm;
  ZeroMemory(&dm, sizeof(dm));
  dm.dmSize = sizeof(dm);

  if(!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)){
    printf("Failed to read current settings");
    return 1;
  }

  printf("current resolution: %dx%d @ %dHz\n", dm.dmPelsWidth, dm.dmPelsHeight, dm.dmDisplayFrequency);

  if(data.list){
    ResolutionArray resis;
    int result = get_resolutions(&resis);
    if(result == 0){
      for(int i = 0; i < resis.length; i++){
        Resolution *item = get_resolution_item(&resis, i);
        printf("%d:\t%dx%d @ %dHz\n" , i+1, item->width, item->height, item->refreshRate);
      }
    }
    free(resis.items);
    return 0;
  }

  printf("Changing to: %dx%d @ %d\n", dm.dmPelsWidth, dm.dmPelsHeight, dm.dmDisplayFrequency);

  Resolution item = {
    .height = data.height,
    .width = data.width,
    .refreshRate = data.refreshRate
  };

  int result = set_resolution(item);
  if(result == 0){
    printf("display settings changed\n");
  } else {
    printf("Changing settings failed. Error code: %d\n", result);
  }

  return 0;
}