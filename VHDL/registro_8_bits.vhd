----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:32:18 04/06/2015 
-- Design Name: 
-- Module Name:    registro_8_bits - Behavioral 
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
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity registro_8_bits is
    Port ( d : in  STD_LOGIC_VECTOR (7 downto 0);
           ce : in  STD_LOGIC;
           clk : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           q : out  STD_LOGIC_VECTOR (7 downto 0));
end registro_8_bits;

architecture Behavioral of registro_8_bits is

signal q_interna: STD_LOGIC_VECTOR (7 downto 0);

begin

q<=q_interna;

process(clk, ce, reset)
begin
	if (clk'event and clk = '1') then
		if reset='1' then
			q_interna<="00000000";
	   elsif ce='1' then
			q_interna<=d;
		else
			q_interna<=q_interna;
		end if;
	end if;

end process;

end Behavioral;

