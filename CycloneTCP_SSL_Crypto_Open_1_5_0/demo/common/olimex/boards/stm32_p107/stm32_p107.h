/**
 * @file stm32_p107.h
 * @brief STM32-P107 demonstration board
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

#ifndef _STM32_P107_H
#define _STM32_P107_H

//Dependencies
#include "stm32f10x.h"

//LED1
#define LED1_PIN  GPIO_Pin_6
#define LED1_GPIO GPIOC

//LED2
#define LED2_PIN  GPIO_Pin_7
#define LED2_GPIO GPIOC

//WKUP button
#define WKUP_PIN  GPIO_Pin_0
#define WKUP_GPIO GPIOA

//TAMPER button
#define TAMPER_PIN  GPIO_Pin_13
#define TAMPER_GPIO GPIOC

#endif
