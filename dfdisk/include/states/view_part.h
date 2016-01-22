/*+===================================================================
  File:

  Summary:

  Author:

  Rev History:

----------------------------------------------------------------------
  Copyright 2015-2016 Radial Technologies, Pty Ltd
===================================================================+*/
#ifndef VIEW_PART_H_INCLUDED
#define VIEW_PART_H_INCLUDED

#include <math.h>

/* Qurey the disk image about it's size */
static uint64_t query_disk_size(FILE* file)
{
    uint32_t ret;

    fseek(file, 0, SEEK_END);
    ret = ftell(file);

    return ret;
}

state_t view_part()
{
    clear();
    titlebar("dfdisk Fixed Disk Setup");

    /**
        Quick rundown on what we're going to do.
        1. Load the image located in the same directory as the executable
        2. Read the first 512 bytes (sector 1) into the MBR struct
        3. Display partition info on screen
    **/

    //Print out the partition info here
    printf_cent("Partition Info\n", 4);
    printf("    Partition   Flags    Type    Vol Label    Size    System    Disk Use\n");

    FILE* img = fopen("dos.img", "rb"); //Open the
    MBR mbr;

    fread(&mbr, 512, 1, img); //Read sector 1
    rewind(img);

    uint64_t disk_size = query_disk_size(img);
    char* boot = "    ";

    uint8_t i;
    for(i = 0; i < 4; i++)
    {
        if(mbr.partitions[i].part_size > 0)
        {
            if(mbr_bootable(&mbr.partitions[i]))
                boot = "boot";

            uint32_t part_size = (mbr.partitions[i].part_size * BYTES_PER_SECTOR) / (1024 * 1024); //MiB size of partition
            uint64_t disk_usage = part_size / (disk_size / (1024 * 1024)) * 100;

            printf("        %d       %s   %s               %dMiB    %s       %d%%\n", i + 1, boot, mbr_gettype(&mbr.partitions[i]), part_size, mbr_get_fsys(&mbr.partitions[i]), disk_usage);
        }
    }

    char choice;


    while((choice = getch()))
    {
        if(choice == 27)
        {
            change_state(0);
            break;;
        }
    }

    return NULL;
}

#endif // VIEW_PART_H_INCLUDED
