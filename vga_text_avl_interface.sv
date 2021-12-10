

module vga_text_avl_interface (
	// Avalon Clock Input, note this clock is also used for VGA, so this must be 50Mhz
	// We can put a clock divider here in the future to make this IP more generalizable
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,					// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,			// Avalon-MM Byte Enable
	input  logic [11:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data
	
	// Exported Conduit (mapped to VGA port - make sure you export in Platform Designer)
	output logic [3:0]  red, green, blue,	// VGA color channels (mapped to output pins in top-level)
	output logic hs, vs						// VGA HS/VS
);

logic [31:0] pallette       [16]; // Registers
//put other local variables here
logic pixel_clk_anh;
logic blank_anh;
logic sync_anh;
logic [9:0] DrawX_nh;
logic [9:0] DrawY_nh;
logic [10:0] current_char_x;
logic [10:0] current_char_y;
logic [31:0] current_word;
logic [7:0] current_char; 
logic [10:0] addr_nh;
logic [7:0] data_nh;
logic [3:0] back;
logic [3:0] front;
logic [31:0]Q; 
logic [10:0] read_addr;
logic wren;
//logic [10:0] left;
//logic [10:0] right;
//logic rden;
//addr_nh[10:7] = 4'b0000;
 
assign addr_nh = 16*Q + DrawY_nh[3:0];
//Declare submodules..e.g. VGA controller, ROMS, etc
vga_controller a1(.Clk(CLK),.Reset(RESET),.hs(hs),.vs(vs),.pixel_clk(pixel_clk_anh),.blank(blank_anh),.sync(sync_anh),.DrawX(DrawX_nh),.DrawY(DrawY_nh));
font_rom a2(.addr(addr_nh),.data(data_nh));
	
   
// Read and write from AVL interface to register block, note that READ waitstate = 1, so this should be in always_ff
always_ff @(posedge CLK) begin
if(AVL_CS&&AVL_WRITE)
begin
//rden = 0;
if(AVL_ADDR >= 472)
begin
wren <= 0;
//AVL_READDATA = LOCAL_REG[AVL_ADDR[2:0]];
case (AVL_BYTE_EN)
    4'b1111: pallette[AVL_ADDR[2:0]] = AVL_WRITEDATA;
	 4'b1100: pallette[AVL_ADDR[2:0]][31:16] = AVL_WRITEDATA[31:16];
	 4'b0011: pallette[AVL_ADDR[2:0]][15:0] = AVL_WRITEDATA[15:0];
	 4'b1000: pallette[AVL_ADDR[2:0]][31:24] = AVL_WRITEDATA[31:24];
    4'b0100: pallette[AVL_ADDR[2:0]][23:16] = AVL_WRITEDATA[23:16];
    4'b0010: pallette[AVL_ADDR[2:0]][15:8] = AVL_WRITEDATA[15:8];
    4'b0001: pallette[AVL_ADDR[2:0]][7:0] = AVL_WRITEDATA[7:0];
endcase	
end
else
begin
wren <= AVL_WRITE;
end
end
if(AVL_CS && AVL_READ)
begin
if(AVL_ADDR>=472)
begin
AVL_READDATA <= pallette[AVL_ADDR -472];
end
else
begin
AVL_READDATA <= Q;
end
end
end

//handle drawing (may either be combinational or sequential - or both).
always_comb
begin:block_display
//left = ((((DrawX_nh - 140) >> 4)+1) *3)-1;
//right =((((DrawX_nh - 140) >> 4)) *3); 
if (DrawX_nh > 140 && DrawX_nh <= 300 && DrawY_nh > 100 && DrawY_nh <= 420)//player one game display
begin
  read_addr = ((DrawY_nh-100) >> 4)*10 + ((DrawX_nh - 140) >> 4);
   //addr_nh = 0; 
end


else if (DrawX_nh > 136 && DrawX_nh <= 232 && DrawY_nh > 16 && DrawY_nh <= 32)//player 1 score display
begin
  read_addr = (((DrawX_nh-136) >> 3)+436);
  //addr_nh = 16*Q + DrawY_nh[3:0]; 
end
else if (DrawX_nh > 136 && DrawX_nh <= 232 && DrawY_nh > 32 && DrawY_nh <= 48) //player 1 level display
begin
  read_addr = (((DrawX_nh-136) >> 3)+448);

end
else if (DrawX_nh > 136 && DrawX_nh <= 232 && DrawY_nh > 48 && DrawY_nh <= 64) //player 1 lines display
begin
  read_addr = (((DrawX_nh-136) >> 3)+460);

end
else if (DrawX_nh > 340  && DrawX_nh <= 500 && DrawY_nh > 100 && DrawY_nh <= 420) // player 2 game 
begin 
  read_addr = (((DrawY_nh-100) >> 4)*10 + ((DrawX_nh - 340) >> 4)+ 200);
end
else if (DrawX_nh > 336 && DrawX_nh <= 432 && DrawY_nh > 16 && DrawY_nh <= 32) //player2 score
begin
 read_addr = (((DrawX_nh-336) >> 3)+200+200);
end
else if (DrawX_nh > 336 && DrawX_nh <= 432 && DrawY_nh > 32 && DrawY_nh <= 48) // player 2 level
begin 
   read_addr = (((DrawX_nh-336) >> 3)+212+200);
end
else if (DrawX_nh > 336 && DrawX_nh <= 432 && DrawY_nh > 48 && DrawY_nh <= 64) // player 2 line
begin
   read_addr = (((DrawX_nh-336) >> 3)+224 + 200);
end
else 
begin
  read_addr = 0;
  //addr_nh = 0; 
end
end




always_ff@(posedge pixel_clk_anh)
begin:Color_Display
		
      if(~blank_anh)
		begin
		    red = 0;
			 green = 0;
			 blue = 0;
		end
	
		else if (DrawX_nh > 140 && DrawX_nh <= 300 && DrawY_nh > 100 && DrawY_nh <= 420) //player1
		begin
			
		   red = pallette[Q[3:0]][11:8];
			green = pallette[Q[3:0]][7:4];
			blue = pallette[Q[3:0]][3:0];
		end
		else if (DrawX_nh > 136 && DrawX_nh <= 232 && DrawY_nh > 16 && DrawY_nh <= 32) //player1 score_section
		begin
		
			 if(data_nh[7-DrawX_nh[2:0]])
			 begin

			 red = 0;
          green = 0;
          blue = 0;
			 end
			 
			 else
			 begin
			 red = 15;
          green = 0;
          blue = 15;
			 end
			
		end
		else if (DrawX_nh > 136 && DrawX_nh <= 232 && DrawY_nh > 32 && DrawY_nh <= 48) //player1 level_section
		begin
		
			 if(data_nh[7-DrawX_nh[2:0]])
			 begin

			 red = 0;
          green = 0;
          blue = 0;
			 end
			 
			 else
			 begin
			 red = 15;
          green = 15;
          blue = 0;
			 end
			
		end
		else if (DrawX_nh > 136 && DrawX_nh <= 232 && DrawY_nh > 48 && DrawY_nh <= 64) //player1 lines_section
		begin
		
			 if(data_nh[7-DrawX_nh[2:0]])
			 begin

			 red = 0;
          green = 0;
          blue = 0;
			 end
			 
			 else
			 begin
			 red = 0;
          green = 15;
          blue = 15;
			 end
			
		end
               else if (DrawX_nh > 340  && DrawX_nh <= 500 && DrawY_nh > 100 && DrawY_nh <= 420) //player2
		begin
			
		   red = pallette[Q[3:0]][11:8];
			green = pallette[Q[3:0]][7:4];
			blue = pallette[Q[3:0]][3:0];
		end
		else if (DrawX_nh > 336 && DrawX_nh <= 432 && DrawY_nh > 16 && DrawY_nh <= 32) //player2 score_section
		begin
			if(data_nh[7-DrawX_nh[2:0]])
			 begin

			 red = 0;
          green = 0;
          blue = 0;
			 end
			 
			 else
			 begin
			 red = 15;
          green = 0;
          blue = 15;
			 end
			 
		end
			else if (DrawX_nh > 336 && DrawX_nh <= 432 && DrawY_nh > 32 && DrawY_nh <= 48) //player2 level_section
		begin
		
			 if(data_nh[7-DrawX_nh[2:0]])
			 begin

			 red = 0;
          green = 0;
          blue = 0;
			 end
			 
			 else
			 begin
			 red = 15;
          green = 15;
          blue = 0;
			 end
			
		end
		else if (DrawX_nh > 336 && DrawX_nh <= 432 && DrawY_nh > 48 && DrawY_nh <= 64) //player2 lines_section
		begin
		
			 if(data_nh[7-DrawX_nh[2:0]])
			 begin

			 red = 0;
          green = 0;
          blue = 0;
			 end
			 
			 else
			 begin
			 red = 0;
          green = 15;
          blue = 15;
			 end
			
		end
	        else 
		begin
		red = 0;
		green = 0;
   	blue = 0;
		end
				
	
		
end      

ram2 r1(.byteena_a(AVL_BYTE_EN),.clock(CLK),.data(AVL_WRITEDATA),.rdaddress(read_addr),.wraddress(AVL_ADDR),.wren(wren),.q(Q));

endmodule
