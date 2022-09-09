----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/08/2022 09:47:57 PM
-- Design Name: 
-- Module Name: mem_subsystem_tb - Behavioral
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

entity mem_subsystem_tb is
--  Port ( );
end mem_subsystem_tb;

architecture Behavioral of mem_subsystem_tb is

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

    component mem_subsystem is
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

    -- Memory port A is being used by both ip and axis_cont, multplexing between AXIS_MEM and IP MEMORY PORT A signals
    signal doa_s:            std_logic_vector(15 downto 0);
--------------------------------------------------------------------------------

    -- Axi lite cont interface signals
    -- need to be stimulated in this TB
    signal reg_data_i_s:     std_logic_vector(11 downto 0);
    signal col_wr_i_s:       std_logic;
    signal row_wr_i_s:       std_logic;
    signal cmd_wr_i_s:       std_logic;

    -- Status signals from axis to axil
    
    signal status_axis_s : STD_LOGIC;

begin


    clk_gen: process
    begin
        clk_s <= '0', '1' after 15 ns;
        wait for 30 ns;
    end process;
    
    driver: process
    begin
        reset_s <= '1';

        wait for 500 ns;
        reset_s <= '0';
        wait until falling_edge(clk_s);

        -- -- set cmd reg, initiate cont
        -- cmd_wr_i_s <= '1';
        -- reg_data_i_s <= (std_logic_vector(to_unsigned(1, 12)));
        -- wait until falling_edge(clk_s);
        -- cmd_wr_i_s <= '0';

        -- set colsize reg
        col_wr_i_s <= '1';
        reg_data_i_s <= (std_logic_vector(to_unsigned(5, 12)));
        wait until falling_edge(clk_s);
        col_wr_i_s <= '0';

        -- set rowsize reg
        row_wr_i_s <= '1';
        reg_data_i_s <= (std_logic_vector(to_unsigned(4, 12)));
        wait until falling_edge(clk_s);
        row_wr_i_s <= '0';

        -- set cmd reg, initiate cont
        cmd_wr_i_s <= '1';
        reg_data_i_s <= (std_logic_vector(to_unsigned(1, 12)));
        wait until falling_edge(clk_s);
        cmd_wr_i_s <= '0';


        -- Dma slave port should be always ready
        m_axis_ready_s <= '1';

        -- sending data from dma simulation 
        wait until falling_edge(clk_s);
        s_axis_valid_s <= '1';

        wait until (axis_done_s = '1');

        cmd_wr_i_s <= '1';
        reg_data_i_s <= (std_logic_vector(to_unsigned(0, 12)));
        wait until falling_edge(clk_s);
        wait until falling_edge(clk_s);
        cmd_wr_i_s <= '0';

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

    -- mem_subsys: mem_subsystem
    
    mem_subsystem_inst: mem_subsystem
      generic map (
        DATA_WIDTH => 16,
        ADDR_WIDTH => 12
      )
      port map (
        clk              => clk_s,
        reset            => reset_s,
        reg_data_i       => reg_data_i_s,
        col_wr_i         => col_wr_i_s,
        row_wr_i         => row_wr_i_s,
        cmd_wr_i         => cmd_wr_i_s,
        colsize_axis_o   => colsize_s,
        rowsize_axis_o   => rowsize_s,
        start_axis_o     => axis_start_s,
        axis_done_i      => axis_done_s, 
        status_axis_o    => status_axis_s, 
        axis_hard_work_i => hard_work_s,
        axis_addr_i      => axis_mem_addr_s,
        axis_data_i      => axis_mem_data_o_s,
        axis_ena_i       => axis_mem_en_s,
        axis_wea_i       => axis_mem_we_s,
        doa_o            => doa_s,
        colsize_ip_o     => colsize_s,
        addra_ip_i       => addra_ip_s,
        doa_ip_i         => doa_ip_s,
        ena_ip_i         => ena_ip_s,
        wea_ip_i         => wea_ip_s,
        addrb_ip_i       => addrb_ip_s,
        dob_ip_i         => dob_ip_s,
        enb_ip_i         => enb_ip_s,
        web_ip_i         => web_ip_s,
        dib_ip_o         => dib_ip_s
      );
    
end Behavioral;
