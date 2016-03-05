----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    20:06:11 10/16/2015 
-- Design Name: 
-- Module Name:    antirrebotes - Behavioral 
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

entity antirrebotes is
    Port ( clk : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           salida : out  STD_LOGIC_VECTOR (7 downto 0));
end antirrebotes;


architecture Behavioral of antirrebotes is

signal shift_reg_0 : std_logic_vector (2 downto 0);
signal shift_reg_1 : std_logic_vector (2 downto 0);
signal shift_reg_2 : std_logic_vector (2 downto 0);
signal shift_reg_3 : std_logic_vector (2 downto 0);
signal shift_reg_4 : std_logic_vector (2 downto 0);
signal shift_reg_5 : std_logic_vector (2 downto 0);
signal shift_reg_6 : std_logic_vector (2 downto 0);
signal shift_reg_7 : std_logic_vector (2 downto 0);

begin

proceso: process(clk, reset) is
  begin
    if (reset='1') then
     shift_reg_0 <= "000";
	  shift_reg_1 <= "000";
	  shift_reg_2 <= "000";
	  shift_reg_3 <= "000";
	  shift_reg_4 <= "000";
	  shift_reg_5 <= "000";
	  shift_reg_6 <= "000";
	  shift_reg_7 <= "000";
   elsif (clk'event and clk='1') then
     shift_reg_0 <= entrada(0) & shift_reg_0(2 downto 1);
	  shift_reg_1 <= entrada(1) & shift_reg_1(2 downto 1);
	  shift_reg_2 <= entrada(2) & shift_reg_2(2 downto 1);
	  shift_reg_3 <= entrada(3) & shift_reg_3(2 downto 1);
	  shift_reg_4 <= entrada(4) & shift_reg_4(2 downto 1);
	  shift_reg_5 <= entrada(5) & shift_reg_5(2 downto 1);
	  shift_reg_6 <= entrada(6) & shift_reg_6(2 downto 1);
	  shift_reg_7 <= entrada(7) & shift_reg_7(2 downto 1);
   end if;
  end process proceso;

salida(0) <= shift_reg_0(2) and shift_reg_0(1) and shift_reg_0(0);
salida(1) <= shift_reg_1(2) and shift_reg_1(1) and shift_reg_1(0);
salida(2) <= shift_reg_2(2) and shift_reg_2(1) and shift_reg_2(0);
salida(3) <= shift_reg_3(2) and shift_reg_3(1) and shift_reg_3(0);
salida(4) <= shift_reg_4(2) and shift_reg_4(1) and shift_reg_4(0);
salida(5) <= shift_reg_5(2) and shift_reg_5(1) and shift_reg_5(0);
salida(6) <= shift_reg_6(2) and shift_reg_6(1) and shift_reg_6(0);
salida(7) <= shift_reg_7(2) and shift_reg_7(1) and shift_reg_7(0);

end Behavioral;

