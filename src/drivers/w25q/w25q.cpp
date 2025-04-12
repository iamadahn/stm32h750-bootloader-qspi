#include "w25q.h"

using namespace W25Q;

Flash::Flash(QSPI_HandleTypeDef *hqspi)
{
    qspi_dev            = hqspi;
    data_mode           = QSPI_DATA_1_LINE;
    instruction_mode    = QSPI_INSTRUCTION_1_LINE;
    id                  = 0;
}

void Flash::reset(void)
{
    QSPI_CommandTypeDef cmd = {0};

    cmd.Instruction     = 0x66;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.InstructionMode = instruction_mode;

    if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
        while (true);
    }
    
    while(!wait());
    
    cmd.Instruction = 0x99;
    if (HAL_QSPI_Command(qspi_dev, &cmd, 1000) != HAL_OK) {
        while (true);
    }
}

bool Flash::write_enable(void)
{
    QSPI_CommandTypeDef cmd = {0};
    QSPI_AutoPollingTypeDef cfg = {0};

    cmd.Instruction     = 0x06;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.InstructionMode = instruction_mode;

    if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
        return false;
    }

    cfg.Match           = 0x02;
    cfg.Mask            = 0x02;
    cfg.Interval        = 0x10;
    cfg.MatchMode       = QSPI_MATCH_MODE_AND;
    cfg.StatusBytesSize = 1;
    cfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    cmd.Instruction = 0x05;
    cmd.DataMode    = data_mode;
    cmd.NbData      = 1;

    if (HAL_QSPI_AutoPolling(qspi_dev, &cmd, &cfg, 100) != HAL_OK) {
        return false;
    }

    return true;
}

void Flash::disable_quad_mode(void)
{
    QSPI_CommandTypeDef cmd = {0};

    cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction     = 0xFF;

    HAL_QSPI_Command(qspi_dev, &cmd, 100);

    data_mode           = QSPI_DATA_1_LINE;
    instruction_mode    = QSPI_INSTRUCTION_1_LINE;
}

void Flash::enable_quad_mode(void)
{
    uint8_t tmp = 0;

    read_reg(&tmp, 2);
    if ((tmp & 0x02) == 0) {
        tmp |= 0x02;
        write_reg(&tmp, 2);
    }

    if (((tmp >> 1) & 0x01) != 1) {
        while (true);
    }

    //enter quad mode
    QSPI_CommandTypeDef cmd = {0};

    cmd.Instruction     = 0x38;
    cmd.InstructionMode = instruction_mode;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    while (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
        while (true);
    }

    data_mode           = QSPI_DATA_4_LINES;
    instruction_mode    = QSPI_INSTRUCTION_4_LINES;

    cmd.Instruction     = 0xC0;
    cmd.InstructionMode = instruction_mode;
    cmd.DataMode        = data_mode;
    cmd.NbData          = 1;

    tmp = 0x03 << 4;
    write_enable();
    if (HAL_QSPI_Command(qspi_dev, &cmd, 100) == HAL_OK) {
        HAL_QSPI_Transmit(qspi_dev, &tmp, 100);
    }
}

uint16_t Flash::read_jedecid(void)
{
    uint8_t tmp[2] = {0};
    uint16_t ret = 0;

    QSPI_CommandTypeDef cmd = {0};

    cmd.Instruction     = 0x90;
    cmd.InstructionMode = instruction_mode;
    cmd.DataMode        = data_mode;
    cmd.AddressMode     = QSPI_ADDRESS_4_LINES;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.NbData          = 2;

    if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_QSPI_Receive(qspi_dev, tmp, 100) != HAL_OK) {
        return HAL_ERROR;
    }

    ret |= tmp[0] << 8;
    ret |= tmp[1] << 0;

    return ret;
}

bool Flash::read_reg(uint8_t *data, uint16_t reg)
{
    QSPI_CommandTypeDef cmd = {0};

    switch (reg) {
        case 1:
            cmd.Instruction = 0x05;
            break;

        case 2:
            cmd.Instruction = 0x35;
            break;

        case 3:
            cmd.Instruction = 0x15;
            break;
    }

    cmd.InstructionMode = instruction_mode;
    cmd.DataMode        = data_mode;
    cmd.NbData          = 1;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;

    if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
        return false;
    }

    if (HAL_QSPI_Receive(qspi_dev, data, 100) != HAL_OK) {
        return false;
    }

    return true;
}

bool Flash::write_reg(uint8_t *data, uint16_t reg)
{
    QSPI_CommandTypeDef cmd = {0};

    switch (reg) {
        case 1:
            cmd.Instruction = 0x01;
            break;

        case 2:
            cmd.Instruction = 0x31;
            break;

        case 3:
            cmd.Instruction = 0x11;
            break;

        default:
            return false;
    }

    write_enable();

    cmd.InstructionMode = instruction_mode;
    cmd.DataMode        = data_mode;
    cmd.NbData          = 1;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;

    if (HAL_QSPI_Command(qspi_dev, &cmd, 10) != HAL_OK) {
        return false;
    }

    if (HAL_QSPI_Transmit(qspi_dev, data, 1000) != HAL_OK) {
        return false;
    }

    return true;
}

