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
     
----------Memory port 1----------
     addra_ip:       out std_logic_vector(ADDR_WIDTH-1 downto 0);
     dia_ip:       in std_logic_vector(DATA_WIDTH-1 downto 0);
     doa_ip:       out std_logic_vector(DATA_WIDTH-1 downto 0);
     wea_ip:            out std_logic; 
     ena_ip:            out std_logic; 
----------Memory port 2----------
     addrb_ip:       out std_logic_vector(ADDR_WIDTH-1 downto 0);
     dib_ip:       in std_logic_vector(DATA_WIDTH-1 downto 0);
     dob_ip:       out std_logic_vector(DATA_WIDTH-1 downto 0);
     web_ip:            out std_logic; 
     enb_ip:            out std_logic; 
     
----------Interfejs za prosljedjivanje broja kolona----------
     colsize:         in std_logic_vector(ADDR_WIDTH-1 downto 0);
     
----------Komandni interfejs----------
     start:           in std_logic;
     
----------Statusni interfejs----------
     hard_work:       out std_logic;
     ready:           out std_logic);
end cumulative_energy_map;

architecture Behavioral of cumulative_energy_map is

     type state_type is (idle, L0, L1, L2, L3, L4, L5, L6, L6a);

     signal state_reg, state_next: state_type;
     signal a_reg, a_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal b_reg, b_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal c_reg, c_next: std_logic_vector(DATA_WIDTH-1 downto 0);
     signal col_reg, col_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
     signal target_pixel_addr_reg, target_pixel_addr_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
     signal abc_addr_reg, abc_addr_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
     signal min_abc: std_logic_vector(DATA_WIDTH-1 downto 0);

     signal hard_toggle_row_next, hard_toggle_row_reg: std_logic;

     component comparator
     port(
          a_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
          b_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
          c_reg:   in std_logic_vector(DATA_WIDTH-1 downto 0);
      
          minimum: out std_logic_vector(DATA_WIDTH-1 downto 0)
          ); 
    end component;

begin
komparator1: comparator port map(a_reg   => a_next,
                                    b_reg   => b_next,
                                    c_reg   => c_next,
                                    minimum => min_abc);
process(clk, reset)
begin


  if reset = '1' then
     state_reg <= idle; 
     a_reg <= (others => '0');
     b_reg <= (others => '0');
     c_reg <= (others => '0');
     col_reg <= (others => '0');
     target_pixel_addr_reg <= (others => '0');
     abc_addr_reg <= (others => '0');

     hard_toggle_row_reg <= '1';
   
  elsif (clk'event and clk = '1') then

     state_reg <= state_next;
     a_reg <= a_next;
     b_reg <= b_next;
     c_reg <= c_next;
     col_reg <= col_next;
     target_pixel_addr_reg <= target_pixel_addr_next;
     abc_addr_reg <= abc_addr_next;

     hard_toggle_row_reg <= hard_toggle_row_next;

  end if;
 end process;
 

 process(a_reg, a_next, b_reg, b_next, c_reg, c_next, 
         col_next, col_reg, target_pixel_addr_reg, target_pixel_addr_next, abc_addr_next, abc_addr_reg, start, state_reg, dia_ip)
 begin
     
  -- Default values

     a_next <= a_reg;
     b_next <= b_reg;
     c_next <= c_reg;

     col_next <= col_reg;
     target_pixel_addr_next <= target_pixel_addr_reg;
     abc_addr_next <= abc_addr_reg;
     hard_toggle_row_next <= hard_toggle_row_reg;

     -- default memory port A
     doa_ip <= (others => '0');
     addra_ip <= (others => '0');
     wea_ip <= '0';
     ena_ip <= '1';
     -- -- default memory port B
     dob_ip <= (others => '0');
     addrb_ip <= (others => '0');
     web_ip <= '0';
     enb_ip <= '1';

     ready <= '0';
     hard_work <= '1';
  
  case state_reg is
   when idle =>
          
     ready <= '1';
     hard_work <= '0';

     if start = '1' then
          state_next <= L0;
     else
          state_next <= idle;
     end if;
   when L0 => 
   

     col_next <= (others => '0');
     addra_ip <= abc_addr_next;

     if hard_toggle_row_next = '1' then
          abc_addr_next <= std_logic_vector(to_unsigned(0, ADDR_WIDTH));
          target_pixel_addr_next <= colsize;

     else
          abc_addr_next <= colsize;
          target_pixel_addr_next <= std_logic_vector(to_unsigned(0, ADDR_WIDTH));

     end if;
     state_next <= L1;
    
   when L1 => 
   
     b_next <= dia_ip;

     abc_addr_next <= std_logic_vector(unsigned(abc_addr_reg) + 1);
     addra_ip <= std_logic_vector(unsigned(abc_addr_next));

     state_next <= L2;
    
   when L2 =>
     -- formiranje min_abc
     c_next <= dia_ip;
     a_next <= dia_ip;

     -- change
     addrb_ip <= target_pixel_addr_reg;
     state_next <= L3;
   
   when  L3 =>

     addrb_ip <= target_pixel_addr_reg;
     web_ip <= '1';
     dob_ip <= std_logic_vector(unsigned(dib_ip) + unsigned(min_abc));

     col_next <= std_logic_vector(unsigned(col_reg) + 1);
     target_pixel_addr_next <= std_logic_vector(unsigned(target_pixel_addr_reg) + 1); 
     abc_addr_next <= std_logic_vector(unsigned(abc_addr_reg) + 1);

     a_next <= b_reg;
     b_next <= c_reg;
     -- adresa za naredno c
     addra_ip <= std_logic_vector(unsigned(abc_addr_next));
     state_next <= L4;
     
   when L4 => 

     c_next <= dia_ip;
     
     --change
     addrb_ip <= target_pixel_addr_reg;
     state_next <= L5;

   when L5 => 

     a_next <= b_reg;
     b_next <= c_reg;

     web_ip <= '1';
     addrb_ip <= target_pixel_addr_reg;
     dob_ip <= std_logic_vector(unsigned(dib_ip) + unsigned(min_abc));

     -- new pixel iteration
     col_next <= std_logic_vector(unsigned(col_reg) + 1);
     target_pixel_addr_next <= std_logic_vector(unsigned(target_pixel_addr_reg) + 1);
     abc_addr_next <= std_logic_vector(unsigned(abc_addr_reg) + 1);

    if (unsigned(col_next) < unsigned(colsize) - 1) then

     -- adresa za naredno c
     addra_ip <= std_logic_vector(unsigned(abc_addr_next));

     state_next <= L4;
    else
     -- prelazi se u dgs, nema c
     state_next <= L6;
    end if;
   
   when L6 => 

     addrb_ip <= target_pixel_addr_reg;
     
     state_next <= L6a;

   when L6a => 

     web_ip <= '1';
     addrb_ip <= target_pixel_addr_reg;
     dob_ip <= std_logic_vector(unsigned(dib_ip) + unsigned(min_abc));
     
     hard_toggle_row_next <= not(hard_toggle_row_reg);

     state_next <= idle;
  end case;
 end process;

end Behavioral;
