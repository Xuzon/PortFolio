--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   14:12:57 11/17/2015
-- Design Name:   
-- Module Name:   C:/Users/daat/Desktop/sistema_empotrado_basico/test_top_placa.vhd
-- Project Name:  sistema_empotrado_basico
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: top_template_picoblaze_3_placa_Nexys_2
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY test_top_placa IS
END test_top_placa;
 
ARCHITECTURE behavior OF test_top_placa IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT top_template_picoblaze_3_placa_Nexys_2
    PORT(
         clk : IN  std_logic;
         SW : IN  std_logic_vector(7 downto 0);
         BTN : IN  std_logic_vector(3 downto 0);
         sdata : IN  std_logic;
         LEDs : OUT  std_logic_vector(7 downto 0);
			cs : OUT std_logic;
			sclk : OUT std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal SW : std_logic_vector(7 downto 0) := (others => '0');
   signal BTN : std_logic_vector(3 downto 0) := (others => '0');
   signal sdata : std_logic := '0';

 	--Outputs
   signal LEDs : std_logic_vector(7 downto 0);
	signal cs : std_logic := '0';
	signal sclk : std_logic := '0';

   -- Clock period definitions
   constant clk_period : time := 20 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: top_template_picoblaze_3_placa_Nexys_2 PORT MAP (
          clk => clk,
          SW => SW,
          BTN => BTN,
          sdata => sdata,
			 cs => cs,
			 sclk => sclk,
          LEDs => LEDs
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      BTN(3) <= '1';
		wait for 500 ns;
		BTN(3) <= '0';
		sdata <= '1';
		wait for 500 ns;
		BTN(0) <= '1';
		wait for 500 ns;
		BTN(0) <= '1';
		SW <= "00001111";
		wait for 255 * clk_period;
		SW <= "11000011";
		wait for 255 * clk_period;
		SW <= "11111111";
		wait for 255 * clk_period;
		assert (false) report "Fin simulacion. NO es un error" severity FAILURE;
   end process;

END;
