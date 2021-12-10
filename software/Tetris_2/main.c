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

BYTE *returnKeys() {
  BYTE rcode;
  // BOOT_MOUSE_REPORT buf;		//USB mouse report
  // BOOT_KBD_REPORT kbdbuf;

  BYTE runningdebugflag = 0; // flag to dump out a bunch of information when we
                             // first get to USB_STATE_RUNNING
  BYTE errorflag = 0; // flag once we get an error device so we don't keep
                      // dumping out state info
  BYTE device = 1;
  // WORD keycode;

  // printf(".");
  MAX3421E_Task();
  USB_Task();
  // usleep (500000);
  if (GetUsbTaskState() == USB_STATE_RUNNING) {

    if (device == 1) {
      // run keyboard debug polling
      rcode = kbdPoll(&kbdbuf);

      if (rcode == hrNAK) {
        // continue; //NAK means no new data
        return 0;
      } else if (rcode) {
        printf("Rcode: ");
        printf("%x \n", rcode);
        return 0;
        // continue;
      }

      // printSignedHex0(kbdbuf.keycode[0]);
      // printSignedHex1(kbdbuf.keycode[1]);
      // printf("\n");
    }

  } else if (GetUsbTaskState() == USB_STATE_ERROR) {
    if (!errorflag) {
      errorflag = 1;
      clearLED(9);
      printf("USB Error State\n");
      // print out string descriptor here
    }
  }
  return kbdbuf.keycode;
}
//struct storing grids scores levels and lines for each player
struct TEXT_VGA_STRUCT {
	alt_u32 VRAM [472];
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
static struct COLOR colors[] = {
    {"black", 0x0, 0x0, 0x0},       {"blue", 0x0, 0x0, 0xa},
    {"green", 0x0, 0xa, 0x0},       {"cyan", 0x0, 0xa, 0xa},
    {"red", 0xa, 0x0, 0x0},         {"magenta", 0xa, 0x0, 0xa},
    {"brown", 0xa, 0x5, 0x0},       {"light gray", 0xa, 0xa, 0xa},
    {"dark gray", 0x5, 0x5, 0x5},   {"light blue", 0x5, 0x5, 0xf},
    {"light green", 0x5, 0xf, 0x5}, {"light cyan", 0x5, 0xf, 0xf},
    {"light red", 0xf, 0x5, 0x5},   {"light magenta", 0xf, 0x5, 0xf},
    {"yellow", 0xf, 0xf, 0x5},      {"white", 0xf, 0xf, 0xf}};

//writes color pallete with colors above
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

//reading from VRAM was difficult,so we double design in C
int occupied[400];

//this function determine whether a block is touching the edge
bool isUsed (int x, int y, int offset) {
	if((x < 0) || (x >= 10) || (y < 0) || (y >= 20)){
      return true;
	}
	else{
		return(occupied[(y*10) + x+offset] != 0);
	}
}
//removes 0 to 4 lines
int removeRow(int y, int offset) {
  int rowCount = 0;
  int colCount = 0;
  //between current y and 3 below y
  for (int i = y; i <= y + 3; i++) {
    if (i < 20) {
      for (int j = 0; j < 10; j++) {
        if (occupied[i * 10 + j + offset] != 0) {
          colCount++;
        }
      }
      //if row is full
      if (colCount == 10) {
        for (int k = ((i)*10 + 9); k > 0; k--) {
          if (k < 200) {
	    //moves row above to row below
            if ((k - (10)) >= 0) {
              vga_ctrl->VRAM[k - 10 + offset] = 0b00000000000000000000000000000000;
              vga_ctrl->VRAM[k + offset] = occupied[k - 10 + offset];
              occupied[k + offset] = occupied[k - 10 + offset];
              occupied[k - 10 + offset] = 0;
            } else {
              vga_ctrl->VRAM[k + offset] = 0b00000000000000000000000000000000;
              occupied[k + offset] = 0;
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
//places new tetrimino at top center only if not currently occupied
bool placeNewTet(int val, int color, int offset) {
  int x = 3; //top center
  int y = 0;
  bool finish = true;
  int blocks[4] = {0, 0, 0, 0}; //block indexes used to place new block
  int block_count = 0;
  //top left to bottom right
  for (int bit = 0x8000; bit > 0; bit = bit >> 1) {
    if ((val & bit) != 0) {
      
      if (isUsed(x, y, offset)) {
        finish = false;
        break;
      }
      else {
        blocks[block_count] = y * 10 + x;
        block_count++;
        finish = true;
      }
    }
    //loops x from 3-6 and resets to 3
    x++;
    if (x > 6) {
      x = 3;
      y++;
    }
  }
  //if all blocks are not occupied place each block in its correct spot
  if (finish) {
    for (int i = 0; i < 4; i++) {
      occupied[blocks[i] + offset] = color;
      vga_ctrl->VRAM[blocks[i] + offset] = color;
    }
  }
  return finish;
}
//starts game
void initGame() {
  //animation through both grids that clears screen
  for (int i = 0; i < 200; i++) {
    vga_ctrl->VRAM[i] = rand() % 7 + 1;
    vga_ctrl->VRAM[i + 200] = rand() % 7 + 1;
    usleep(5000);
    vga_ctrl->VRAM[i] = 0b00000000000000000000000000000000;
    occupied[i] = 0;
    vga_ctrl->VRAM[i + 200] = 0b00000000000000000000000000000000;
    occupied[i + 200] = 0;
  }

  usleep(70000);
}

int pieceNums[28] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3,
                     3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
int pieceCount = 0;

//picks new tetrimino index
int genTet(){
       
       int res = -1;
       ///if all blocks are used reset array
       if (pieceCount >= 28) {
         pieceCount = 0;
         for (int i = 0; i < 28; i++) {
           pieceNums[i] = i / 4;
         }
       }
       int select = rand() % 28;
       //picks new index until not occupied
       while (pieceNums[select] == -1) {
         select = rand() % 28;
       }
       pieceCount++;
       res = pieceNums[select];
       pieceNums[select] = -1;
       return res;
}
//assigns string to VRAM, fills end with spaces if not complete
void textVGADrawColorText(char* str, int offset)
{
	int i = 0;
	char* sp = " ";
	while (str[i]!=0)
	{
		vga_ctrl->VRAM[i+offset] = str[i];
		i++;
	}
	if(i < 11){
		for(int j=i;j<12;j++){
			vga_ctrl->VRAM[j+offset] = *sp;
		}
	}
}

//tetriminos stored as 4x4 binary arrays in hexadecimal each digit is a column. 
int pieces[7][4] = {{
                        0x0F00, 0x2222, 0x00F0, 0x4444 // i
                    },
                    {
                        0x44C0, 0x8E00, 0x6440, 0x0E20 // j
                    },
                    {
                        0x4460, 0x0E80, 0xC440, 0x2E00 // L
                    },
                    {
                        0xCC00, 0xCC00, 0xCC00, 0xCC00 // o
                    },
                    {
                        0x06C0, 0x8C40, 0x6C00, 0x4620 // s
                    },
                    {
                        0x0E40, 0x4C40, 0x4E00, 0x4640 // t
                    },
                    {
                        0x0C60, 0x4C80, 0xC600, 0x2640 // z
                    }};

//pushes tetriminos down, passed by reference to manage each player, used for both player input and automatic
void Down(int* x,int* y,int* curr_tet,int* lines,int* score,int* level,int* levelThreshold, int* select,bool* flag, int offset){
  int currx = 0;
  int curry = 0;
  bool last_down[4] = {false, false, false, false}; //stores last rows positions
  bool now_down[4] = {false, false, false, false}; //sores current row positions
  int blocks[4] = {0, 0, 0, 0}; //stores new blocks
  int block_count = 0; 
  bool move = true;
  int currLines = 0; //lines removed
  //bottom to top
  for (int bit = 1; bit <= 0x8000; bit = bit << 1) {
    if ((*curr_tet & bit) != 0) {
      //read right to left instead of left to right to match bottom up design
      if (!isUsed(-currx + *x + 3, -curry + 1 + *y + 3, offset) ||
          last_down[currx]) {
        blocks[block_count] = (-curry + *y + 3) * 10 + -currx + *x + 3;
        block_count++;
        now_down[currx] = true;
      } else {
        move = false;
        break;
      }
    } else {
      now_down[currx] = false;
    }
    //loops through each block
    currx++;
    //next row
    if (currx > 3) {
      currx = 0;
      curry++;
      //sets last row positions to current row
      for (int i = 0; i < 4; i++) {
        last_down[i] = now_down[i];
        now_down[i] = false;
      }
    }
  }
  //if all blocks free
  if (move) {
    (*y)++;
    //move all blocks in tetrimino down one
    for (int i = 0; i < 4; i++) {
      vga_ctrl->VRAM[blocks[i] + 10 + offset] = *select + 1;
      vga_ctrl->VRAM[blocks[i] + offset] = 0b00000000000000000000000000000000;
      occupied[blocks[i] + offset] = 0;
      occupied[blocks[i] + 10 + offset] = *select + 1;
    }
  }
  //if can't move, checkfor full lines and create another tetrimino
  else {
    currLines = removeRow(*y, offset);
    *lines += currLines;
    //generate score based on level and number of rows broken
    switch (currLines) {
    case 0:
      break;
    case 1:
      *score += 1 * (*level);
      break;
    case 2:
      *score += 3 * (*level);
      break;
    case 3:
      *score += 5 * (*level);
      break;
    case 4:
      *score += 8 * (*level);
      break;
    }
    //check for level increase
    if ((*lines) >= (*level) * 5 + *levelThreshold) {
      *levelThreshold += (*level) * 5;
      (*level)++;
    }
    //resets to top of screen
    *x = 3;
    *y = 0;
    *select = genTet();
    *curr_tet = pieces[*select][0];
    *flag = placeNewTet(*curr_tet, *select + 1, offset);
  }
}
//moves current tetrimino left
void left(int* x,int* y,int* curr_tet, int* select, int offset){
  int currx = 0;
  int curry = 0;
  bool move = true;
  bool last = false;
  int blocks[4] = {0, 0, 0, 0}; //blocks to move
  int block_count = 0;
  //top to bottom
  for (int bit = 0x8000; bit > 0; bit = bit >> 1) {
    if ((*curr_tet & bit) != 0) {
      //left to right
      if (!isUsed(currx - 1 + *x, curry + *y, offset) || last) {
        blocks[block_count] = (curry + *y) * 10 + currx + *x;
        block_count++;
        last = true;
      } else {
        move = false;
        break;
      }
    } else {
      last = false;
    }
    currx++;
    if (currx > 3) {
      currx = 0;
      curry++;
    }
  }
  if (move) {
    *x -= 1; //moves 4x4 grid left
    for (int i = 0; i < 4; i++) {

      vga_ctrl->VRAM[blocks[i] - 1 + offset] = *select + 1;
      vga_ctrl->VRAM[blocks[i] + offset] = 0b00000000000000000000000000000000;
      occupied[blocks[i] + offset] = 0;
      occupied[blocks[i] - 1 + offset] = *select + 1;
    }
  }
}
//moves tetrinimo right
void right(int* x,int* y,int* curr_tet, int* select, int offset) {
  int currx = 0;
  int curry = 0;
  bool move = true;
  bool last = false;
  int blocks[4] = {0, 0, 0, 0}; //moves right
  int block_count = 0;
  //bottom to top
  for (int bit = 1; bit <= 0x8000; bit = bit << 1) {
    if ((*curr_tet & bit) != 0) {
      //right to left
      if (!isUsed(-currx + 1 + *x + 3, -curry + *y + 3, offset) || last) {

        blocks[block_count] = (-curry + *y + 3) * 10 + -currx + *x + 3;
        block_count++;
        last = true;
      } else {
        move = false;
        break;
      }
    } else {
      last = false;
    }
    currx++;
    if (currx > 3) {
      currx = 0;
      curry++;
      last = false;
    }
  }
  if (move) {
    *x += 1; //moves 4x4 grid right
    for (int i = 0; i < 4; i++) {
      vga_ctrl->VRAM[blocks[i] + 1 + offset] = *select + 1;
      vga_ctrl->VRAM[blocks[i] + offset] = 0b00000000000000000000000000000000;
      occupied[blocks[i] + offset] = 0;
      occupied[blocks[i] + 1 + offset] = *select + 1;
    }
  }
}
//rotate tetrimino clockwise, works when destination blocks are empty. ignores blocks in the way
void rotate(int*curr_rot,int* x,int* y,int* curr_tet, int* select, int offset){
	int currx =0;
	int curry =0;
	int new_rot;
	int blocks[4] = {0,0,0,0}; //current blocks
	int block_count =0;
	int rot_blocks[4] = {0,0,0,0}; //blocks after rotaion
	bool move = true;
	new_rot = (*curr_rot+1)%4; //next index in pieces 
	int rot_count =0;
	//top to bottom
        for (int bit = 0x8000; bit > 0; bit = bit >> 1) {

          if ((pieces[*select][new_rot] & bit) != 0) {
	    //left to right of destination
            if (!isUsed(currx + *x, curry + *y, offset) ||
                (*curr_tet & bit) != 0) {
              blocks[block_count] = (curry + *y) * 10 + currx + *x;
              block_count++;
            } else {
              move = false;
              break;
            }
          }
	  //left to right of current tetrimino
	  else if ((*curr_tet & bit) != 0) {
            rot_blocks[rot_count] = (curry + *y) * 10 + currx + *x;
            rot_count++;
          }
	  
          currx++;
          if (currx > 3) {
            currx = 0;
            curry++;
          }
        }
	//places new rotation at appropriate place
        if (move) {
          *curr_tet = pieces[*select][new_rot];
          *curr_rot = (*curr_rot + 1) % 4;
          for (int i = 0; i < 4; i++) {
            vga_ctrl->VRAM[rot_blocks[i] + offset] = 0b0000000000000000000000000000000;
            vga_ctrl->VRAM[blocks[i] + offset] = *select + 1;
            occupied[rot_blocks[i] + offset] = 0;
            occupied[blocks[i] + offset] = *select + 1;
          }
        }
}

int main() {

	MAX3421E_init();
	USB_init();

	//waits for player to press space before game starts, this time will be a random
	//number which can be used to initialiaze srand
	int count = 0;
	while(*returnKeys() != 0x2C){
		count ++;
	}
	//intializes random with counter, better than system time
	srand(count);

	//strings used for printing winner
	char* win = " WINNER";
	char* lose = " LOSER";
	
//player1 local
	BYTE* curr; //this can be wasd or 8456
	int x = 3;
	int y = 0;
	int curr_tet = 0;
	int curr_rot = 0;
	int select = 0;
	bool flag = true;
	int lines = 0;
	int score = 0;
	int level = 1;
	int levelThreshold = 0;
	char score_string[80];
	char level_string[80];
	char line_string[80];
	BYTE key1;
	BYTE key2;
	bool one = false;
	bool two = false;

//player 2 local
	int x2 = 3;
	int y2 = 0;
	int curr_tet2 = 0;
	int curr_rot2 = 0;
	int select2 = 0;
	bool flag2 = true;
	int lines2 = 0;
	int score2 = 0;
	int level2 = 5;
	int levelThreshold2 = 0;
	char score_string2[80];
	char level_string2[80];
	char line_string2[80];

	//sets the color pallete using colors provided
        for (int i = 0; i < 16; i++) {
          setColorPalette(i, colors[i].red, colors[i].green, colors[i].blue);
        }
	//runs start animation
        initGame();
	//player 1 start
	select = genTet();
	curr_tet = pieces[select][0];
	flag = placeNewTet(curr_tet,select+1,0);
	//player 2 start
	select2 = genTet();
	curr_tet2 = pieces[select2][0];
	flag2 = placeNewTet(curr_tet2,select2+1,200);


	//timers for each player used to determine game speed
	clock_t start1 = clock();
	clock_t start2 = clock();
	clock_t end1;
	clock_t end2;
	//main game loop
	while (1) {
	  
          // draw scores,lines, and levels
          sprintf(score_string, " Score : %d", score);
          textVGADrawColorText(score_string, 436);
          sprintf(level_string, " Level : %d", level);
          textVGADrawColorText(level_string, 448);
          sprintf(line_string, " Lines : %d", lines);
          textVGADrawColorText(line_string, 460);

          sprintf(score_string2, " Score : %d", score2);
          textVGADrawColorText(score_string2, 400);
          sprintf(level_string2, " Level : %d", level2);
          textVGADrawColorText(level_string2, 412);
          sprintf(line_string2, " Lines : %d", lines2);
          textVGADrawColorText(line_string2, 424);

	  //gets current key
          curr = returnKeys();
          key1 = 0x00;
          key2 = 0x00;
	  //bools for if player already has a key
          one = false;
          two = false;

          // get either player keys
          for (int i = 0; i < 6; i++) {
	    //wasd
            if ((curr[i] == 0x1A || curr[i] == 0x04 || curr[i] == 0x07 ||
                 curr[i] == 0x16) &&
                !one) {
              key1 = curr[i];
              one = true;
            }
	    //8456 on keypad
	    else if ((curr[i] == 0x5C || curr[i] == 0x5E || curr[i] == 0x5D ||
                        curr[i] == 0x60) &&
                       !two) {
              key2 = curr[i];
              two = true;
            }
	    //if both players have an input
	    else if (one && two) {
              break;
            }
          }
          // pause condition esc to start, space to stop
          if (*curr == 0x29) {
            while (*curr != 0x2C) {
              curr = returnKeys();
            }
            usleep(250000);
          }

	  //end condition when both players have lost
          if (!flag && !flag2) {
	    //player1 wins
            if (score > score2) {
              textVGADrawColorText(win, 436);
              textVGADrawColorText(win, 448);
              textVGADrawColorText(win, 460);
              textVGADrawColorText(lose, 400);
              textVGADrawColorText(lose, 412);
              textVGADrawColorText(lose, 424);
            }
	    //player2 wins
	    else if (score2 > score) {
              textVGADrawColorText(lose, 436);
              textVGADrawColorText(lose, 448);
              textVGADrawColorText(lose, 460);
              textVGADrawColorText(win, 400);
              textVGADrawColorText(win, 412);
              textVGADrawColorText(win, 424);
            }//if tie score, higher level wins
	    //player1 wins
	    else if (level >= level2) {
              textVGADrawColorText(win, 436);
              textVGADrawColorText(win, 448);
              textVGADrawColorText(win, 460);
              textVGADrawColorText(lose, 400);
              textVGADrawColorText(lose, 412);
              textVGADrawColorText(lose, 424);
            }
	    //player2 wins
	    else if (level2 > level) {
              textVGADrawColorText(lose, 436);
              textVGADrawColorText(lose, 448);
              textVGADrawColorText(lose, 460);
              textVGADrawColorText(win, 400);
              textVGADrawColorText(win, 412);
              textVGADrawColorText(win, 424);
            }
	    //check for restarat
            curr = returnKeys();
            while (*curr != 0x2C) {
              curr = returnKeys();
            }
	    //reset game
            usleep(500000);
            initGame();
	    //player 1 vars
            select = genTet();
            score = 0;
            level = 1;
            lines = 0;
            levelThreshold = 0;
            curr_tet = pieces[select][0];
            flag = placeNewTet(curr_tet, select + 1, 0);
            curr_rot = 0;
            x = 3;
            y = 0;

	    //player2 vars
            select2 = genTet();
            score2 = 0;
            level2 = 5;
            lines2 = 0;
            levelThreshold2 = 0;
            curr_tet2 = pieces[select2][0];
            flag2 = placeNewTet(curr_tet2, select2 + 1, 200);
            curr_rot2 = 0;
            x2 = 3;
            y2 = 0;
          }
          // down

	  //timing condiiton
	  //checks difference from last drop of each player, if greater than value set by level, move down
          end1 = clock();
          end2 = clock();

          if ((double)(end1 - start1) / (double)CLOCKS_PER_SEC >=
                  0.5 - (0.05 * level) &&
              flag) {
            start1 = clock();
            Down(&x, &y, &curr_tet, &lines, &score, &level, &levelThreshold,
                 &select, &flag, 0);
          }
          if ((double)(end2 - start2) / (double)CLOCKS_PER_SEC >=
                  0.5 - (0.05 * level2) &&
              flag2) {
            start2 = clock();
            Down(&x2, &y2, &curr_tet2, &lines2, &score2, &level2,
                 &levelThreshold2, &select2, &flag2, 200);
          }
   
	  //player1 wasd
          if (flag) {
            switch (key1) {
            // rotate
            case 0x1A:
              rotate(&curr_rot, &x, &y, &curr_tet, &select, 0);
              break;
            // left
            case 0x04:
              left(&x, &y, &curr_tet, &select, 0);
              break;

            // right
            case 0x07:
              right(&x, &y, &curr_tet, &select, 0);
              break;
            case 0x16:
              // down by control
              Down(&x, &y, &curr_tet, &lines, &score, &level, &levelThreshold,
                   &select, &flag, 0);
              break;
            }
          }
	  //player2 8456
          if (flag2) {
            switch (key2) {
            case 0x60:
              rotate(&curr_rot2, &x2, &y2, &curr_tet2, &select2, 200);
              break;
            // left
            case 0x5C:
              left(&x2, &y2, &curr_tet2, &select2, 200);
              break;

            // right
            case 0x5E:
              right(&x2, &y2, &curr_tet2, &select2, 200);
              break;
            case 0x5D:
              // down by control
              Down(&x2, &y2, &curr_tet2, &lines2, &score2, &level2,
                   &levelThreshold2, &select2, &flag2, 200);
              break;
            }
          }
        }
        return 0;
}


