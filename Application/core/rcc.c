/*
 * Copyright (C) 2024 zhmaksim <zhiharev.maxim.alexandrovich@yandex.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/* Includes ---------------------------------------------------------------- */

#include "rcc.h"
#include "systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

#define RCC_HSERDY_TIMEOUT      100
#define RCC_LSERDY_TIMEOUT      5000

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void rcc_setup_hse(void);

static void rcc_setup_pll(void);

static void rcc_setup_bus(void);

static void rcc_setup_clksource_cpu(void);

static uint32_t rcc_tick(void);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать RCC
 */
void rcc_init(void)
{
    rcc_setup_hse();
    rcc_setup_pll();
    rcc_setup_bus();
    rcc_setup_clksource_cpu();
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Настроить HSE
 */
static void rcc_setup_hse(void)
{
    uint32_t tickstart;

    /* Включить HSE */
    SET_BIT(RCC->CR, 0b1 << RCC_CR_HSEON_Pos);

    tickstart = rcc_tick();
    while (!READ_BIT(RCC->CR, RCC_CR_HSERDY_Msk)) {
        if (rcc_tick() - tickstart > RCC_HSERDY_TIMEOUT) {
            error();
        }
    }

    /* Включить CSS HSE */
    SET_BIT(RCC->CR, 0b1 << RCC_CR_CSSON_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Настроить PLL
 */
static void rcc_setup_pll(void)
{
    /* Выключить PLL перед настройкой */
    CLEAR_BIT(RCC->CR, 0b1 << RCC_CR_PLLON_Pos);

    /* Настроить PLL */
    WRITE_REG(RCC->PLLCFGR,
              0b1 << RCC_PLLCFGR_PLLSRC_Pos             /* Источник тактирования = HSE (25MHz) */
            | 0b11001 << RCC_PLLCFGR_PLLM_Pos           /* PLLM = /25 (HSE = 25MHz / 25 = 1MHz) */
            | 0b11000000 << RCC_PLLCFGR_PLLN_Pos        /* PLLN = x192 (1MHz * 192 = 192MHz) */
            | 0b0 << RCC_PLLCFGR_PLLP_Pos               /* PLLP = /2 (192MHz / 2 = 96MHz) */
            | 0b100 << RCC_PLLCFGR_PLLQ_Pos);           /* PLLQ = /4 (192MHz / 4 = 48MHz) */

    /* Включить PLL */
    SET_BIT(RCC->CR, 0b1 << RCC_CR_PLLON_Pos);
    while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY_Msk))
        ;

    /* Ожидание готовности PWR VOS после включения PLL */
    while (!READ_BIT(PWR->CSR, PWR_CSR_VOSRDY_Msk))
        ;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Настроить CPU BUS
 */
static void rcc_setup_bus(void)
{
    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_HPRE_Msk
             | RCC_CFGR_PPRE1_Msk
             | RCC_CFGR_PPRE2_Msk,
               0b0 << RCC_CFGR_HPRE_Pos         /* AHB = /1 (96MHz / 1 = 96MHz) */
             | 0b100 << RCC_CFGR_PPRE1_Pos      /* APB1 = /2 (96MHz / 2 = 48MHz) */
             | 0b0 << RCC_CFGR_PPRE2_Pos);      /* APB2 = /1 (96MHz / 1 = 96MHz) */
}
/* ------------------------------------------------------------------------- */

/**
 * @brief       Настроить источник тактирования CPU
 */
static void rcc_setup_clksource_cpu(void)
{
    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_SW_Msk,
               0b10 << RCC_CFGR_SW_Pos);            /* Источник тактирования = PLLP */

    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS_Msk) != 0b10 << RCC_CFGR_SWS_Pos)
        ;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Значение системного таймера
 *
 * @return          Значение таймера
 */
static inline uint32_t rcc_tick(void)
{
    return systick_get_tick();
}
/* ------------------------------------------------------------------------- */
