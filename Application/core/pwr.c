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

#include "pwr.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать PWR
 */
void pwr_init(void)
{
    /* Включить тактирование PWR */
    SET_BIT(RCC->APB1ENR, 0b1 << RCC_APB1ENR_PWREN_Pos);

    /* Настроить VOS1 (100MHz) */
    MODIFY_REG(PWR->CR,
               PWR_CR_VOS_Msk,
               0b11 << PWR_CR_VOS_Pos);

    /* Отключить защиту от записи в домен резервного копирования */
    SET_BIT(PWR->CR, 0b1 << PWR_CR_DBP_Pos);

    /* Включить резервный регулятор */
    SET_BIT(PWR->CSR, 0b1 << PWR_CSR_BRE_Pos);
    while (!READ_BIT(PWR->CSR, PWR_CSR_BRR_Msk))
        ;
}
/* ------------------------------------------------------------------------- */
