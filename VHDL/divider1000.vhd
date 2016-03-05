------------------------------------------------------------------
--  divider1000.vhd -- 
-- This circuit generates a clock signal with a frequency 1000 times slower
-- than the input clock frequency
-- except the first time, that will be 999 times slower
------------------------------------------------------------------
-- Luis Jacobo Alvarez Ruiz de Ojeda
-- Dpto. Tecnologia Electronica
-- University of Vigo
-- 24, March, 2006 
------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.all;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity divider1000 is
    Port ( clkin : in std_logic;
           clkout : out std_logic;
           reset : in std_logic);
end divider1000;

architecture Behavioral of divider1000 is

signal count: integer range 0 to 499:=0;
signal clkout_aux: std_logic:='0';

begin

clkout <= clkout_aux;

process (reset,clkin)
begin
	if reset = '1' then 	clkout_aux <='0';
								count <= 0;
 	elsif
		clkin='1' and clkin'event then
			if count = 499 then clkout_aux <= not clkout_aux;
									 count <= 0;
			else count <= count+1;
			end if;
	end if;
end process;
end Behavioral;
