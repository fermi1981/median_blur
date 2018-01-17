//
//  bmp.h
//  FFMPEG-Develop
//
//  Created by wonderidea on 2/6/17.
//  Copyright (c) 2017 wonderidea. All rights reserved.
//
#ifndef BMP_H
#define BMP_H

/* 这里注意不要int与short混合写，不然会有对齐的问题（除非修改了对齐为字节） */
typedef struct tagFWBITMAPFILEHEADER
{
    union{
        unsigned char bytes[14];
        struct{
            unsigned short  bfType;
            unsigned short  bfSize0;
            unsigned short  bfSize1;
            unsigned short  bfReserved1;  //00 00
            unsigned short  bfReserved2;  //00 00
            unsigned short  bfOffBits0;
            unsigned short  bfOffBits1;
        };
    };
} FWBITMAPFILEHEADER;  //14

//for 32bit system = 0x28 bytes
typedef struct tagFWBITMAPINFOHEADER {
    union{
        unsigned char bytes[40];
        struct{
            unsigned int biSize:32;       //4 Btes =0x00000028   ->header size
            unsigned int  biWidth:32;      //4 Btes =0x00000010   ->16*16 icon
            unsigned int  biHeight:32;     //4 Btes =0x00000010*2 ->bitmap->Height*2
            unsigned short  biPlanes:16;     //2 Btes =0x0001   ->
            unsigned short  biBitCount:16;   //2 Btes =0x0018   ->24bit RGB mode
            unsigned int biCompression:32;    //4 Btes =0x00000000 compression method
            unsigned int biSizeImage:32;      //4 Btes =0x00000000 size of image
            unsigned int  biXPelsPerMeter:32;  //4 Btes =0x00000000 horizontal resolution
            unsigned int  biYPelsPerMeter:32;  //4 Btes =0x00000000 vertical resolution
            unsigned int biClrUsed:32;        //4 Btes =0x00000000 number of colors used
            unsigned int biClrImportant:32;   //4 Btes =0x00000000 number of important colors
        };
    };
} FWBITMAPINFOHEADER;

extern unsigned char *bitmap_from_file(const char *file,FWBITMAPFILEHEADER *header,FWBITMAPINFOHEADER *bitmap_info);

extern int data_to_bmp_file(unsigned char *data,int width,int height,int bmp_bit_count,const char* filename);

#endif