void Flash::init(void)
{
    disable_quad_mode();
    reset();
    enable_quad_mode();
    id = read_jedecid();
}

bool Flash::wait(void)
{
    QSPI_CommandTypeDef cmd = {0};
    QSPI_AutoPollingTypeDef cfg = {0};

    cmd.InstructionMode = instruction_mode;
    cmd.DataMode        = data_mode;
    cmd.Instruction     = 0x05;                 //read register 1
    cmd.NbData          = 1;                    //read one byte
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;

    //mask setting
    cfg.Mask            = 0x01;                         //detect the busy bit
    cfg.Match           = 0x00;                         //device is idle if bit busy is set as 0
    cfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;   //stop sending cmd if match
    cfg.Interval        = 0x10;                         //time between two send
    cfg.MatchMode       = QSPI_MATCH_MODE_AND;          //don't care when detect only one bit
    cfg.StatusBytesSize = 1;
    if (HAL_QSPI_AutoPolling(qspi_dev, &cmd, &cfg, 1000) != HAL_OK) {
        return false;
    }

    return true;
}

bool Flash::read_data(uint8_t *data, uint32_t addr, uint32_t len)
{
    if (addr > 0xFFFFFF) {
        return false;
    }

    QSPI_CommandTypeDef cmd = {0};

    cmd.Instruction     = 0x0B;
    cmd.InstructionMode = instruction_mode;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.Address         = addr;
    cmd.AddressMode     = QSPI_ADDRESS_4_LINES;
    cmd.DataMode        = data_mode;
    cmd.NbData          = len;
    cmd.DummyCycles     = 8;

    if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
        return false;
    }

    if (HAL_QSPI_Receive(qspi_dev, data, 100) != HAL_OK) {
        return false;
    }

    return true;
}

bool Flash::write_data(uint8_t *data, uint32_t addr, uint32_t len)
{
    if (addr > 0xFFFFFF) { //detect if address bigger than max address value
        return false;
    }

    QSPI_CommandTypeDef cmd = {0};
    uint32_t end_addr, current_addr = 0, current_size;
    uint8_t *current_buffer = data;

    while (current_addr <= addr) { //current address increats until bigger than the passed address
        current_addr += 0x0100; //increment is a page 256 bytes
    }

    current_size = current_addr - addr; //now current_size is the blank space of last page to be programed
    if (current_size >= len) {
        current_size = len;
    }

    current_addr    = addr;
    end_addr        = addr + len;

    /* cmd config */
    if (instruction_mode == QSPI_INSTRUCTION_4_LINES) {
        cmd.Instruction = 0x02;
        cmd.AddressMode = QSPI_ADDRESS_4_LINES;
    } else {
        cmd.Instruction = 0x32;
        cmd.AddressMode = QSPI_ADDRESS_1_LINE;
    }

    cmd.InstructionMode = instruction_mode;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.Address         = current_addr;
    cmd.DataMode        = data_mode;
    cmd.NbData          = current_size;

    do {
        write_enable();
        enable_quad_mode();

        if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
            return false;
        }

        if (HAL_QSPI_Transmit(qspi_dev, current_buffer, 10000) != HAL_OK) {
            return false;
        }

        current_addr    += current_size;
        current_buffer  += current_size;
        current_size    = end_addr - current_addr;

        if (current_size > 0x100) {
            /*check if the rest bytes bigger than 0x100*/
            current_size = 0x100;
        }

        cmd.NbData  = current_size;
        cmd.Address = current_addr;
    } while (end_addr > cmd.Address);

    return true;
}

bool Flash::sector_erase(uint32_t start, uint32_t end)
{
    QSPI_CommandTypeDef cmd = {0};

    uint16_t sector_start = start / 4096; //start is the num of the first sector
    uint16_t sector_end = end / 4096; //end is the num of the last sector

    cmd.Instruction     = 0x20;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.InstructionMode = instruction_mode;

    if (instruction_mode == QSPI_INSTRUCTION_4_LINES) {
        cmd.AddressMode = QSPI_ADDRESS_4_LINES;
    } else {
        cmd.AddressMode = QSPI_ADDRESS_1_LINE;
    }

    do {
        write_enable();
        cmd.Address = sector_start * 4096; //sector increse
        if (HAL_QSPI_Command(qspi_dev, &cmd, 100) != HAL_OK) {
            return false;
        }
        sector_start++;
        wait();
    } while(sector_start <= sector_end);

    return true;
}

void Flash::enable_memory_map(void)
{
    QSPI_CommandTypeDef cmd = {0};
    QSPI_MemoryMappedTypeDef cfg = {0};

    cmd.InstructionMode     = instruction_mode;
    cmd.Instruction         = 0xEB; //quad fast read
    cmd.AddressMode         = QSPI_ADDRESS_4_LINES;
    cmd.AddressSize         = QSPI_ADDRESS_24_BITS;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = data_mode;
    cmd.DummyCycles         = 8;

    cfg.TimeOutActivation   = QSPI_TIMEOUT_COUNTER_DISABLE;
    cfg.TimeOutPeriod       = 0;

    if (HAL_QSPI_MemoryMapped(qspi_dev, &cmd, &cfg) != HAL_OK) {
        while (true);
    }
}
