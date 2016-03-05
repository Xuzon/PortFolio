----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:03:38 11/17/2015 
-- Design Name: 
-- Module Name:    interfaz_sensor - Behavioral 
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
use IEEE.NUMERIC_STD.ALL;

--el reloj slck maximo 4 MHz (250 ns) minimo 1 MHz (1000 ns) es decir contar 14 clk_periods (7 en bajada 7 en subida)
--primero hay que bajar cs y tras minimo 10 ns flanco ascendente sclk

entity interfaz_sensor is
    Port ( reset : in  STD_LOGIC;
           clk : in  STD_LOGIC;
           sdata : in  STD_LOGIC;
			  leer : in std_logic;
			  medida_leida : in std_logic;
           sens_8 : out  STD_LOGIC_VECTOR (7 downto 0);
           cs : out  STD_LOGIC;
			  medida_disponible : out STD_LOGIC;
           sclk : out  STD_LOGIC
	  );
end interfaz_sensor;


architecture Behavioral of interfaz_sensor is
	signal cont : integer range 0 to 7;
	signal cont2 : integer range 0 to 8;
	signal divisor_tiempo : integer range 0 to 7;
	signal segundaEspera : std_logic:= '0';
	signal reg : std_logic_vector (7 downto 0);
	signal disponible : std_logic := '0';
	signal ejecucion : std_logic := '0';
	signal cs_interno : std_logic := '1';
	signal sclk_interno : std_logic := '1';
begin
	cs <= cs_interno;
	medida_disponible <= disponible;
	sclk <= sclk_interno;
	process(clk,reset,sdata,medida_leida)
	begin
		if(reset = '1') then
			sens_8 <= "00000000";
			cs_interno <= '1';
			sclk_interno <= '1';
		elsif(clk'event and clk ='1') then
				if(cs_interno <= '0') then
					if(medida_leida = '1' and disponible = '1') then--si me mandaron que leyeron la medida
						disponible <= '0';--digo que no hay medida disponible
					end if;
					divisor_tiempo <= divisor_tiempo + 1;--cuento uno más al divisor de tiempo
					if (divisor_tiempo = 13) then--cuando llegue a 14 pulsos
						divisor_tiempo <= 0;--vuelve a recontar
					end if;
					if(divisor_tiempo < 7 and ejecucion = '1' and disponible = '0') then --si está en la parte baja de sclk
						sclk_interno <= '0';	--lo mantenemos
					else
						sclk_interno <= '1';
					end if;
					if(divisor_tiempo = 7) then--cuando llegue a un flanco ascendente del sclk
						if(ejecucion= '1' and disponible = '0') then --si está activado 
							if(segundaEspera = '0') then --primero tres 0 y datos
								if(cont = 2) then --si ya leimos los tres primeros 0
									if(cont2 = 8) then --si ya leimos los 8 bits de datos
										sens_8 <= reg;--lo sacamos
										cont <= 0;
										segundaEspera <= '1';--cambiamos de estado				
									else --leemos bit de dato
										reg(cont2) <= sdata;
										cont2 <= cont2 + 1;
									end if;
								else --si no
									cont <= cont + 1;
								end if;
							else --zeros de cola
								if(cont = 5) then--si ya leimos todos los 0
									disponible <= '1';
									ejecucion <= '0';
									cont <= 0; --reinicializo
									cont2 <= 0;
									segundaEspera <= '0';
									cs_interno <= '1';
								else --si no
									cont <= cont + 1;--cuento
								end if;
							end if;
						end if;
					end if;
				end if;
				if(ejecucion = '0' and leer = '1')then --si no estoy ejecutando y me mandan leer = 1
					ejecucion <= '1';--empiezo a ejecutar
					cs_interno <= '0';
				end if;
			end if;
	end process;
end Behavioral;

