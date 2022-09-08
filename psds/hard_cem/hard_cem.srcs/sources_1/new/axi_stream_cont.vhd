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

           axis_start : in std_logic;
           axis_done : out std_logic;


           colsize : in std_logic_vector(ADDR_WIDTH-1 downto 0);
           rowsize : in std_logic_vector(ADDR_WIDTH-1 downto 0);
           
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
           hard_done : in std_logic;
           hard_start : out std_logic;

           -- to memory interface 
           axis_mem_addr : out std_logic_vector(ADDR_WIDTH-1 downto 0);           
           axis_mem_data_o : out std_logic_vector(DATA_WIDTH-1 downto 0);           
           axis_mem_data_i : in std_logic_vector(DATA_WIDTH-1 downto 0);           
           axis_mem_we : out std_logic;
           axis_mem_en : out std_logic
           );
end axi_stream_cont;

architecture Behavioral of axi_stream_cont is

    type state_type is (idle, start_first_row, first_row, new_row, sh_t, hard_work, hs_t_addr, hs_t);

    signal state_reg, state_next: state_type;
    signal col_reg, col_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
    signal row_reg, row_next: std_logic_vector(ADDR_WIDTH-1 downto 0);
    signal data_reg, data_next: std_logic_vector(DATA_WIDTH-1 downto 0);

    signal m_data_reg, m_data_next: std_logic_vector(DATA_WIDTH-1 downto 0);

    signal axis_toggle_row_reg, axis_toggle_row_next: std_logic;

    signal comb_addr: std_logic_vector(ADDR_WIDTH-1 downto 0);
    signal comb_addr_hs: std_logic_vector(ADDR_WIDTH-1 downto 0);
begin

registers: process(axis_clk, axis_reset)
begin
    if axis_reset = '1' then
        state_reg <= idle;
        col_reg <= (others => '0');
        row_reg <= (others => '0');
        axis_toggle_row_reg <= '0';
        data_reg <= (others => '0');
        m_data_reg <= (others => '0');

    elsif(axis_clk'event and axis_clk = '1') then

        state_reg <= state_next;
        col_reg <= col_next;
        row_reg <= row_next;
        axis_toggle_row_reg <= axis_toggle_row_next;
        data_reg <= data_next;
        m_data_reg <= m_data_next;
    end if;
end process; 

address_gen: process(col_reg, axis_toggle_row_reg)
begin
    if (axis_toggle_row_reg = '0') then
        comb_addr <= col_reg;
        comb_addr_hs <= std_logic_vector(unsigned(col_reg) + unsigned(colsize));
    else
        comb_addr <= std_logic_vector(unsigned(col_reg) + unsigned(colsize));
        comb_addr_hs <= col_reg;
    end if;
end process;

combination: process(state_reg, state_next, col_next, col_reg, row_reg, row_next, axis_toggle_row_next, axis_toggle_row_reg,
                     axis_start, hard_done, s_axis_data, s_axis_valid, m_axis_ready)
begin

    -- Status interface

    axis_done <= '0';

    -- Default register values    

    col_next <= col_reg;
    row_next <= row_reg;
    axis_toggle_row_next <= axis_toggle_row_reg;
    data_next <= data_reg;
    m_data_next <= m_data_reg;

    -- Default axis slave
    s_axis_ready <= '0';

    -- Default axis master
    m_axis_data <= (others => '0');
    m_axis_last <= '0';
    m_axis_valid <= '0';

    -- Default hard control
    hard_start <= '0';

    -- Default memory port
    axis_mem_addr <= comb_addr;
    axis_mem_data_o <= (others => '0');
    axis_mem_we <= '0';
    axis_mem_en <= '1'; 

    case state_reg is
        when idle =>

            if (axis_start = '1') then
                row_next <= (others => '0');
                state_next <= start_first_row;
                axis_done <= '0';
            else
                axis_done <= '1';
                state_next <= idle;
            end if;

        when start_first_row =>
            
            -- get new data
            col_next <= (others => '0');
            s_axis_ready <= '1';

            if(s_axis_valid = '1') then
                state_next <= first_row;
            else
                state_next <= start_first_row;
            end if;

        when first_row =>
            
            axis_mem_we <= '1';
            axis_mem_addr <= comb_addr;
            data_next <= s_axis_data;
            axis_mem_data_o <= data_next;

            s_axis_ready <= '1';

            if(s_axis_valid = '1') then
                col_next <= std_logic_vector(unsigned(col_reg) + 1);

                -- potential bug, col next value has not been assigned
                if(unsigned(col_reg) = unsigned(colsize) - 1) then
                    -- switch row
                    s_axis_ready <= '0';
                    axis_toggle_row_next <= not(axis_toggle_row_reg);
                    state_next <= new_row;
                else
                    state_next <= first_row;
                end if;
            else
                state_next <= first_row;
            end if;

        when new_row =>
            
            -- get new data
            col_next <= (others => '0');
            s_axis_ready <= '1';

            if(s_axis_valid = '1') then
                state_next <= sh_t;
            else
                state_next <= new_row;
            end if;

        when sh_t =>
            
            axis_mem_we <= '1';
            axis_mem_addr <= comb_addr;
            data_next <= s_axis_data;
            axis_mem_data_o <= data_next;

            s_axis_ready <= '1';

            if(s_axis_valid = '1') then
                col_next <= std_logic_vector(unsigned(col_reg) + 1);

                -- potential bug, col next value has not been assigned
                -- delay 1 cycle, col_reg is used instead of col_next
                if(unsigned(col_reg) = unsigned(colsize) - 1) then
                    -- switch row

                    axis_toggle_row_next <= not(axis_toggle_row_reg);

                    hard_start <= '1';
                    s_axis_ready <= '0';
                    state_next <= hard_work;
                else
                    state_next <= sh_t;
                end if;
            else
                state_next <= sh_t;
            end if;
            
            
        when hard_work =>
                
            if(hard_done = '1') then
                col_next <= (others => '0');
                state_next <= hs_t_addr;
            else
                state_next <= hard_work;
            end if;
        when hs_t_addr =>

            -- m_axis_data <= m_data_reg;
            axis_mem_addr <= comb_addr_hs;

            if(m_axis_ready = '1') then
                state_next <= hs_t;
            else
                state_next <= hs_t_addr;
            end if;

        when hs_t =>
            
            m_axis_valid <= '1';
            m_data_next <= axis_mem_data_i;
            m_axis_data <= m_data_next;
            col_next <= std_logic_vector(unsigned(col_reg) + 1);

            -- potential bug, col next value has not been assigned
            if(unsigned(col_next) = unsigned(colsize)) then

                row_next <= std_logic_vector(unsigned(row_reg) + 1);
                -- axis_toggle_row_next <= not(axis_toggle_row_reg);
                
                -- s_axis_last should be checked here
                if(unsigned(row_next) = (unsigned(rowsize) - 1)) then
                    axis_done <= '1';
                    m_axis_last <= '1';

                    state_next <= idle;
                else
                    -- s_axis_ready <= '1';
                    state_next <= new_row;

                end if;
            else
                state_next <= hs_t_addr;
            end if;

    end case;

end process;
end Behavioral;
