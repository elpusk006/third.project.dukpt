/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : spdif_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef SPDIF_IODEFINE_H
#define SPDIF_IODEFINE_H

struct st_spdif
{                                                          /* SPDIF            */
    uint32_t       TLCA;                                   /*  TLCA            */
    uint32_t       TRCA;                                   /*  TRCA            */
    uint32_t       TLCS;                                   /*  TLCS            */
    uint32_t       TRCS;                                   /*  TRCS            */
    uint32_t       TUI;                                    /*  TUI             */
    uint32_t       RLCA;                                   /*  RLCA            */
    uint32_t       RRCA;                                   /*  RRCA            */
    uint32_t       RLCS;                                   /*  RLCS            */
    uint32_t       RRCS;                                   /*  RRCS            */
    uint32_t       RUI;                                    /*  RUI             */
    uint32_t       CTRL;                                   /*  CTRL            */
    uint32_t       STAT;                                   /*  STAT            */
    uint32_t       TDAD;                                   /*  TDAD            */
    uint32_t       RDAD;                                   /*  RDAD            */
};


#define SPDIF   (*(volatile struct st_spdif   *)0xE8012000uL) /* SPDIF */


#define SPDIFTLCA SPDIF.TLCA
#define SPDIFTRCA SPDIF.TRCA
#define SPDIFTLCS SPDIF.TLCS
#define SPDIFTRCS SPDIF.TRCS
#define SPDIFTUI SPDIF.TUI
#define SPDIFRLCA SPDIF.RLCA
#define SPDIFRRCA SPDIF.RRCA
#define SPDIFRLCS SPDIF.RLCS
#define SPDIFRRCS SPDIF.RRCS
#define SPDIFRUI SPDIF.RUI
#define SPDIFCTRL SPDIF.CTRL
#define SPDIFSTAT SPDIF.STAT
#define SPDIFTDAD SPDIF.TDAD
#define SPDIFRDAD SPDIF.RDAD
#endif
