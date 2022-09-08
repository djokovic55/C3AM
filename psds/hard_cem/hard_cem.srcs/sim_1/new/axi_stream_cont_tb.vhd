----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/01/2022 10:23:50 PM
-- Design Name: 
-- Module Name: axi_stream_cont_tb - Behavioral
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

entity axi_stream_cont_tb is
--  Port ( );
end axi_stream_cont_tb;

architecture Behavioral of axi_stream_cont_tb is

    type mem_t is array (0 to 24) of std_logic_vector(15 downto 0);

    component axi_stream_cont is
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
    end component;

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

    component cumulative_energy_map is
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
    end component;
    
    constant MEM_CONTENT: mem_t :=
                               (std_logic_vector(to_unsigned(1, 16)),
                                std_logic_vector(to_unsigned(2, 16)),
                                std_logic_vector(to_unsigned(0, 16)),
                                std_logic_vector(to_unsigned(4, 16)),
                                std_logic_vector(to_unsigned(3, 16)),
                                std_logic_vector(to_unsigned(1, 16)),
                                std_logic_vector(to_unsigned(7, 16)),
                                std_logic_vector(to_unsigned(8, 16)),
                                std_logic_vector(to_unsigned(9, 16)),
                                std_logic_vector(to_unsigned(10, 16)),
                                std_logic_vector(to_unsigned(11, 16)),
                                std_logic_vector(to_unsigned(12, 16)),
                                std_logic_vector(to_unsigned(13, 16)),
                                std_logic_vector(to_unsigned(14, 16)),
                                std_logic_vector(to_unsigned(15, 16)),
                                std_logic_vector(to_unsigned(16, 16)),
                                std_logic_vector(to_unsigned(17, 16)),
                                std_logic_vector(to_unsigned(18, 16)),
                                std_logic_vector(to_unsigned(19, 16)),
                                std_logic_vector(to_unsigned(20, 16)),
                                std_logic_vector(to_unsigned(21, 16)),
                                std_logic_vector(to_unsigned(22, 16)),
                                std_logic_vector(to_unsigned(23, 16)),
                                std_logic_vector(to_unsigned(24, 16)),
                                std_logic_vector(to_unsigned(25, 16)));
                                
--------------------------------------------------------------------------------
    -- Common signals

    signal clk_s:               std_logic;
    signal reset_s:             std_logic;
    
    signal colsize_s:           std_logic_vector(11 downto 0);
    signal rowsize_s:           std_logic_vector(11 downto 0);

--------------------------------------------------------------------------------
    -- IP cem, signals
    
    -- Memory port A
    signal addra_ip_s:         std_logic_vector(11 downto 0);
    -- signal dia_ip_s:           std_logic_vector(15 downto 0);
    signal doa_ip_s:           std_logic_vector(15 downto 0);
    signal wea_ip_s:           std_logic;
    signal ena_ip_s:           std_logic;

    -- Memory port B
    signal addrb_ip_s:         std_logic_vector(11 downto 0);
    signal dib_ip_s:           std_logic_vector(15 downto 0);
    signal dob_ip_s:           std_logic_vector(15 downto 0);
    signal web_ip_s:           std_logic;
    signal enb_ip_s:           std_logic;

    signal start_s:            std_logic := '0';
    signal ready_s:            std_logic;
    signal hard_work_s:        std_logic;
--------------------------------------------------------------------------------

    -- Axi stream cont interface signals

    signal axis_start_s :      std_logic;
    signal axis_done_s :       std_logic;

        -- axi stream master interface
    signal m_axis_data_s :     std_logic_vector(15 downto 0);
    signal m_axis_valid_s :    std_logic;
    signal m_axis_ready_s :    std_logic;
    signal m_axis_last_s :     std_logic;

        -- axi stream slave interface
    signal s_axis_data_s :     std_logic_vector(15 downto 0);
    signal s_axis_valid_s :    std_logic;
    signal s_axis_ready_s :    std_logic;
    signal s_axis_last_s :     std_logic;

        -- from ip interface
    signal hard_done_s :       std_logic;
    signal hard_start_s :      std_logic;

        -- to memory interface 
    signal axis_mem_addr_s :   std_logic_vector(11 downto 0);           
    -- signal axis_mem_data_i_s : std_logic_vector(15 downto 0);           
    signal axis_mem_data_o_s : std_logic_vector(15 downto 0);           
    signal axis_mem_we_s :     std_logic;
    signal axis_mem_en_s :     std_logic;

    -- Memory port A is being used by both ip and axis_cont, multplexing between AXIS_MEM and MEMORY PORT A signals
    signal doa_s:            std_logic_vector(15 downto 0);

    -- muxes output signals 
    signal addra_mux:          std_logic_vector(11 downto 0);
    signal dia_mux:            std_logic_vector(15 downto 0);
    signal wea_mux:            std_logic;
    signal ena_mux:            std_logic;

