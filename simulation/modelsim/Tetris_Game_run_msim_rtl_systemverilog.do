transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlib Finalsoc
vmap Finalsoc Finalsoc
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/Finalsoc.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_reset_controller.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_reset_synchronizer.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_avalon_st_adapter_006.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_avalon_st_adapter.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_avalon_sc_fifo.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_usb_rst.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_usb_gpx.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_timer_0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_sysid_qsys_0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_spi0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_sdram_pll.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_sdram.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_onchip_memory2_0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_nios2_gen2_0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_nios2_gen2_0_cpu.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_nios2_gen2_0_cpu_debug_slave_sysclk.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_nios2_gen2_0_cpu_debug_slave_tck.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_nios2_gen2_0_cpu_debug_slave_wrapper.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_nios2_gen2_0_cpu_test_bench.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_leds_pio.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_keycode.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_key.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_jtag_uart_0.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_hex_digits_pio.v}
vlog -vlog01compat -work work +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/ram2.v}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_irq_mapper.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_avalon_st_adapter_006_error_adapter_0.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_avalon_st_adapter_error_adapter_0.sv}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_avalon_st_handshake_clock_crosser.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_avalon_st_clock_crosser.v}
vlog -vlog01compat -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_std_synchronizer_nocut.v}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_width_adapter.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_burst_uncompressor.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_rsp_mux.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_arbitrator.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_rsp_demux.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_cmd_mux.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_cmd_demux.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_burst_adapter.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_burst_adapter_uncmpr.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_router_008.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_router_002.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/Finalsoc_mm_interconnect_0_router.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_slave_agent.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_master_agent.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_slave_translator.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/altera_merlin_master_translator.sv}
vlog -sv -work Finalsoc +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Finalsoc/synthesis/submodules/vga_text_avl_interface.sv}
vlog -sv -work work +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/VGA_controller.sv}
vlog -sv -work work +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/font_rom.sv}
vlog -sv -work work +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/HexDriver.sv}
vlog -sv -work work +incdir+C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project {C:/Users/shenj/OneDrive/Desktop/ECE385_Final_Project/Tetris_Game.sv}

