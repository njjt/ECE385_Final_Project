//ECE 385 USB Host Shield code
//based on Circuits-at-home USB Host code 1.x
//to be used for ECE 385 course materials
//Revised October 2020 - Zuofu Cheng

#include <stdio.h>
#include "system.h"
#include "altera_avalon_spi.h"
#include "altera_avalon_spi_regs.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "usb_kb/GenericMacros.h"
#include "usb_kb/GenericTypeDefs.h"
#include "usb_kb/HID.h"
#include "usb_kb/MAX3421E.h"
#include "usb_kb/transfer.h"
#include "usb_kb/usb_ch9.h"
#include "usb_kb/USB.h"
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern HID_DEVICE hid_device;

static BYTE addr = 1; 				//hard-wired USB address
const char* const devclasses[] = { " Uninitialized", " HID Keyboard", " HID Mouse", " Mass storage" };
BOOT_KBD_REPORT kbdbuf;
BYTE GetDriverandReport() {
	BYTE i;
	BYTE rcode;
	BYTE device = 0xFF;
	BYTE tmpbyte;

	DEV_RECORD* tpl_ptr;
	printf("Reached USB_STATE_RUNNING (0x40)\n");
	for (i = 1; i < USB_NUMDEVICES; i++) {
		tpl_ptr = GetDevtable(i);
		if (tpl_ptr->epinfo != NULL) {
			printf("Device: %d", i);
			printf("%s \n", devclasses[tpl_ptr->devclass]);
			device = tpl_ptr->devclass;
		}
	}
	//Query rate and protocol
	rcode = XferGetIdle(addr, 0, hid_device.interface, 0, &tmpbyte);
	if (rcode) {   //error handling
		printf("GetIdle Error. Error code: ");
		printf("%x \n", rcode);
	} else {
		printf("Update rate: ");
		printf("%x \n", tmpbyte);
	}
	printf("Protocol: ");
	rcode = XferGetProto(addr, 0, hid_device.interface, &tmpbyte);
	if (rcode) {   //error handling
		printf("GetProto Error. Error code ");
		printf("%x \n", rcode);
	} else {
		printf("%d \n", tmpbyte);
	}
	return device;
}

void setLED(int LED) {
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_PIO_BASE,
			(IORD_ALTERA_AVALON_PIO_DATA(LEDS_PIO_BASE) | (0x001 << LED)));
}

void clearLED(int LED) {
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_PIO_BASE,
			(IORD_ALTERA_AVALON_PIO_DATA(LEDS_PIO_BASE) & ~(0x001 << LED)));

}

void printSignedHex0(signed char value) {
	BYTE tens = 0;
	BYTE ones = 0;
	WORD pio_val = IORD_ALTERA_AVALON_PIO_DATA(HEX_DIGITS_PIO_BASE);
	if (value < 0) {
		setLED(11);
		value = -value;
	} else {
		clearLED(11);
	}
	//handled hundreds
	if (value / 100)
		setLED(13);
	else
		clearLED(13);

	value = value % 100;
	tens = value / 10;
	ones = value % 10;

	pio_val &= 0x00FF;
	pio_val |= (tens << 12);
	pio_val |= (ones << 8);

	IOWR_ALTERA_AVALON_PIO_DATA(HEX_DIGITS_PIO_BASE, pio_val);
}

void printSignedHex1(signed char value) {
	BYTE tens = 0;
	BYTE ones = 0;
	DWORD pio_val = IORD_ALTERA_AVALON_PIO_DATA(HEX_DIGITS_PIO_BASE);
	if (value < 0) {
		setLED(10);
		value = -value;
	} else {
		clearLED(10);
	}
	//handled hundreds
	if (value / 100)
		setLED(12);
	else
		clearLED(12);

	value = value % 100;
	tens = value / 10;
	ones = value % 10;
	tens = value / 10;
	ones = value % 10;

	pio_val &= 0xFF00;
	pio_val |= (tens << 4);
	pio_val |= (ones << 0);

	IOWR_ALTERA_AVALON_PIO_DATA(HEX_DIGITS_PIO_BASE, pio_val);
}

void setKeycode(WORD keycode)
{
	//IOWR_ALTERA_AVALON_PIO_DATA(0x8002000, keycode);
	IOWR_ALTERA_AVALON_PIO_DATA(KEYCODE_BASE, keycode);
}

