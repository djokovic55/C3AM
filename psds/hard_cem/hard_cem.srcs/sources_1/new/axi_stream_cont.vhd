----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 08/25/2022 12:47:31 PM
-- Design Name: 
-- Module Name: axi_stream_cont - Behavioral
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

entity axi_stream_cont is
generic( 
          DATA_WIDTH: integer := 16;
          ADDR_WIDTH: integer := 12);
    Port ( 
           axis_clk : in STD_LOGIC;
           axis_reset : in STD_LOGIC;
           
           -- axi stream master interface
           m_axis_data : out std_logic_vector(DATA_WIDTH-1 downto 0);
           m_axis_valid : out std_logic;
           m_axis_ready : in std_logic;
           m_axis_last : out std_logic;

           -- axi stream slave interface
           s_axis_data : in std_logic_vector(DATA_WIDTH-1 downto 0);
           s_axis_valid : in std_logic;
           s_axis_ready : out std_logic;
           s_axis_last : in std_logic;

           -- from ip interface
           ready_ip : in std_logic;

           -- to memory interface 
           axis_mem_addr : out std_logic_vector(ADDR_WIDTH-1 downto 0);           
           axis_mem_data_o : out std_logic_vector(DATA_WIDTH-1 downto 0);           
           axis_mem_data_i : in std_logic_vector(DATA_WIDTH-1 downto 0);           
           axis_mem_we : out std_logic;
           axis_mem_en : out std_logic
           );
end axi_stream_cont;

architecture Behavioral of axi_stream_cont is
signal addr_counter_next, addr_counter_reg : std_logic_vector(DATA_WIDTH-1 downto 0);
begin

counter : process(axis_clk, axis_reset)
begin
    
    if axis_reset = '1' then
       addr_counter_reg <= (others => '0'); 
    elsif(axis_clk'event and axis_clk = '1') then
       addr_counter_next <= std_logic_vector(unsigned(addr_counter_reg) + 1);
    end if;
    
end process;
axis_master_if: process(axis_clk, axis_reset)
begin
    if axis_reset = '1' then
       m_axis_data <= (others => '0'); 
       m_axis_valid <= '0';
       m_axis_last <= '0';
    elsif(axis_clk'event and axis_clk = '1') then
        if(ready_ip = '1') then
        end if;
    end if;
end process; 
end Behavioral;
