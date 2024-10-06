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

#include "flash.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать FLASH
 */
void flash_init(void)
{
    /* Настроить задержку чтения флэш-памяти = 3WS */
    MODIFY_REG(FLASH->ACR,
               FLASH_ACR_LATENCY_Msk,
               0b11 << FLASH_ACR_LATENCY_Pos);

    /* Включить предварительную выборку данных */
    SET_BIT(FLASH->ACR, 0b1 << FLASH_ACR_PRFTEN_Pos);

    /* Настроить кэш-инструкций */
    /* Выключить кэш-инструкций */
    CLEAR_BIT(FLASH->ACR, 0b1 << FLASH_ACR_ICEN_Pos);
    /* Сбросить кэш-инструкций */
    SET_BIT(FLASH->ACR, 0b1 << FLASH_ACR_ICRST_Pos);
    __NOP();
    CLEAR_BIT(FLASH->ACR, 0b1 << FLASH_ACR_ICRST_Pos);
    /* Включить кэш-инструкций */
    SET_BIT(FLASH->ACR, 0b1 << FLASH_ACR_ICEN_Pos);

    /* Настроить кэш-данных */
    /* Выключить кэш-данных */
    CLEAR_BIT(FLASH->ACR, 0b1 << FLASH_ACR_DCEN_Pos);
    /* Сбросить кэш-данных */
    SET_BIT(FLASH->ACR, 0b1 << FLASH_ACR_DCRST_Pos);
    __NOP();
    CLEAR_BIT(FLASH->ACR, 0b1 << FLASH_ACR_DCRST_Pos);
    /* Включить кэш-данных */
    SET_BIT(FLASH->ACR, 0b1 << FLASH_ACR_DCEN_Pos);
}
/* ------------------------------------------------------------------------- */
