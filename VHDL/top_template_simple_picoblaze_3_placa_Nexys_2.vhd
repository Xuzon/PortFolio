------------------------------------------------------------------
-- top_template_picoblaze_3_placa_Nexys_2 -- Main file
-- Design for Nexys 2 board for Picoblaze
-- Without STARTUP component and without reset signal in the frequency_divider component
-- Both things will make the Picoblaze malfunction
------------------------------------------------------------------
-- Luis Jacobo Alvarez Ruiz de Ojeda
-- Dpto. Tecnologia Electronica
-- University of Vigo
-- 27, September, 2014 
-- Tool versions: ISE 14
-- Description: 
	-- Plantilla para la conexión del microprocesador Picoblaze con 8 periféricos de entrada,
	-- 8 periféricos de salida y 1 fuente de interrupción en la placa Nexys 2.
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.all;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity top_template_picoblaze_3_placa_Nexys_2 is
    Port (
	 -- INPUTS
	 clk : 	in std_logic; -- Clock signal (50 MHz)
	 SW: in std_logic_vector (7 downto 0);	-- Switches
	 BTN: in std_logic_vector (3 downto 0); -- Pushbuttons
	 sdata : in std_logic;

	-- OUTPUTS
	 LEDs: out std_logic_vector (7 downto 0); -- LEDs
	 cs : out std_logic;
	 sclk : out std_logic
	  );
end top_template_picoblaze_3_placa_Nexys_2;

architecture Behavioral of top_template_picoblaze_3_placa_Nexys_2 is

-- Declaración del divisor de reloj
	COMPONENT frequency_divider
	PORT(
		clk50MHz : IN std_logic;
		reset : IN std_logic;          
		clk2MHz : OUT std_logic;
		clk1MHz : OUT std_logic;
		clk1kHz : OUT std_logic
		);
	END COMPONENT;

-- Declaración del componente Picoblaze 3 empotrado
	COMPONENT picoblaze3_empotrado
	PORT(
		in_port : IN std_logic_vector(7 downto 0);
		interrupt : IN std_logic;
		reset : IN std_logic;
		clk : IN std_logic;          
		port_id : OUT std_logic_vector(7 downto 0);
		write_strobe : OUT std_logic;
		out_port : OUT std_logic_vector(7 downto 0);
		read_strobe : OUT std_logic;
		interrupt_ack : OUT std_logic
		);
	END COMPONENT;
	
