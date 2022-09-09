----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 08/15/2022 05:45:55 PM
-- Design Name: 
-- Module Name: comparator - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
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
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity comparator is
generic (DATA_WIDTH: integer:= 16);
 port(       
      a_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
      b_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
      c_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
      
      minimum: out std_logic_vector(DATA_WIDTH-1 downto 0)
      );
end comparator;

architecture Behavioral of comparator is
signal a_or_c:   std_logic_vector(DATA_WIDTH-1 downto 0);
 signal sel_1, sel_2: std_logic;

begin

komparator1: 
 process(a_reg, c_reg)
 begin
  if (a_reg <= c_reg) then
   sel_1 <= '1';
  else
   sel_1 <= '0';
  end if;
 end process; 
 
 komparator2:
 process(a_or_c, b_reg)
 begin
  if (a_or_c <= b_reg) then
   sel_2 <= '1';
  else
   sel_2 <= '0';
  end if;
 end process; 
 
 mux1:
 process(a_reg, c_reg, sel_1)
 begin
  if (sel_1 = '1') then
   a_or_c <= a_reg;
  else
   a_or_c <= c_reg;
  end if;
 end process;
 
 mux2:
 process(b_reg, a_or_c, sel_2)
 begin
  if (sel_2 = '1') then
   minimum <= a_or_c;
  else
   minimum <= b_reg;
  end if;
 end process;

end Behavioral;
