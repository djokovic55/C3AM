-- Dual-Port Block RAM with Two Write Ports
-- Correct Modelization with a Shared Variable
-- File: rams_tdp_rf_rf.vhd
library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;
entity rams_tdp_rf_rf is
	generic(
		DATA_WIDTH: integer := 16;
		ADDR_WIDTH: integer := 12
	);
	 port(
		 clka : in std_logic;
		 clkb : in std_logic;
		 ena : in std_logic;
		 enb : in std_logic;
		 wea : in std_logic;
		 web : in std_logic;
		 addra : in std_logic_vector(ADDR_WIDTH-1 downto 0);
		 addrb : in std_logic_vector(ADDR_WIDTH-1 downto 0);
		 dia : in std_logic_vector(DATA_WIDTH-1 downto 0);
		 dib : in std_logic_vector(DATA_WIDTH-1 downto 0);
		 doa : out std_logic_vector(DATA_WIDTH-1 downto 0);
		 dob : out std_logic_vector(DATA_WIDTH-1 downto 0)
	 );
end rams_tdp_rf_rf;

architecture syn of rams_tdp_rf_rf is

	type ram_type is array (2023 downto 0) of std_logic_vector(DATA_WIDTH-1 downto 0);
	signal RAM : ram_type;

	attribute ram_style:string;
	attribute ram_style of RAM: signal is "block";
begin

 process(clka, clkb)
 begin
	if clka'event and clka = '1' then
		if ena = '1' then
			doa <= RAM(to_integer(unsigned(addra)));
			if wea = '1' then
				RAM(to_integer(unsigned(addra))) <= dia;
			end if;
		end if;
	 end if;

	 if clkb'event and clkb = '1' then
		 if enb = '1' then
			dob <= RAM(to_integer(unsigned(addrb)));
			if web = '1' then
				RAM(to_integer(unsigned(addrb))) <= dib;
			end if;
		 end if;
	 end if;
 end process;

end syn;