BYTE* returnKeys()
{
	    BYTE rcode;
		//BOOT_MOUSE_REPORT buf;		//USB mouse report
		//BOOT_KBD_REPORT kbdbuf;

		BYTE runningdebugflag = 0;//flag to dump out a bunch of information when we first get to USB_STATE_RUNNING
		BYTE errorflag = 0; //flag once we get an error device so we don't keep dumping out state info
		BYTE device = 1;
		//WORD keycode;

		//printf(".");
		MAX3421E_Task();
		USB_Task();
	    //usleep (500000);
		if (GetUsbTaskState() == USB_STATE_RUNNING) {

		    if (device == 1) {
				    //run keyboard debug polling
				    rcode = kbdPoll(&kbdbuf);

					if (rcode == hrNAK) {
					//continue; //NAK means no new data
						return 0;
					} else if (rcode) {
							printf("Rcode: ");
							printf("%x \n", rcode);
							return 0;
							//continue;
						}

						printSignedHex0(kbdbuf.keycode[0]);
						printSignedHex1(kbdbuf.keycode[1]);
						//printf("\n");
					}



				} else if (GetUsbTaskState() == USB_STATE_ERROR) {
					if (!errorflag) {
						errorflag = 1;
						clearLED(9);
						printf("USB Error State\n");
						//print out string descriptor here
					}
				} //else //not in USB running state
				//{

					//printf("USB task state: ");
					//printf("%x\n", GetUsbTaskState());
					//if (runningdebugflag) {	//previously running, reset USB hardware just to clear out any funky state, HS/FS etc
						//runningdebugflag = 0;
						//MAX3421E_init();
						//USB_init();
					//}
					//errorflag = 0;
					//clearLED(9);

		return kbdbuf.keycode;
}

struct TEXT_VGA_STRUCT {
	alt_u32 VRAM [200]; //Week 2 - extended VRAM
	//modify this by adding const bytes to skip to palette, or manually compute palette
	alt_u32 pallette [16];
};


struct COLOR{
	char name [20];
	alt_u8 red;
	alt_u8 green;
	alt_u8 blue;
};


//you may have to change this line depending on your platform designer
static volatile struct TEXT_VGA_STRUCT* vga_ctrl = VGA_TEXT_MODE_CONTROLLER_0_BASE;

//CGA colors with names
static struct COLOR colors[]={
    {"black",          0x0, 0x0, 0x0},
	{"blue",           0x0, 0x0, 0xa},
    {"green",          0x0, 0xa, 0x0},
	{"cyan",           0x0, 0xa, 0xa},
    {"red",            0xa, 0x0, 0x0},
	{"magenta",        0xa, 0x0, 0xa},
    {"brown",          0xa, 0x5, 0x0},
	{"light gray",     0xa, 0xa, 0xa},
    {"dark gray",      0x5, 0x5, 0x5},
	{"light blue",     0x5, 0x5, 0xf},
    {"light green",    0x5, 0xf, 0x5},
	{"light cyan",     0x5, 0xf, 0xf},
    {"light red",      0xf, 0x5, 0x5},
	{"light magenta",  0xf, 0x5, 0xf},
    {"yellow",         0xf, 0xf, 0x5},
	{"white",          0xf, 0xf, 0xf}
};
void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue)
{
	//fill in this function to set the color palette starting at offset 0x0000 2000 (from base)
	//vga_ctrl->VRAM[2048+(color>>1)];
	//vga_ctrl->pallette[color>>1] |= 0b11111111111111111111111111111111;
	vga_ctrl->pallette[color] &= 0b00000000000000000000000000000000;
	alt_u32 temp;
	temp = red<<8;
	temp|= green<<4;
	temp|= blue;
	vga_ctrl->pallette[color] |= temp;
	}



