//
//  main.c
//  MedianFilter
//
//  Created by wonderidea on 1/16/18.
//  Copyright © 2018 wonderidea. All rights reserved.
//

#include <stdio.h>
#include "bmp.h"
#include "median_filter.h"
#include <stdlib.h>
#include <string.h>

const char *usage_string[]={
    "usage:",
    "exe {input bmpfile} {output bmp file} {options}",
    "options:",
    "-bw|--bw:set width of box",
    "-bh|--bh:set height of box",
    "example:",
    "exe {input bmpfile} {output bmp file} -bw 3 -bh 3",
    ""
};

int main(int argc, const char * argv[])
{
    int help = 1;
    if (argc>=3) {
        const char *input_file = argv[1];
        const char *output_file = argv[2];
        int box_w = 3;
        int box_h = 3;
        if (argc>3) {
            int index = 3;
            while (index<argc) {
                const char *str_value = argv[index];
                index++;
                if (strcmp(str_value, "-bw")==0||strcmp(str_value, "--bw")==0)
                {
                    if (index<argc) {
                        box_w = atoi(argv[index]);
                    }
                    index++;
                }
                else if (strcmp(str_value, "-bh")==0||strcmp(str_value, "--bh")==0)
                {
                    if (index<argc) {
                        box_h = atoi(argv[index]);
                    }
                    index++;
                }
            }
        }
        if (box_w<3) {
            box_w = 3;
        }
        if (box_h<3) {
            box_h = 3;
        }
#ifdef DEBUG
        //input_file = "/Users/wonderidea/Documents/test_gauss_blur.bmp";
        //output_file = "/Users/wonderidea/Documents/test_median.bmp";
        //box_w = 3;
        //box_h = 3;
#endif
        FWBITMAPFILEHEADER header;
        FWBITMAPINFOHEADER bitmap_info;
        unsigned char *color_bitmap = bitmap_from_file(input_file, &header, &bitmap_info);
        if (!color_bitmap) {
            return -1;
        }
        int width = bitmap_info.biWidth;
        int height = bitmap_info.biHeight;
        if (height<0) {
            height = 0- height;
        }
        Filter_Box *box = filter_box_alloc(color_bitmap, width, height, bitmap_info.biBitCount, 3, 3);
        if (box) {
            MedianFilter(box);
            data_to_bmp_file(box->dest_bitmap, width, height, bitmap_info.biBitCount, output_file);
            filter_box_free(box);
            help = 0;
        }
        free(color_bitmap);
    }
    if (help) {
        int count = sizeof(usage_string)/sizeof(usage_string[0]);
        for (int i=0; i<count; i++) {
            printf("%s\n",usage_string[i]);
        }
        
    }
    return 0;
}
