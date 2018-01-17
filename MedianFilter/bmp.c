//
//  bmp.c
//  FFMPEG-Develop
//
//  Created by wonderidea on 2/6/17.
//  Copyright (c) 2017 wonderidea. All rights reserved.
//
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
//#include <stdbool.h>
#include <errno.h>

#include "bmp.h"
#ifdef __cplusplus
};
#endif

unsigned char *bitmap_from_file(const char *file,FWBITMAPFILEHEADER *header,FWBITMAPINFOHEADER *bitmap_info)
{
    FILE *rfp = fopen(file, "rb");
    if (!rfp) {
        return NULL;
    }
    
    fread(header, 14, 1, rfp);
    fread(bitmap_info, sizeof(FWBITMAPINFOHEADER), 1, rfp);
    long long bSize=header->bfSize1;
    bSize <<= 16;
    bSize += header->bfSize0;
    long long bitmap_size = bSize - header->bfOffBits0;
    if (bitmap_size<=0) {
        fclose(rfp);
        return NULL;
    }
    
    int WIDTH = bitmap_info->biWidth;
    int HEIGHT = bitmap_info->biHeight;
    int tb_flag=0;  //1=pdf coordination
    if (HEIGHT<0) {
        HEIGHT = 0-HEIGHT;
        tb_flag = 1;
    }
    
    int bits = bitmap_info->biBitCount;
    if (bits<24) {
        printf("暂时支持24位和32位bmp\n");
        return NULL;
    }
    
    if ((14+sizeof(FWBITMAPINFOHEADER))!=header->bfOffBits0)
    {
        fseek(rfp, header->bfOffBits0, SEEK_CUR);
    }
    
    unsigned char *bitmap = (unsigned char *)malloc(WIDTH*HEIGHT<<2);
    //to gray bitmap
    int line_size = WIDTH*bits>>3;
    unsigned char *row_buffer = (unsigned char *)malloc(line_size);
    long long offset = 0;
    unsigned char bytes[4]={0,0,0,0xFF};
    for (int y=0; y<HEIGHT; y++) {
        fread(row_buffer, 1, line_size, rfp);
        unsigned char *p_color = bitmap+offset;
        if (bits==24) {
            int index=0;
            for (int i=0; i<line_size; i+=3) {
                memcpy(bytes, row_buffer+i, 3);
                //BGRA
                memcpy(p_color+index, bytes, 4);
                index += 4;
            }
        }
        else if(bits==32)
        {
            memcpy(p_color, row_buffer, line_size);
        }
        offset += (WIDTH<<2);
    }
    fclose(rfp);
    return bitmap;
}

int data_to_bmp_file(unsigned char *data,int width,int height,int bmp_bit_count,const char* filename)
{
    if (!data || !filename) {
        return 0;
    }
    unsigned long long imageBytesCount=(width*height*bmp_bit_count)>>3;
    
    FWBITMAPFILEHEADER bmpFileHeader;
    FWBITMAPINFOHEADER bitmapInfo;
    
    bmpFileHeader.bfType=0x4D42;
    bmpFileHeader.bfReserved1=0;
    bmpFileHeader.bfReserved2=0;
    bmpFileHeader.bfOffBits0=14+sizeof(FWBITMAPINFOHEADER);
    bmpFileHeader.bfOffBits1=0;
    
    unsigned long long bSize=bmpFileHeader.bfOffBits0 + imageBytesCount;
    bmpFileHeader.bfSize0=(bSize&0x0000FFFF);
    bmpFileHeader.bfSize1=(bSize&0xFFFF0000)>>16;
    
    //unsigned long long imageSize=(width*bmp_bit_count+31)/32*4*height;
    bitmapInfo.biSize=sizeof(FWBITMAPINFOHEADER);  //40bytes
    bitmapInfo.biWidth=width;
    bitmapInfo.biHeight=(height-1)^0xFFFFFFFF;
    bitmapInfo.biPlanes=1;
    bitmapInfo.biBitCount=bmp_bit_count;
    bitmapInfo.biCompression=0;
    bitmapInfo.biSizeImage=0;//imageSize&0xFFFFFFFF; //bytes at one row
    bitmapInfo.biXPelsPerMeter=100;//100;
    bitmapInfo.biYPelsPerMeter=100;//100;
    bitmapInfo.biClrUsed=0;
    bitmapInfo.biClrImportant=0;
    
    FILE *wtp=fopen(filename, "wb");
    if(!wtp)
    {
        return 0;
    }
    fwrite(&bmpFileHeader, 14, 1, wtp);
    fwrite(&bitmapInfo, sizeof(FWBITMAPINFOHEADER), 1, wtp);
    fwrite(data, 1, imageBytesCount, wtp);
    fclose(wtp);
    return 1;
}




