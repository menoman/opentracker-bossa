///////////////////////////////////////////////////////////////////////////////
// BOSSA
//
// Copyright (C) 2011-2012 ShumaTech http://www.shumatech.com/
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

#ifndef _NVMFLASH_H
#define _NVMFLASH_H

#include <stdint.h>
#include <exception>

#include "Flash.h"

class NvmFlashCmdError : public std::exception
{
public:
    NvmFlashCmdError(const char* message) : exception(), _message(message) {};
    const char* what() const throw() { return _message; }
protected:
    const char* _message;
};

class NvmFlash : public Flash
{
public:
    NvmFlash(Samba& samba,
             const std::string& name,
             uint32_t addr,
             uint32_t pages,
             uint32_t size,
             uint32_t planes,
             uint32_t lockRegions,
             uint32_t user,
             uint32_t stack,
             uint32_t regs,
             bool canBrownout);

    virtual ~NvmFlash();

    void loadBuffer(const uint8_t*, uint16_t size);

    void eraseAll();
    void eraseAuto(bool enable);

    bool isLocked();
    bool getLockRegion(uint32_t region);
    void setLockRegion(uint32_t region, bool enable);

    bool getSecurity();
    void setSecurity();

    bool getBod();
    void setBod(bool enable);
    bool canBod() { return _canBrownout; }

    bool getBor();
    void setBor(bool enable);
    bool canBor() { return _canBrownout; }

    bool getBootFlash();
    void setBootFlash(bool enable);
    bool canBootFlash() { return true; }

    void writePage(uint32_t page);
    void readPage(uint32_t page, uint8_t* data);

    // The application starts at page 128, i.e address 0x2000
    // Since bootloader is in page 0.
    uint16_t appStartPage() { return 128; }

private:
    uint32_t _regs;
    uint16_t _bufferSize;
    bool _canBrownout;
    bool _eraseAuto;
    const uint8_t* _buffer;

    const uint8_t* getCompletePage(const uint8_t*, uint16_t);
    void setup_page_write();
    uint32_t getAddressByRegion(uint32_t region);
    bool nvmIsReady();
    void executeNvmCommand(uint32_t cmd);

};

#endif // _NVMFLASH_H
