----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:42:07 09/17/2015 
-- Design Name: 
-- Module Name:    seleccion_binaria_8_perifericos_entrada_con_registro - Behavioral 
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

entity seleccion_binaria_8_perifericos_entrada_con_registro is
    Port ( puerto_00_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_01_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_02_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_03_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_04_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_05_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_06_in : in  STD_LOGIC_VECTOR (7 downto 0);
           puerto_07_in : in  STD_LOGIC_VECTOR (7 downto 0);
           port_id : in  STD_LOGIC_VECTOR (7 downto 0);
           clk_micro : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           in_port : out  STD_LOGIC_VECTOR (7 downto 0));
end seleccion_binaria_8_perifericos_entrada_con_registro;


architecture Behavioral of seleccion_binaria_8_perifericos_entrada_con_registro is

COMPONENT mux8c_8b
	PORT(
		canal_0_entrada : IN std_logic_vector(7 downto 0);
		canal_1_entrada : IN std_logic_vector(7 downto 0);
		canal_2_entrada : IN std_logic_vector(7 downto 0);
		canal_3_entrada : IN std_logic_vector(7 downto 0);
		canal_4_entrada : IN std_logic_vector(7 downto 0);
		canal_5_entrada : IN std_logic_vector(7 downto 0);
		canal_6_entrada : IN std_logic_vector(7 downto 0);
		canal_7_entrada : IN std_logic_vector(7 downto 0);
		sel : IN std_logic_vector(2 downto 0);          
		mux_out : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;
	
COMPONENT register_8_bits
	PORT(
		d : IN std_logic_vector(7 downto 0);
		ce : IN std_logic;
		clk : IN std_logic;
		reset : IN std_logic;          
		q : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

SIGNAL salida_m : STD_LOGIC_VECTOR(7 downto 0);
SIGNAL selector : STD_LOGIC_VECTOR(2 downto 0);

begin

	selector<=port_id(2) & port_id(1) & port_id(0);

	Inst_mux8c_8b: mux8c_8b PORT MAP(
		canal_0_entrada => puerto_00_in,
		canal_1_entrada => puerto_01_in,
		canal_2_entrada => puerto_02_in,
		canal_3_entrada => puerto_03_in,
		canal_4_entrada => puerto_04_in,
		canal_5_entrada => puerto_05_in,
		canal_6_entrada => puerto_06_in,
		canal_7_entrada => puerto_07_in,
		sel => selector,
		mux_out => salida_m
	);
	
	Inst_register_8_bits: register_8_bits PORT MAP(
		d => salida_m,
		ce => '1',
		clk => clk_micro,
		reset => reset,
		q => in_port
	);


end Behavioral;

