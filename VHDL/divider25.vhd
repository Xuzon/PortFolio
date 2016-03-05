------------------------------------------------------------------
--  divider25.vhd -- 
-- This circuit generates a clock signal with a frequency 25 times slower
-- than the input clock frequency
-- except the first time, that will be 24 times slower
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

entity divider25 is
    Port ( clkin : in std_logic;
           clkout : out std_logic;
			  reset: in std_logic
			  );
end divider25;

architecture Behavioral of divider25 is

signal count: integer range 0 to 24:=0;
signal clkout_aux: std_logic:='0';

begin

clkout <= clkout_aux;

process (reset, clkin)
begin
	if reset = '1' then 	clkout_aux <='0';
				count <= 0;
 	elsif (clkin='1' and clkin'event) then
			if (count = 12) or (count = 24) then clkout_aux <= not clkout_aux;
			end if;
			if count = 24 then 	count <= 0;
			else count <= count+1;
			end if;
	end if;
end process;

end Behavioral;
