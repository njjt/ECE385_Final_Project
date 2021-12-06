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
#include <unistd.h>

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

						//printSignedHex0(kbdbuf.keycode[0]);
						//printSignedHex1(kbdbuf.keycode[1]);
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
	alt_u32 VRAM [240]; //Week 2 - extended VRAM
	alt_u32 pallette [16];
};


struct COLOR{
	char name [20];
	alt_u8 red;
	alt_u8 green;
	alt_u8 blue;
};



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


int occupied[200];

//this function determine whether a block is touching the edge
bool isUsed (int x, int y) {
	if((x < 0) || (x >= 10) || (y < 0) || (y >= 20)){
      return true;
	}
	else{
		return(occupied[(y*10) + x] != 0);
	}

}
int removeRow(int y){
	int rowCount = 0;
	int colCount = 0;
	for(int i=y;i<=y+3;i++){
		if(i<20){
			for(int j = 0;j<10;j++){
				if(occupied[i*10 + j] != 0){
					colCount ++;
				}
			}
			if(colCount == 10){
			printf("%x \n", y);
				for(int k = ((i)*10+9);k>0;k--){
					if(k < 200){
					if((k-(10)) >= 0){
						vga_ctrl->VRAM[k-10] = 0b00000000000000000000000000000000;
						vga_ctrl->VRAM[k] = occupied[k-10];
						occupied[k] = occupied[k-10];
						occupied[k-10] = 0;
					}
					else{
						vga_ctrl->VRAM[k] = 0b00000000000000000000000000000000;
						occupied[k] = 0;
					}
					}
				}
				rowCount++;
			}
			colCount = 0;
		}

	}
	return rowCount;
}
bool placeNewTet(int val,int color){
	int x = 3;
	int y = 0;
	bool finish = true;
	int blocks[4] = {0,0,0,0};
	int block_count = 0;
	for(int bit = 0x8000 ; bit > 0 ; bit = bit >> 1) {
		if((val & bit) != 0){
			if(isUsed(x,y)){
				finish = false;
				break;
			}
			else{
			blocks[block_count] = y*10+x;
			block_count++;
			finish = true;
			}
			}
		x++;
		if(x > 6){
		x = 3;
		y++;
		}
}
	if(finish){
		for(int i =0;i<4;i++){
			//printf("%x \n",blocks[i]);
			occupied[blocks[i]] = color;
			vga_ctrl->VRAM[blocks[i]] = color;
		}

	}
	return finish;
}

void initGame(){
	for(int i=0;i<200;i++){
			vga_ctrl->VRAM[i] = rand()%7 +1;
			usleep(5000);
			vga_ctrl->VRAM[i] = 0b00000000000000000000000000000000;
			occupied[i] = 0;
		}
	usleep(70000);
}
int pieceNums[28] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6};
int pieceCount = 0;
int genTet(){
	int res = -1;
	if(pieceCount == 28){
		pieceCount = 0;
		for(int i=0;i<28;i++){
			pieceNums[i] = i/4;
		}
	}
	int select = rand()%28;
	while(pieceNums[select] == -1){
		select = rand()%28;
	}
	res = pieceNums[select];
	pieceNums[select] = -1;
	return res;
}


void textVGADrawColorText(char* str, int offset)
{
	int i = 0;
	while (str[i]!=0)
	{
		vga_ctrl->VRAM[i+offset] = str[i];
		//vga_ctrl->VRAM[(y*COLUMNS + x + i) * 2 + 1] = str[i];

		i++;
	}
}



