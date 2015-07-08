/*******************************************************************************

  Robot Toolkit ++ (RTK++)

  Copyright (c) 2007-2014 Shuhui Bu <bushuhui@nwpu.edu.cn>
    http://www.adv-ci.com

  ----------------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

#ifndef __UTILS_UART_H__
#define __UTILS_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

///
/// \brief The UART class
///
class UART
{
public:
    UART();
    ~UART();

    int open(void);
    int close(void);

    int write(void *d, int len);
    int read(void *d,  int len);

public:
    int     port_no;            ///< port number - for windows
    char    port_name[256];     ///< port name   - for linux/Unix

    int     baud_rate;          ///< Baud rate 38400,9600 and so on
    int     byte_size;          ///< the bits in one byte 7,8..
    int     parity_sw;          ///< FIXME: windows not used yet
    int     stop_bits;          ///< stop bits 1,15,2

    int     DTR_sw;             ///< DTR switch
    int     RTS_sw;             ///< RTS switch
    int     timeout_sw;         ///< time out function

    void    *data;              ///< inner used data
};


#endif // __UTILS_UART_H__
