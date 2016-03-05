--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   16:46:03 11/10/2015
-- Design Name:   
-- Module Name:   C:/Alumnos/CEP/CEP4/Practica_6/LUT/test_pwm.vhd
-- Project Name:  sistema_empotrado_basico
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: PWM
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
 
ENTITY test_pwm IS
END test_pwm;
 
ARCHITECTURE behavior OF test_pwm IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT PWM
    PORT(
         lum_pot : IN  std_logic_vector(7 downto 0);
         reset : IN  std_logic;
         clk : IN  std_logic;
         lum_pulse : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal lum_pot : std_logic_vector(7 downto 0) := (others => '0');
   signal reset : std_logic := '0';
   signal clk : std_logic := '0';

 	--Outputs
   signal lum_pulse : std_logic;

   -- Clock period definitions
   constant clk_period : time := 20 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: PWM PORT MAP (
          lum_pot => lum_pot,
          reset => reset,
          clk => clk,
          lum_pulse => lum_pulse
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
      reset <= '1';
		wait for 500 ns;
		reset <= '0';
		lum_pot <= "00001111";
		wait for 255 * clk_period;
		lum_pot <= "11000011";
		wait for 255 * clk_period;
		lum_pot <= "11111111";
		wait for 255 * clk_period;
		assert (false) report "Fin simulacion. NO es un error" severity FAILURE;
   end process;

END;
