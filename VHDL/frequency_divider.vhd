------------------------------------------------------------------
--  frequency_divider.vhd -- 
-- This circuit generates 3 clock signals from the S3 board 50MHz clock
-- The frequencies of the generated clocks are 2 MHz, 1 MHz and 1 kHz
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

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity frequency_divider is
    Port ( clk50MHz : in std_logic;
           reset : in std_logic;
           clk2MHz : out std_logic;
           clk1MHz : out std_logic;
           clk1kHz : out std_logic
			  );
end frequency_divider;

architecture Behavioral of frequency_divider is

------------------------------------------------------------------
--  Signal Declarations and Constants
------------------------------------------------------------------

signal clk1MHz_int, clk2MHz_int, clk1kHz_int: std_logic:='0';
signal clk1MHz_aux, clk2MHz_aux, clk1kHz_aux: std_logic:='0';

------------------------------------------------------------------
--  Component Declarations
------------------------------------------------------------------


-- Frequency divider by factor 50
	COMPONENT divider50
	PORT(
		clkin : IN std_logic;
		reset : IN std_logic;       
		clkout : OUT std_logic
		);
	END COMPONENT;

-- Frequency divider by factor 25
	COMPONENT divider25
	PORT(
		clkin : IN std_logic;
		reset : IN std_logic;       
		clkout : OUT std_logic
		);
	END COMPONENT;

-- Frequency divider by factor 1000
	COMPONENT divider1000
	PORT(
		clkin : IN std_logic;
		reset : IN std_logic;       
		clkout : OUT std_logic
		);
	END COMPONENT;

begin

-- Output assignments

clk2MHz <= clk2MHz_aux;
clk1MHz <= clk1MHz_aux;
clk1kHz <= clk1kHz_aux;


-- frequency divider 1 instantiation
	freq_divider_1: divider50 PORT MAP(
		clkin => clk50MHz,
		clkout => clk1MHz_int,
		reset => reset
	);

-- global clock buffer instantiation for clk1MHz
   BUFG_inst_1 : BUFG
   port map (
      O => clk1MHz_aux,     -- Clock buffer output
      I => clk1MHz_int      -- Clock buffer input
   );

-- frequency divider 2 instantiation
	freq_divider2: divider25 PORT MAP(
		clkin => clk50MHz,
		clkout => clk2MHz_int,
		reset => reset
	);

-- global clock buffer instantiation for clk2MHz
   BUFG_inst_2 : BUFG
   port map (
      O => clk2MHz_aux,     -- Clock buffer output
      I => clk2MHz_int      -- Clock buffer input
   );

-- frequency divider 3 instantiation
	freq_divider_3: divider1000 PORT MAP(
		clkin => clk1MHz_aux,
		clkout => clk1kHz_int,
		reset => reset
	);

-- global clock buffer instantiation for clk1kHz
   BUFG_inst_3 : BUFG
   port map (
      O => clk1kHz_aux,     -- Clock buffer output
      I => clk1kHz_int      -- Clock buffer input
   );



end Behavioral;
