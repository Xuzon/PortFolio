----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    17:41:48 09/17/2015 
-- Design Name: 
-- Module Name:    decodificador_1_entre_8_con_buses - Behavioral 
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

entity decodificador_1_entre_8_con_buses is
    Port ( sel : in  STD_LOGIC_VECTOR (2 downto 0);
           enable : in  STD_LOGIC;
           q : out  STD_LOGIC_VECTOR (7 downto 0));
end decodificador_1_entre_8_con_buses;

architecture Behavioral of decodificador_1_entre_8_con_buses is

begin

process(sel, enable)
begin
      if(enable='1') then
         case sel is
            when "000" => q <= "00000001";
            when "001" => q <= "00000010";
            when "010" => q <= "00000100";
            when "011" => q <= "00001000";
            when "100" => q <= "00010000";
            when "101" => q <= "00100000";
            when "110" => q <= "01000000";
            when "111" => q <= "10000000";
            when others => q <= "00000000";
         end case;
      end if;

end process;


end Behavioral;

