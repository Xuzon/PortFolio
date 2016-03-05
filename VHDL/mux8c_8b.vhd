----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:34:05 09/17/2015 
-- Design Name: 
-- Module Name:    mux8c_8b - Behavioral 
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

entity mux8c_8b is
    Port ( canal_0_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_1_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_2_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_3_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_4_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_5_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_6_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           canal_7_entrada : in  STD_LOGIC_VECTOR (7 downto 0);
           sel : in  STD_LOGIC_VECTOR (2 downto 0);
           mux_out : out  STD_LOGIC_VECTOR (7 downto 0));
end mux8c_8b;

architecture Behavioral of mux8c_8b is

begin

	mux_out <=  canal_0_entrada WHEN sel="000" ELSE
					canal_1_entrada WHEN sel="001" ELSE
					canal_2_entrada WHEN sel="010" ELSE
					canal_3_entrada WHEN sel="011" ELSE
					canal_4_entrada WHEN sel="100" ELSE
					canal_5_entrada WHEN sel="101" ELSE
					canal_6_entrada WHEN sel="110" ELSE
					canal_7_entrada WHEN sel="111" ELSE
					"00000000";

end Behavioral;