//this function determine whether a block is touching the edge
bool isUsed (int x, int y) {
	if((x < 0) || (x >= 10) || (y < 0) || (y >= 20)){
      return true;
	} else if (vga_ctrl->VRAM[y*10+x]>0){
      return true;
	}
	else{
		return false;
	}

}
void removeRow(int y, int num){
	for(int i = (y*10);i>0;i--){
		if(i-(10*num) > 0){
			vga_ctrl->VRAM[i] = vga_ctrl->VRAM[i-(10*num)];
			vga_ctrl->VRAM[i-10*num] = 0b00000000000000000000000000000000;
		}
		else{
			vga_ctrl->VRAM[i] = 0b00000000000000000000000000000000;
		}

	}
}
void placeNewTet(int val,int color){
	int x = 3;
	int y = 0;
	printf("%x", val);

	for(int bit = 0x8000 ; bit > 0 ; bit = bit >> 1) {
		printf("%x",x);
		if((val & bit) != 0){
			vga_ctrl->VRAM[y*10 +x] = color;

			}
		x++;
		if(x > 6){
		x = 3;
		y++;

		}
}
}



int main() {

	printf("initializing MAX3421E...\n");
	MAX3421E_init();
	printf("initializing USB...\n");
	USB_init();
	srand(time(0));
	bool move = true;
	BYTE* curr; //this can be wasd
	int x = 3;
	int y = 0;
	int currx = 0;
	int curry = 0;
	int width = 0;
	int height = 0;
	int curr_tet = 0;
	int curr_rot = 0;
	int blocks[4] = {0,0,0,0};
	int block_count = 0;
	int select = 0;
	bool last = false;

	int pieces[7][4] = {
			{
					0x0F00, 0x2222, 0x00F0, 0x4444  //i
			},
			{
					0x44C0, 0x8E00, 0x6440, 0x0E20  //j
			},
			{
					0x4460, 0x0E80, 0xC440, 0x2E00  //L
			},
			{
					0xCC00, 0xCC00, 0xCC00, 0xCC00  //o
			},
			{
					0x06C0, 0x8C40, 0x6C00, 0x4620  //s
			},
			{
					0x0E40, 0x4C40, 0x4E00, 0x4640  //t
			},
			{
					0x0C60, 0x4C80, 0xC600, 0x2640  //z
			}
	};




	for (int i = 0; i < 16; i++)
		{
			setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);
		}
	for(int i=0;i<200;i++){
		vga_ctrl->VRAM[i] = 0b00000000000000000000000000000000;
	}
	select = rand()%7;
	curr_tet = pieces[select][0];

	placeNewTet(curr_tet,select+1);
	while (1) {
		curr = returnKeys();
		move = true;
		switch(*curr){
		case 0x1A:

		//	for(int bit = 0x8000; bit >0;bit >> 1){
		//		if(curr_tet && bit){
		//		}
		//		}
		case 0x04:
			for(int bit = 0x8000 ; bit > 0 ; bit = bit >> 1) {
				if((curr_tet & bit) != 0){
					if(!isUsed(currx-1+x,curry+y) || last){
						blocks[block_count] = (curry+y)*10 + currx + x;
						block_count++;
						last = true;
					}
					else{
						move = false;
						break;
					}
				}
				else{
					last = false;
				}
				currx++;
				if(currx > 3){
				currx = 0;
				curry++;
				}
			}
			if(move){
				x -=1;
				for(int i =0;i<4;i++){
					printf("%x ",blocks[i]-1);
					printf("\n");
					vga_ctrl->VRAM[blocks[i]-1] = select+1;
					vga_ctrl->VRAM[blocks[i]] = 0b00000000000000000000000000000000;
			}
			}
			currx = 0;
			curry = 0;
			block_count = 0;
			move = true;
			break;


		case 0x07:
			for(int bit = 1 ; bit <= 0x8000 ; bit = bit << 1) {
					if((curr_tet & bit) != 0){
						if(!isUsed(-currx+1+x+3,-curry+y+3) || last){
							printf("anything");
							blocks[block_count] = (-curry+y+3)*10 + -currx + x+3;
							block_count++;
							last = true;
						}
						else{
							move = false;
							break;
						}
					}
					else{
						last = false;
					}
					currx++;
					if(currx > 3){
					currx = 0;
					curry++;
					last = false;
					}
				}
				if(move){
					x +=1;
					for(int i =0;i<4;i++){
						printf("%x ",blocks[i]+1);
						printf("\n");
						vga_ctrl->VRAM[blocks[i]+1] = select+1;
						vga_ctrl->VRAM[blocks[i]] = 0b00000000000000000000000000000000;
				}
				}
				currx = 0;
				curry = 0;
				block_count = 0;
				move = true;
				break;
		//case 0x16:







	}

	}
	return 0;
}


