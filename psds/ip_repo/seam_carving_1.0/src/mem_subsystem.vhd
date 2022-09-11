----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/08/2022 07:51:26 PM
-- Design Name: 
-- Module Name: mem_subsystem - Behavioral
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

entity mem_subsystem is
    generic( 
          DATA_WIDTH: integer := 16;
          ADDR_WIDTH: integer := 12);
    Port ( 
           clk : in STD_LOGIC;
           reset : in STD_LOGIC;

           -- Interface Axi Lite
           reg_data_i : in STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           col_wr_i : in STD_LOGIC;
           row_wr_i : in STD_LOGIC;
           cmd_wr_i : in STD_LOGIC;
           
           -- Iterface Axi Stream cont

           colsize_axis_o : out STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           rowsize_axis_o : out STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           start_axis_o : out STD_LOGIC;

           axis_done_i : in STD_LOGIC;
           status_axis_o : out STD_LOGIC;

           -- Memory Port A, axi stream
           axis_hard_work_i : in STD_LOGIC;

           axis_addr_i : in STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           axis_data_i : in STD_LOGIC_VECTOR (DATA_WIDTH-1 downto 0);
           axis_ena_i : in STD_LOGIC;
           axis_wea_i : in STD_LOGIC;

           -- Common data_o memory port A
           doa_o : out STD_LOGIC_VECTOR (DATA_WIDTH-1 downto 0);

           -- Interface Ip

           colsize_ip_o : out STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           -- Memory Port A, Ip
           addra_ip_i : in STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           doa_ip_i : in STD_LOGIC_VECTOR (DATA_WIDTH-1 downto 0);
           ena_ip_i : in STD_LOGIC;
           wea_ip_i : in STD_LOGIC;

           -- Memory Port B, Ip
           addrb_ip_i : in STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
           dob_ip_i : in STD_LOGIC_VECTOR (DATA_WIDTH-1 downto 0);
           enb_ip_i : in STD_LOGIC;
           web_ip_i : in STD_LOGIC;
           dib_ip_o : out STD_LOGIC_VECTOR (DATA_WIDTH-1 downto 0)
         );
end mem_subsystem;

architecture Behavioral of mem_subsystem is

component  rams_tdp_rf_rf is
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
end component;

signal colsize_s, rowsize_s: std_logic_vector(ADDR_WIDTH-1 downto 0);
signal cmd_s : std_logic;
signal axis_done_s : std_logic;

signal addra_mux : STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
signal dia_mux : STD_LOGIC_VECTOR (DATA_WIDTH-1 downto 0);
signal ena_mux : STD_LOGIC;
signal wea_mux : STD_LOGIC;

begin

      colsize_ip_o <= colsize_s;
      colsize_axis_o <= colsize_s;
      rowsize_axis_o <= rowsize_s;

      status_axis_o <= axis_done_s;
      start_axis_o <= cmd_s;

      -- registers
      cmd_reg:process(clk) 
      begin
            if(rising_edge(clk)) then
                  if(reset = '1') then
                        cmd_s <= '0';
                  elsif(cmd_wr_i = '1') then
                        cmd_s <= reg_data_i(0);
                  end if;
            end if;
      end process;

      colsize_reg:process(clk) 
      begin
            if(rising_edge(clk)) then
                  if(reset = '1') then
                        colsize_s <= (others => '0');
                  elsif(col_wr_i = '1') then
                        colsize_s <= reg_data_i;
                  end if;
            end if;
      end process;

      rowsize_reg:process(clk) 
      begin
            if(rising_edge(clk)) then
                  if(reset = '1') then
                        rowsize_s <= (others => '0');
                  elsif(row_wr_i = '1') then
                        rowsize_s <= reg_data_i;
                  end if;
            end if;
      end process;

      status_reg:process(clk) 
      begin
            if(rising_edge(clk)) then
                  if(reset = '1') then
                        axis_done_s <= '0';
                  else
                        axis_done_s <= axis_done_i;
                  end if;
            end if;
      end process;
      --------------------------------------------------------------------------------
      -- Memorija

    memory_port_A_muxes : process(axis_addr_i, axis_data_i, axis_wea_i, axis_ena_i,
                                  addra_ip_i, doa_ip_i, wea_ip_i, ena_ip_i,
                                  axis_hard_work_i)
    begin
        if (axis_hard_work_i = '1') then
            addra_mux <= addra_ip_i;
            dia_mux <= doa_ip_i;
            wea_mux <= wea_ip_i;
            ena_mux <= ena_ip_i; 
        else
            addra_mux <= axis_addr_i;
            dia_mux <= axis_data_i;
            wea_mux <= axis_wea_i;
            ena_mux <= axis_ena_i; 
        end if;
    end process;
      
    memorija: rams_tdp_rf_rf
        generic map(DATA_WIDTH => 16, ADDR_WIDTH => 12)
        port map(
                clka   => clk,
                clkb   => clk,
                ena   => ena_mux,
                enb   => enb_ip_i,
                wea   => wea_mux,
                web   => web_ip_i,
                addra => addra_mux,
                addrb => addrb_ip_i,
                dia   => dia_mux,
                dib   => dob_ip_i,
                -- doa ne zahteva nikakvo multipleksiranje
                doa   => doa_o, 
                dob   => dib_ip_o);
      

end Behavioral;
