/* RealBoy Emulator: Free, Fast, Yet Accurate, Game Boy/Game Boy Color Emulator.
 * Copyright (C) 2013 Sergio Andrés Gómez del Real
 *
 * This program is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by   
 * the Free Software Foundation; either version 2 of the License, or    
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 
 */

#include "gboy.h"
#include "gboy_mem.h"

void
mem_wr(Uint16 gb_addr, Uint8 val, Uint8 *host_addr)
{
	if (gb_addr >= 0xff00 && gb_addr < 0xff80)
		io_ctrl_wr(gb_addr&0xff, val);



	else if (gb_addr <= 0x7fff){
		// MBC2 read
		if ((gb_cart.cart_type == 0x6 || gb_cart.cart_type == 0x5 ) && gb_addr >= 0x4000)
			return;
		else
			(gb_mbc.mbc_funcs[(gb_addr>>12)])(val);

	}

	else if ((gb_cart.cart_type == 0x6 || gb_cart.cart_type == 0x5 ) && gb_addr >= 0xA000 && gb_addr <= 0xA1FF){
		*host_addr = val&0xf;
	}

	else
		*host_addr = val;
}

Uint8
mem_rd(Uint16 gb_addr, Uint8 *host_addr)
{
	if (gb_addr < 0x8000)
		return *host_addr;

	// not needed since we only read val&0xf
//	else if ((gb_cart.cart_type == 0x6 || gb_cart.cart_type == 0x5 ) && gb_addr >= 0xA000 && gb_addr <= 0xA1FF){
//		return (*host_addr)&0xf;
//	}

	else if (gb_addr > 0xe000 && gb_addr < 0xfe00)
		gb_addr &= ~0x2000;

//if (gb_addr < 0xc000) {
//	
//}

	if (gb_addr == 0xff00 && gboy_mode==SGB)
		return sgb_read();

	return *host_addr;
}