begin


    memory_port_A_muxes : process(axis_mem_addr_s, axis_mem_data_o_s, axis_mem_we_s, axis_mem_en_s,
                                  addra_ip_s, doa_ip_s, wea_ip_s, ena_ip_s,
                                  hard_start_s, hard_work_s)
    begin
        if (hard_work_s = '1') then
            addra_mux <= addra_ip_s;
            dia_mux <= doa_ip_s;
            -- bug, izlazni podaci iz memorije treba da se proslede direktno do kontrolera i IP-a, 
            -- tu ne treba nikakvo multipleksiranje, sto je i pogresno zapravo jer bi tu bio potreban demux
            -- samo ulazni signali u memoriju treba da se multipleksiraju, jer imamo odovojene procese unutar ip-a i kont koji ih drajvuju
            -- doa_mux <= dia_ip_s;
            wea_mux <= wea_ip_s;
            ena_mux <= ena_ip_s; 
        else
            addra_mux <= axis_mem_addr_s;
            dia_mux <= axis_mem_data_o_s;
            -- doa_mux <= axis_mem_data_i_s;
            wea_mux <= axis_mem_we_s;
            ena_mux <= axis_mem_en_s; 
        end if;
    end process;

    clk_gen: process
    begin
        clk_s <= '0', '1' after 15 ns;
        wait for 30 ns;
    end process;
    
    driver: process
    begin
        reset_s <= '1';
        colsize_s <= (std_logic_vector(to_unsigned(5, 12)));
        rowsize_s <= (std_logic_vector(to_unsigned(4, 12)));

        wait for 500 ns;
        reset_s <= '0';
        wait until falling_edge(clk_s);
        -- initiate cont
        axis_start_s <= '1';

        -- Dma slave port should be always ready
        m_axis_ready_s <= '1';

        -- sending data from dma simulation 
        wait until falling_edge(clk_s);
        s_axis_valid_s <= '1';

        wait until (axis_done_s = '1');
        axis_start_s <= '0';

        --zavrsetak stimulus generatora
        wait; 
    end process;

    dma : process(clk_s)
    variable index : integer := 0;
    begin
        if(rising_edge(clk_s)) then
            if(s_axis_ready_s = '1' and index < 25) then
                s_axis_data_s <= MEM_CONTENT(index);
                index := index + 1;
            else
                null;
            end if;

        end if;
    end process;

    memorija: rams_tdp_rf_rf
        generic map(DATA_WIDTH => 16, ADDR_WIDTH => 12)
        port map(
                clka   => clk_s,
                clkb   => clk_s,
                ena   => ena_mux,
                enb   => enb_ip_s,
                wea   => wea_mux,
                web   => web_ip_s,
                -- addra ne zahteva nikakvo multipleksiranje
                addra => addra_mux,
                addrb => addrb_ip_s,
                dia   => dia_mux,
                dib   => dob_ip_s,
                doa   => doa_s, 
                dob   => dib_ip_s);
        
    hard: cumulative_energy_map
        generic map(DATA_WIDTH => 16, ADDR_WIDTH => 12)
        port map(
                clk          => clk_s,
                reset        => reset_s,

                addra_ip     => addra_ip_s,
                dia_ip       => doa_s,
                doa_ip       => doa_ip_s,    
                ena_ip       => ena_ip_s,    
                wea_ip       => wea_ip_s,   

                addrb_ip     => addrb_ip_s,
                dib_ip       => dib_ip_s, 
                dob_ip       => dob_ip_s,
                enb_ip       => enb_ip_s,    
                web_ip          => web_ip_s,

                colsize         => colsize_s,
                start           => hard_start_s,
                -- new signal
                hard_work       => hard_work_s,
                ready           => hard_done_s);

    axi_stream_cont_inst: axi_stream_cont
      generic map (
        DATA_WIDTH => 16,
        ADDR_WIDTH => 12
      )
      port map (
        axis_clk        => clk_s,
        axis_reset      => reset_s,
        axis_start      => axis_start_s,
        axis_done      => axis_done_s,
        colsize         => colsize_s,
        rowsize         => rowsize_s,
        m_axis_data     => m_axis_data_s,
        m_axis_valid    => m_axis_valid_s,
        m_axis_ready    => m_axis_ready_s,
        m_axis_last     => m_axis_last_s,
        s_axis_data     => s_axis_data_s,
        s_axis_valid    => s_axis_valid_s,
        s_axis_ready    => s_axis_ready_s,
        s_axis_last     => s_axis_last_s,
        hard_done       => hard_done_s,
        hard_start      => hard_start_s,
        axis_mem_addr   => axis_mem_addr_s,
        axis_mem_data_o => axis_mem_data_o_s,
        axis_mem_data_i => doa_s,
        axis_mem_we     => axis_mem_we_s,
        axis_mem_en     => axis_mem_en_s);
end Behavioral;
