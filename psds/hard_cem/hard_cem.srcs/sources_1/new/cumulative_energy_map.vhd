----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 08/15/2022 11:23:04 AM
-- Design Name: 
-- Module Name: cumulative_energy_map - Behavioral
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity cumulative_energy_map is
generic( 
          DATA_WIDTH: integer := 16;
          ADDR_WIDTH: integer := 12);
    
 port(
----------Interfejs za klok i reset signale----------
     clk:             in std_logic;
     reset:           in std_logic;
     
----------Interfejs memorije za citanje----------
     rd_addr_o:       out std_logic_vector(ADDR_WIDTH-1 downto 0);
     rd_data_i:       in std_logic_vector(DATA_WIDTH-1 downto 0);
     
----------Interfejs memorije za upisivanje--------
     wr_addr_o:       out std_logic_vector(ADDR_WIDTH-1 downto 0);
     wr_data_o:       out std_logic_vector(DATA_WIDTH-1 downto 0);
     wr_o:            out std_logic; 
     
----------Interfejs za prosljedjivanje broja kolona----------
     colsize:         in unsigned(ADDR_WIDTH-1 downto 0);
     
----------Komandni interfejs----------
     start:           in std_logic;
     hard_toggle_row: in std_logic;
     
----------Statusni interfejs----------
     ready:           out std_logic);
end cumulative_energy_map;

architecture Behavioral of cumulative_energy_map is

     type state_type is (idle, L1, L2, L3, L4, L5);

     signal state_reg, state_next: state_type;
     signal a_reg, a_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal b_reg, b_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal c_reg, c_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal min_reg, min_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal col_reg, col_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
     signal target_pixel_addr_reg, target_pixel_addr_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
     signal abc_addr_reg, abc_addr_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
     signal min_abc: std_logic_vector(DATA_WIDTH-1 downto 0);
     
     component comparator
     port(
          a_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
          b_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
          c_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
      
          minimum: out std_logic_vector(DATA_WIDTH-1 downto 0)
          ); 
    end component;

begin
komparator1: comparator port map(a_reg   => a_reg,
                                    b_reg   => b_reg,
                                    c_reg   => c_reg,
                                    minimum => min_abc);
process(clk, reset)
begin


  if reset = '1' then
     state_reg <= idle; 
     a_reg <= (others => '0');
     b_reg <= (others => '0');
     c_reg <= (others => '0');
     min_reg <= (others => '0');
     col_reg <= (others => '0');
     target_pixel_addr_reg <= (others => '0');
     abc_addr_reg <= (others => '0');
   
  elsif (clk'event and clk = '1') then

     state_reg <= state_next;
     a_reg <= a_next;
     b_reg <= b_next;
     c_reg <= c_next;
     min_reg <= min_next;
     col_reg <= col_next;
     target_pixel_addr_reg <= target_pixel_addr_next;
     abc_addr_reg <= abc_addr_next;

  end if;
 end process;
 
 process(a_reg, a_next, b_reg, b_next, c_reg, c_next, min_reg, min_next, 
         col_next, col_reg, target_pixel_addr_reg, target_pixel_addr_next, abc_addr_next, abc_addr_reg, start, state_reg, rd_data_i)
 begin
     
  -- Difoltne vrijednosti
     -- state_next <= state_reg;

     a_next <= a_reg;
     b_next <= b_reg;
     c_next <= c_reg;
     min_next <= min_reg;

     col_next <= col_reg;
     target_pixel_addr_next <= target_pixel_addr_reg;
     abc_addr_next <= abc_addr_reg;

     rd_addr_o <= (others => '0');
     wr_data_o <= (others => '0');
     wr_addr_o <= (others => '0');

     ready <= '0';
     wr_o <= '0';
  
  case state_reg is
   when idle => 
   
     ready <= '1';

     if start = '1' then
          col_next <= (others => '0');
          rd_addr_o <= abc_addr_next;

          if hard_toggle_row = '1' then
               abc_addr_next <= col_next;
               target_pixel_addr_next <= std_logic_vector(unsigned(col_next) + unsigned(colsize));

          else
               abc_addr_next <= std_logic_vector(unsigned(col_next) + unsigned(colsize));
               target_pixel_addr_next <= col_next;

          end if;
          state_next <= L1;
     else
          state_next <= idle;
    end if; 
    
   when L1 => 
   
     b_next <= rd_data_i;

     abc_addr_next <= std_logic_vector(unsigned(abc_addr_reg) + 1);
     rd_addr_o <= std_logic_vector(unsigned(abc_addr_next));

     state_next <= L2;
    
   when L2 =>

     c_next <= rd_data_i;
     a_next <= rd_data_i;

     rd_addr_o <= target_pixel_addr_reg;

     state_next <= L3;
   when L3 =>
     -- bilo u prethodnom stanju
     min_next <= min_abc;

     wr_addr_o <= target_pixel_addr_reg;
     wr_o <= '1';
     -- promena min_reg u min_next
     wr_data_o <= std_logic_vector(unsigned(rd_data_i) + unsigned(min_next));
     col_next <= std_logic_vector(unsigned(col_reg) + 1);

     target_pixel_addr_next <= std_logic_vector(unsigned(target_pixel_addr_reg) + 1); 

     -- bilo u L4
     a_next <= b_reg;
     b_next <= c_reg;
     -- promena na next vrednost 
     abc_addr_next <= std_logic_vector(unsigned(abc_addr_reg) + 1);
     rd_addr_o <= std_logic_vector(unsigned(abc_addr_next));

     state_next <= L4;
   when L4 => 

     c_next <= rd_data_i;

     rd_addr_o <= target_pixel_addr_reg;
     wr_o <= '0';
     
     
     state_next <= L5;

   when L5 => 
     -- bilo u prethodnom stanju
     min_next <= min_abc;
     a_next <= b_reg;
     c_next <= b_reg;
     b_next <= c_reg;

     wr_addr_o <= target_pixel_addr_reg;
     wr_o <= '1';
     -- min_reg prelazi u min_next
     wr_data_o <= std_logic_vector(unsigned(rd_data_i) + unsigned(min_next));
     -- new pixel iteration
     col_next <= std_logic_vector(unsigned(col_reg) + 1);
     target_pixel_addr_next <= std_logic_vector(unsigned(target_pixel_addr_reg) + 1);
     abc_addr_next <= std_logic_vector(unsigned(abc_addr_reg) + 1);

    if (unsigned(col_next) < colsize) then
     rd_addr_o <= std_logic_vector(unsigned(abc_addr_next));

     state_next <= L4;
    else
     rd_addr_o <= target_pixel_addr_next;

     state_next <= idle;
    end if;
   
  end case;
 end process;

end Behavioral;
