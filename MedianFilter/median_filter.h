//
//  median_filter.h
//  MedianFilter
//
//  Created by wonderidea on 1/16/18.
//  Copyright © 2018 wonderidea. All rights reserved.
//

#ifndef median_filter_h
#define median_filter_h

typedef struct tag_Filter_Box
{
    /**
     * 复制原图
     */
    unsigned char *src_bitmap;
    /**
     * 通过滤波之后的图像数据
     */
    unsigned char *dest_bitmap;
    /**
     * 坐标X
     */
    int x;
    /**
     * 坐标Y
     */
    int y;
    /**
     * 图像宽
     */
    int width;
    /**
     * 图像高
     */
    int height;
    /**
     * 图像色深（支持24，32）
     */
    int bit_count;
    /**
     * 窗体左边距
     */
    int box_sx;
    /**
     * 窗体上（下）边距
     */
    int box_sy;
    /**
     * 窗体宽
     */
    int box_w;
    /**
     * 窗体高
     */
    int box_h;
}Filter_Box;

/**
 * 初始化滤波器
 */
Filter_Box *filter_box_alloc(unsigned char *src_bitmap,int width,int height,int bitCount,int filter_box_w,int filter_box_h);

/**
 * 中值滤波
 */
void MedianFilter(Filter_Box *box);

/**
 * 释放滤波器
 */
void filter_box_free(Filter_Box *box);

#endif /* median_filter_h */