int main() {

	printf("initializing MAX3421E...\n");
	MAX3421E_init();
	printf("initializing USB...\n");
	USB_init();
	srand(time(0));
	printf("%x \n",time(0));
	bool move = true;
	BYTE* curr; //this can be wasd
	int x = 3;
	int y = 0;
	int currx = 0;
	int curry = 0;
	int curr_tet = 0;
	int curr_rot = 0;
	int blocks[4] = {0,0,0,0};
	int rot_blocks[4] = {0,0,0,0};
	int block_count = 0;
	int select = 0;
	int new_rot = 0;
	int rot_count =0;
	bool last = false;
	bool last_down[4] = {false,false,false,false};
	bool now_down[4] = {false,false,false,false};
	bool flag = true;
	BYTE fail = 0x0F;
	int lines = 0;
	int currLines = 0;
	int score = 0;
	int level = 1;
	int levelThreshold = 0;
	char score_string[80];
	char level_string[80];
	char line_string[80];
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


	//sprintf(score_string, "Score : %d", score);
	//textVGADrawColorText (score_string);

	for (int i = 0; i < 16; i++)
		{
			setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);
		}
	initGame();
	select = genTet();
	curr_tet = pieces[select][0];
	flag = placeNewTet(curr_tet,select+1);
	while (1) {
		sprintf(score_string, " Score : %d", score);
		textVGADrawColorText (score_string,200);
		sprintf(level_string, " Level : %d", level);
		textVGADrawColorText (level_string,212);
		sprintf(line_string, " Lines : %d", lines);
		textVGADrawColorText (line_string,224);
		printSignedHex0(level);
		printSignedHex1(score);
		curr = returnKeys();
		if(*curr == 0x29){
			while(*curr != 0x2C){
				curr = returnKeys();
			}
			usleep(250000);
		}
		move = true;
		if(!flag){
			usleep(500000);
			initGame();
			select = genTet();
			score = 0;
			level =1;
			levelThreshold = 0;
			curr_tet = pieces[select][0];
			flag = placeNewTet(curr_tet,select+1);
			curr_rot = 0;
			x= 3;
			y= 0;
		}
		//down
		for(int bit = 1 ; bit <= 0x8000 ; bit = bit << 1) {
				if((curr_tet & bit) != 0){
					if(!isUsed(-currx+x+3,-curry+1+y+3) || last_down[currx]){
						blocks[block_count] = (-curry+y+3)*10 + -currx + x+3;
						block_count++;
						now_down[currx] = true;
						}
					else{
						move = false;
						break;
					}
					}
				else{
					now_down[currx] = false;
				}
				currx++;
				if(currx > 3){
					currx = 0;
					curry++;
					for(int i=0;i<4;i++){
					last_down[i] = now_down[i];
					now_down[i] = false;
					}
				}
			}
			if(move){
				y++;
				for(int i =0;i<4;i++){
					vga_ctrl->VRAM[blocks[i]+10] = select+1;
					vga_ctrl->VRAM[blocks[i]] = 0b00000000000000000000000000000000;
					occupied[blocks[i]] = 0;
					occupied[blocks[i]+10] = select+1;
				}
			}
			else{
				currLines = removeRow(y);
				lines += currLines;

				switch(currLines){
				case 0:
					break;
				case 1:
					score += 1*level;
					break;
				case 2:
					score += 3*level;
					break;
				case 3:
					score += 5*level;
					break;
				case 4:
					score += 8*level;
					break;
				}
				if(lines >= level*5  + levelThreshold){
					levelThreshold += level*5;
					level++;
				}
				x= 3;
				y =0;
				select = genTet();
				curr_tet = pieces[select][0];
				flag = placeNewTet(curr_tet,select+1);
			}
			currx = 0;
			curry = 0;
			block_count = 0;
			move = true;
			for(int i=0;i<4;i++){
				last_down[i] = false;
			}
			usleep(17000-1000*level);


		switch(*curr){
		//rotate
		case 0x1A:
			new_rot = (curr_rot+1)%4;
			for(int bit = 0x8000; bit >0;bit = bit >> 1){

				if((pieces[select][new_rot] & bit) != 0){

					if(!isUsed(currx + x,curry+y) || (curr_tet & bit) != 0){
						blocks[block_count] = (curry+y)*10 + currx + x;
						block_count++;
					}
					else{
						move = false;
						break;
					}
				}
				else if((curr_tet & bit) != 0){
					rot_blocks[rot_count] = (curry+y)*10 + currx + x;
					rot_count++;
				}

				currx++;
				if(currx > 3){
					currx = 0;
					curry++;
				}
			}
			if(move){
				curr_tet = pieces[select][new_rot];
				curr_rot = (curr_rot +1)%4;
				for(int i =0;i<4;i++){
				vga_ctrl->VRAM[rot_blocks[i]] = 0b0000000000000000000000000000000;
				vga_ctrl->VRAM[blocks[i]] = select+1;
				occupied[rot_blocks[i]] = 0;
				occupied[blocks[i]] = select +1;

				}
			}

		currx = 0;
		curry = 0;
		move = true;
		block_count = 0;
		rot_count = 0;

		break;
		//left
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

					vga_ctrl->VRAM[blocks[i]-1] = select+1;
					vga_ctrl->VRAM[blocks[i]] = 0b00000000000000000000000000000000;
					occupied[blocks[i]] = 0;
					occupied[blocks[i]-1] = select+1;

			}
			}
			currx = 0;
			curry = 0;
			block_count = 0;
			move = true;
			break;

		//right
		case 0x07:
			for(int bit = 1 ; bit <= 0x8000 ; bit = bit << 1) {
					if((curr_tet & bit) != 0){
						if(!isUsed(-currx+1+x+3,-curry+y+3) || last){

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

						vga_ctrl->VRAM[blocks[i]+1] = select+1;
						vga_ctrl->VRAM[blocks[i]] = 0b00000000000000000000000000000000;
						occupied[blocks[i]] = 0;
						occupied[blocks[i]+1] = select+1;
				}
				}
				currx = 0;
				curry = 0;
				block_count = 0;
				move = true;
				break;
		case 0x16:
			for(int bit = 1 ; bit <= 0x8000 ; bit = bit << 1) {
				if((curr_tet & bit) != 0){
					if(!isUsed(-currx+x+3,-curry+1+y+3) || last_down[currx]){
						blocks[block_count] = (-curry+y+3)*10 + -currx + x+3;
						block_count++;
						now_down[currx] = true;
						}
					else{
						move = false;
						break;
					}
					}
				else{
					now_down[currx] = false;
				}
				currx++;
				if(currx > 3){
					currx = 0;
					curry++;
					for(int i=0;i<4;i++){
					last_down[i] = now_down[i];
					now_down[i] = false;
					}
				}
			}
			if(move){
				y++;
				for(int i =0;i<4;i++){
					vga_ctrl->VRAM[blocks[i]+10] = select+1;
					vga_ctrl->VRAM[blocks[i]] = 0b00000000000000000000000000000000;
					occupied[blocks[i]] = 0;
					occupied[blocks[i]+10] = select+1;
				}
			}
			currx = 0;
			curry = 0;
			block_count = 0;

			move = true;
			for(int i=0;i<4;i++){
				last_down[i] = false;
			}
			break;
	}

		usleep(17000-1000*level);
	}
	return 0;
}


