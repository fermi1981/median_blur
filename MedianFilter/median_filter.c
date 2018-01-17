//
//  median_filter.c
//  MedianFilter
//
//  Created by wonderidea on 1/16/18.
//  Copyright © 2018 wonderidea. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "median_filter.h"


int GetMedianNum(int * bArray, int iFilterLen)
{
    int i,j;// 循环变量
    int bTemp;
    
    // 用冒泡法对数组进行排序
    int count = iFilterLen-1;
    for (j = 0; j < count; j++)
    {
        for (i = 0; i < (count - j); i++)
        {
            if (bArray[i] > bArray[i + 1])
            {
                // 互换
                bTemp = bArray[i];
                bArray[i] = bArray[i + 1];
                bArray[i + 1] = bTemp;
            }
        }
    }
    
    // 计算中值
    if (iFilterLen&1)
    {
        //odd 数组有奇数个元素，返回中间一个元素
        bTemp = bArray[iFilterLen>>1];
    }
    else
    {
        //even 数组有偶数个元素，返回中间两个元素平均值
        bTemp = (bArray[iFilterLen>>1] + bArray[(iFilterLen>>1) + 1])>>1;
    }
    
    return bTemp;
}

Filter_Box *filter_box_alloc(unsigned char *src_bitmap,int width,int height,int bitCount,int filter_box_w,int filter_box_h)
{
    Filter_Box *box = (Filter_Box *)malloc(sizeof(Filter_Box));
    if (!box) {
        return NULL;
    }
    memset(box,0,sizeof(Filter_Box));
    box->src_bitmap = src_bitmap;
    box->width = width;
    box->height = height;
    box->bit_count = bitCount;
    int line_size = width *(bitCount>>3);
    unsigned char *tmp_src_bitmap = (unsigned char *)malloc(line_size*height);
    if (!tmp_src_bitmap) {
        free(box);
        return NULL;
    }
    unsigned char *dest_bitmap = (unsigned char *)malloc(line_size*height);
    if (!dest_bitmap) {
        free(tmp_src_bitmap);
        free(box);
        return NULL;
    }
    box->src_bitmap = tmp_src_bitmap;
    box->dest_bitmap = dest_bitmap;
    
    long long image_size = line_size*box->height;
    memcpy(box->src_bitmap, src_bitmap, image_size);
    memcpy(box->dest_bitmap, box->src_bitmap, image_size);
    
    int sf_y = filter_box_h>>1;
    if (sf_y==0) {
        sf_y=1;
        filter_box_h = 3;
    }
    int sf_x = filter_box_w>>1;
    if (sf_x==0) {
        sf_x=1;
        filter_box_w = 3;
    }
    box->box_sx = sf_x;
    box->box_sy = sf_y;
    box->box_w = filter_box_w;
    box->box_h = filter_box_h;
    return box;
}

void fill_box(Filter_Box *box,int *box_array,int box_len)
{
    //X:j-sf_x -> j+sf_x
    //Y:i-sf_y -> i+sf_y
    int half_w = box->box_w>>1;
    int half_h = box->box_h>>1;
    int x=box->x;
    int y=box->y;
    int i,j;
    unsigned char *row;
    int color_bytes = (box->bit_count>>3);
    int line_size = box->width * color_bytes;
    int box_index=0;
    for (i=(y-half_h); i<=(y+half_h); i++)
    {
        row = (unsigned char *)(box->src_bitmap+i*line_size);
        for (j=(x-half_w); j<(x+half_w); j++)
        {
            int b = (int)(row[j*color_bytes+0]&0xFF);
            int g = (int)(row[j*color_bytes+1]&0xFF);
            int r = (int)(row[j*color_bytes+2]&0xFF);
            box_array[box_index]=b+(g<<8)+(r<<16) ;
            box_index++;
            if (box_index>=box_len) {
                return;
            }
        }
    }
}

//支持24位和32位
void MedianFilter(Filter_Box *box)
{
    unsigned char *lpDst;   // 指向要复制区域的指针
    int i,j;
    int line_size;      // 图像每行的字节数
    int color_bytes = box->bit_count>>3;
    line_size = box->width*color_bytes;
    
    int filter_box_w = box->box_w;
    int filter_box_h = box->box_h;
    
    int sf_y = box->box_sy;
    int sf_x = box->box_sx;
    
    int box_len = filter_box_w*filter_box_h;
    int *box_value = (int *)calloc(box_len,sizeof(int));
    
    int limit_h = (box->height-sf_y);
    int limit_w = (box->width-sf_x);
    int limit_size = limit_w;
    
    int offset = sf_y*line_size;;
    int tmp_value;
    
    
    for (i=sf_y; i<limit_h; i++)
    {
        //lpSrc = (unsigned char *)(box->src_bitmap+offset);
        lpDst = (unsigned char *)(box->dest_bitmap+offset);
        for (j=sf_x; j<limit_size; j++)
        {
            box->x = j;
            box->y = i;
            int col = j*color_bytes;
            fill_box(box, box_value, box_len);
            tmp_value = GetMedianNum(box_value, box_len);
            lpDst[col+0] = tmp_value&0xFF;
            lpDst[col+1] = (tmp_value>>8)&0xFF;
            lpDst[col+2] = (tmp_value>>16)&0xFF;
        }
        offset += line_size;
    }
    //TODO:处理边缘
    
    free(box_value);
}

void filter_box_free(Filter_Box *box)
{
    if (box->src_bitmap) {
        free(box->src_bitmap);
        box->src_bitmap = NULL;
    }
    if (box->dest_bitmap) {
        free(box->dest_bitmap);
        box->dest_bitmap = NULL;
    }
    free(box);
}