--	Declaración del componente LUT
	COMPONENT consulta_luz
	PORT(
		dir_lut : IN std_logic_vector(7 downto 0);          
		luz_id : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

-- Declaración del componente de selección binaria de 8 periféricos de entrada con registro
	COMPONENT seleccion_binaria_8_perifericos_entrada_con_registro
	PORT(
		puerto_00_in : IN std_logic_vector(7 downto 0);
		puerto_01_in : IN std_logic_vector(7 downto 0);
		puerto_02_in : IN std_logic_vector(7 downto 0);
		puerto_03_in : IN std_logic_vector(7 downto 0);
		puerto_04_in : IN std_logic_vector(7 downto 0);
		puerto_05_in : IN std_logic_vector(7 downto 0);
		puerto_06_in : IN std_logic_vector(7 downto 0);
		puerto_07_in : IN std_logic_vector(7 downto 0);
		port_id : IN std_logic_vector(7 downto 0);
		clk_micro : IN std_logic;
		reset : IN std_logic;          
		in_port : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

-- Declaración del componente de selección binaria de 8 periféricos de salida con registro
	COMPONENT seleccion_binaria_8_perifericos_salida_con_registro
	PORT(
		out_port : IN std_logic_vector(7 downto 0);
		port_id : IN std_logic_vector(7 downto 0);
		clk_micro : IN std_logic;
		reset : IN std_logic;
		write_strobe : IN std_logic;          
		out_port_reg : OUT std_logic_vector(7 downto 0);
		sel_puerto_0_out : OUT std_logic;
		sel_puerto_1_out : OUT std_logic;
		sel_puerto_2_out : OUT std_logic;
		sel_puerto_3_out : OUT std_logic;
		sel_puerto_4_out : OUT std_logic;
		sel_puerto_5_out : OUT std_logic;
		sel_puerto_6_out : OUT std_logic;
		sel_puerto_7_out : OUT std_logic
		);
	END COMPONENT;
	
-- Declaración del componente registro de 8 bits
	COMPONENT registro_8_bits
	PORT(
		d : IN std_logic_vector(7 downto 0);
		ce : IN std_logic;
		clk : IN std_logic;
		reset : IN std_logic;          
		q : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

-- Declaración del componente antirrebotes
	COMPONENT antirrebotes
	PORT(
		clk : IN std_logic;
		reset : IN std_logic;
		entrada : IN std_logic_vector(7 downto 0);          
		salida : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

-- Declaración del periférico PWM
	COMPONENT PWM
	PORT(
		lum_pot : IN std_logic_vector(7 downto 0);
		reset : IN std_logic;
		clk : IN std_logic;          
		lum_pulse : OUT std_logic
		);
	END COMPONENT;
	
-- Declaración del periférico complejo
	COMPONENT interfaz_sensor
	PORT(
		reset : IN std_logic;
		clk : IN std_logic;
		sdata : IN std_logic;
		leer : IN std_logic;
		medida_leida : IN std_logic;          
		sens_8 : OUT std_logic_vector(7 downto 0);
		cs : OUT std_logic;
		medida_disponible : OUT std_logic;
		sclk : OUT std_logic
		);
	END COMPONENT;

-- Declaración de señales internas

-- Picoblaze
	signal in_port : std_logic_vector(7 downto 0) := x"00";
	signal interrupt : std_logic := '0';
	signal port_id : std_logic_vector(7 downto 0) := x"00";
	signal write_strobe : std_logic := '0';
	signal out_port : std_logic_vector(7 downto 0) := x"00";
	signal read_strobe : std_logic := '0';
	signal interrupt_ack : std_logic := '0';

-- frequency divider	
	signal clk1kHz : 	std_logic := '0';							

-- Periféricos de entrada
	signal in_swiches : std_logic_vector(7 downto 0) :="00000000";
	signal reg_swiches : std_logic_vector(7 downto 0) :="00000000";

-- Periféricos de salida
	signal out_select_salidas : std_logic_vector(7 downto 0) := "00000000";
	signal en_LEDs	: std_logic := '0';
	signal en_LUT : std_logic;
	signal en_leer : std_logic;
	signal en_medida_leida : std_logic;

-- Componentes placa Nexys 2

-- Otras
	signal reset : std_logic := '0';
	signal bus_dir_lut : std_logic_vector (7 downto 0);
	signal sensor : std_logic_vector (7 downto 0);
	signal bus_leer : std_logic_vector (7 downto 0);
	signal bus_leida : std_logic_vector (7 downto 0);
	signal bus_salida_lut : std_logic_vector (7 downto 0);
	signal PWMin : std_logic_vector (7 downto 0);
	signal lum_pulse : std_logic;
	signal medida_disponible : std_logic_vector (7 downto 0);
	signal btn_0 : std_logic_vector (7 downto 0);

begin

-- Componentes de la placa Nexys 2

-- El pulsador BTN3 es el reset global
reset <= BTN(3);


-- Instanciación del divisor de reloj
	Inst_frequency_divider: frequency_divider PORT MAP(
		clk50MHz => clk,
		reset => reset,
		clk2MHz => open,
		clk1MHz => open,
		clk1kHz => clk1kHz
	);


-- Instanciación del componente Picoblaze 3 empotrado
	Inst_picoblaze3_empotrado: picoblaze3_empotrado PORT MAP(
		port_id => port_id,
		write_strobe => write_strobe,
		out_port => out_port,
		read_strobe => read_strobe,
		in_port => in_port,
		interrupt => interrupt,
		interrupt_ack => interrupt_ack,
		reset => reset,
		clk => clk
	);

-- Instanciación del componente LUT
	Inst_consulta_luz: consulta_luz PORT MAP(
		dir_lut => bus_dir_lut,
		luz_id => bus_salida_lut
	);
	
-- Instanciación del PWM
	Inst_PWM: PWM PORT MAP(
		lum_pot => PWMin,
		reset => reset,
		clk => clk,
		lum_pulse => lum_pulse
	);
	
	LEDs <= lum_pulse&lum_pulse&lum_pulse&lum_pulse&lum_pulse&lum_pulse&lum_pulse&lum_pulse;

-- Circuito antirrebotes para los interruptores
--	Inst_antirrebotes_swiches: antirrebotes PORT MAP(
--		clk => clk1kHz,
--		reset => reset,
--		entrada => SW,
--		salida => in_swiches
--	);

-- Instanciación del componente de selección binaria de 8 periféricos de entrada con registro
	btn_0 <= "0000000"&BTN(0);

	Inst_seleccion_binaria_8_perifericos_entrada_con_registro: seleccion_binaria_8_perifericos_entrada_con_registro PORT MAP(
		puerto_00_in => "00000000",
		puerto_01_in => sensor,
		puerto_02_in => PWMin,
		puerto_03_in => bus_salida_lut,
		puerto_04_in => reg_swiches,
		puerto_05_in => btn_0,
		puerto_06_in => medida_disponible,
		puerto_07_in => "00000000",
		port_id => port_id,
		clk_micro => clk,
		reset => reset,
		in_port => in_port
	);


-- Instanciación del componente de selección binaria de 8 periféricos de salida con registro
	Inst_seleccion_binaria_8_perifericos_salida_con_registro: seleccion_binaria_8_perifericos_salida_con_registro PORT MAP(
		out_port => out_port,
		port_id => port_id,
		clk_micro => clk,
		reset => reset,
		write_strobe => write_strobe,
		out_port_reg => out_select_salidas,
		sel_puerto_0_out => open,
		sel_puerto_1_out => en_LEDs,
		sel_puerto_2_out => en_LUT,
		sel_puerto_3_out => en_leer,
		sel_puerto_4_out => en_medida_leida,
		sel_puerto_5_out => open,
		sel_puerto_6_out => open,
		sel_puerto_7_out => open
	);

-- Periférico 1 de entrada. Interruptores
-- Instanciación del componente registro de 8 bits
	Inst_registro_8_bits_swiches: registro_8_bits PORT MAP(
		d => SW,
		ce => '1',
		clk => clk,
		reset => reset,
		q => reg_swiches
	);


-- ************************
-- Periféricos de salida
-- ************************
-- Periférico 0 de salida. Registro de PWM
-- Instanciación del componente registro de 8 bits
	Inst_registro_8_bits_PWM: registro_8_bits PORT MAP(
		d => out_select_salidas,
		ce => en_LEDs,
		clk => clk,
		reset => reset,
		q => PWMin
	);

	Inst_registro_8_bits_LUT: registro_8_bits PORT MAP(
		d => out_select_salidas,
		ce => en_LUT,
		clk => clk,
		reset => reset,
		q => bus_dir_lut
	);
	
	
	--Registro 8 bits de leer y de medida leida
	Inst_registro_8_bits_LEER: registro_8_bits PORT MAP(
		d => out_select_salidas,
		ce => en_leer,
		clk => clk,
		reset => reset,
		q => bus_leer
	);
	
		Inst_registro_8_bits_LEIDA: registro_8_bits PORT MAP(
		d => out_select_salidas,
		ce => en_medida_leida,
		clk => clk,
		reset => reset,
		q => bus_leida
	);
	--***********************
	-- PERIFÉRICO COMPLEJO
	--*********************
		Inst_interfaz_sensor: interfaz_sensor PORT MAP(
		reset => reset,
		clk => clk,
		sdata => sdata,
		leer => bus_leer(0),
		medida_leida => bus_leida(0),
		sens_8 => sensor,
		cs => cs,
		medida_disponible => medida_disponible(0),
		sclk => sclk
	);
	
	
end Behavioral;

