library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.NUMERIC_STD.ALL;

entity memory is
	port(
		clk : in std_logic;
		-- ena : in std_logic;
		-- enb : in std_logic;
		wea : in std_logic;
		addra : in std_logic_vector(11 downto 0);
		addrb : in std_logic_vector(11 downto 0);
		dia : in std_logic_vector(15 downto 0);
		dob : out std_logic_vector(15 downto 0)
		);
end entity;

architecture syn of memory is
	type ram_type is array (24 downto 0) of std_logic_vector(15 downto 0);
	signal RAM : ram_type;
begin

 process(clk)
	begin

	if clk'event and clk = '1' then
		if wea = '1' then
				RAM(to_integer(unsigned(addra))) <= dia;
		end if;

	dob <= RAM(to_integer(unsigned(addrb)));
	end if;
 end process;
 
--  process(clk)
--  begin
-- 	if clk'event and clk = '1' then
-- 		dob <= RAM(conv_integer(addrb));
-- 	  --  if enb = '1' then
-- 	  --      dob <= RAM(conv_integer(addrb));
-- 	  --  end if;
-- 	end if;
--  end process;
end syn;