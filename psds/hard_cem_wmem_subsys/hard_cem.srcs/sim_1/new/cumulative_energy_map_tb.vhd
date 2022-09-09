library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.std_logic_arith.ALL;
use IEEE.std_logic_unsigned.ALL;
use IEEE.NUMERIC_STD.ALL;

entity cumulative_energy_map_tb is
end entity;

architecture beh of cumulative_energy_map_tb is
    type mem_t is array (0 to 24) of std_logic_vector(15 downto 0);

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
                                
    signal clk_s:               std_logic;
    signal reset_s:             std_logic;
    
    signal colsize_s:           std_logic_vector(11 downto 0);
    -- port 1 
    signal addra_ip_s:         std_logic_vector(11 downto 0);
    signal dia_ip_s:         std_logic_vector(15 downto 0);
    signal doa_ip_s:         std_logic_vector(15 downto 0);
    signal wea_ip_s:               std_logic;
    signal ena_ip_s:               std_logic;

    -- signal ena_tb_s:               std_logic;
    -- port 2
    signal addrb_ip_s:         std_logic_vector(11 downto 0);
    signal dib_ip_s:         std_logic_vector(15 downto 0);
    signal dob_ip_s:         std_logic_vector(15 downto 0);
    signal web_ip_s:               std_logic;
    signal enb_ip_s:               std_logic;

    signal enb_tb_s:               std_logic;
    -- signali preko kojih tb puni memoriju
    signal addrb_tb:         std_logic_vector(11 downto 0);
    signal db_tb:         std_logic_vector(15 downto 0);
    signal web_tb:               std_logic;

    signal addrb_or:         std_logic_vector(11 downto 0);
    signal dib_or:         std_logic_vector(15 downto 0);
    signal web_or:               std_logic;
    -- signal ena_or:               std_logic;
    signal enb_or:               std_logic;

    
    signal start_s:             std_logic := '0';
    signal ready_s:             std_logic;

begin
    -- before memory's interface
    web_or <= web_tb or web_ip_s;
    dib_or <= db_tb or dob_ip_s;
    addrb_or <= addrb_tb or addrb_ip_s;
    -- ena_or <= ena_tb_s or ena_ip_s;
    enb_or <= enb_tb_s or enb_ip_s;

    clk_gen: process
    begin
        clk_s <= '0', '1' after 15 ns;
        wait for 30 ns;
    end process;
    
    stim_gen: process
    begin
        reset_s <= '1';
        colsize_s <= (std_logic_vector(to_unsigned(5, 12)));
        -- hard_toggle_row_s <= '1';

        enb_tb_s <= '0'; 
        wait for 500 ns;
        reset_s <= '0';

        enb_tb_s <= '1'; 
        wait until falling_edge(clk_s);
         
        web_tb <= '0';
        wait until falling_edge(clk_s);

        --punjenje memorije
        web_tb <= '1';
        wait until falling_edge(clk_s);
        for i in 0 to 4 loop
            for j in 0 to 4 loop
                addrb_tb <= std_logic_vector(to_unsigned(i*5 + j, 12));
                db_tb <= MEM_CONTENT(i*5 + j);
                wait until falling_edge(clk_s);
            end loop;
        end loop;
        web_tb <= '0';
        addrb_tb <= (others => '0');
        db_tb <= (others => '0');
        enb_tb_s <= '0';
        --pocetak rada algoritma
        for i in 0 to 2 loop
            
            wait until falling_edge(clk_s);
            start_s <= '1';
            wait until falling_edge(clk_s);
            -- wait until falling_edge(clk_s);
            -- wait until falling_edge(clk_s);
            start_s <= '0';
        
            wait until ready_s = '1';
        end loop;
        
        --zavrsetak stimulus generatora
        wait; 
    end process;
    
    memorija: rams_tdp_rf_rf
        generic map(DATA_WIDTH => 16, ADDR_WIDTH => 12)
        port map(
                clka   => clk_s,
                clkb   => clk_s,
                ena   => ena_ip_s,
                enb   => enb_or,
                wea   => wea_ip_s,
                web   => web_or,
                addra => addra_ip_s,
                addrb => addrb_or,
                dia   => doa_ip_s,
                dib   => dib_or,
                doa   => dia_ip_s, 
                dob   => dib_ip_s 
                );
        
    hard: cumulative_energy_map
        generic map(DATA_WIDTH => 16, ADDR_WIDTH => 12)
        port map(
                clk          => clk_s,
                reset        => reset_s,

                addra_ip     => addra_ip_s,
                dia_ip       => dia_ip_s,
                doa_ip       => doa_ip_s,    
                ena_ip       => ena_ip_s,    
                wea_ip       => wea_ip_s,   

                addrb_ip     => addrb_ip_s,
                dib_ip       => dib_ip_s, 
                dob_ip       => dob_ip_s,
                enb_ip       => enb_ip_s,    
                web_ip          => web_ip_s,

                colsize         => colsize_s,
                start           => start_s,
                ready           => ready_s
                );
end beh;
