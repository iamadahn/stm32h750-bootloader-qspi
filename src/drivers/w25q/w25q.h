#pragma once

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_qspi.h"

namespace W25Q
{
    class Flash
    {
        private:
        QSPI_HandleTypeDef *qspi_dev;
        uint32_t data_mode;
        uint32_t instruction_mode;
        uint16_t id;
        void reset(void);
        bool write_enable(void);
        void disable_quad_mode(void);
        void enable_quad_mode(void);
        uint16_t read_jedecid(void);
        bool read_reg(uint8_t *data, uint16_t reg);
        bool write_reg(uint8_t *data, uint16_t reg);
        bool wait(void);

        public:
        Flash(QSPI_HandleTypeDef *hqspi);
        void init(void);
        bool read_data(uint8_t *data, uint32_t addr, uint32_t len);
        bool write_data(uint8_t *data, uint32_t addr, uint32_t len);
        bool sector_erase(uint32_t start, uint32_t end);
        void enable_memory_map(void);
    };
}

