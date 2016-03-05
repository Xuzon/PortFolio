----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:19:08 11/10/2015 
-- Design Name: 
-- Module Name:    PWM - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity PWM is
    Port ( lum_pot : in  STD_LOGIC_VECTOR (7 downto 0);
           reset : in  STD_LOGIC;
           clk : in  STD_LOGIC;
           lum_pulse : out  STD_LOGIC);
end PWM;

architecture Behavioral of PWM is
signal cont : integer range 0 to 255;
signal contMax : integer range 0 to 255;
begin
process(lum_pot,reset,clk)
begin
	
	if(reset='1') then
		cont <= 0;
		contMax <= 0;
		lum_pulse <= '0';
	elsif(clk'event and clk = '1') then
			if(std_logic_vector(to_unsigned(cont,8)) = lum_pot) then
				lum_pulse <= '0';
			else 	
				cont <= cont + 1;
				lum_pulse <= '1';
			end if;
			contMax <= contMax + 1;
			if(contMax = 255) then
				contMax <= 0;
				cont <= 0;
			end if;
		end if;
end process;
end Behavioral;

