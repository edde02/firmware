/**
 * @file       CircularBuffer.h
 * @author     Pere Tuset-Peiro (peretuset@openmote.com)
 * @version    v0.1
 * @date       May, 2015
 * @brief
 *
 * @copyright  Copyright 2015, OpenMote Technologies, S.L.
 *             This file is licensed under the GNU General Public License v2.
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include "Buffer.h"

class CircularBuffer : public Buffer
{
public:
    CircularBuffer(uint8_t* buffer, uint32_t length);
    void reset(void);
    uint32_t getSize(void);
    bool isEmpty(void);
    bool isFull(void);
    bool read(uint8_t* data);
    bool read(uint8_t* buffer, uint32_t length);
    bool write(uint8_t data);
    bool write(const uint8_t* data, uint32_t length);
private:
};

#endif /* CIRCULAR_BUFFER_H_ */
