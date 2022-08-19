library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.std_logic_arith.ALL;
use IEEE.std_logic_unsigned.ALL;
use IEEE.NUMERIC_STD.ALL;

entity cumulative_energy_map_tb is
end entity;

architecture beh of cumulative_energy_map_tb is
    type mem_t is array (0 to 24) of std_logic_vector(15 downto 0);
    
    constant MEM_CONTENT: mem_t :=
                               (std_logic_vector(to_unsigned(1, 16)),
                                std_logic_vector(to_unsigned(2, 16)),
                                std_logic_vector(to_unsigned(3, 16)),
                                std_logic_vector(to_unsigned(4, 16)),
                                std_logic_vector(to_unsigned(5, 16)),
                                std_logic_vector(to_unsigned(6, 16)),
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
    
    signal colsize_s:           unsigned(11 downto 0);
    --interfejsi memorije za citanje
    signal rd_addr_o_s:         std_logic_vector(11 downto 0);
    signal rd_data_i_s:         std_logic_vector(15 downto 0);
    --Interfejsi memorije za upis
    signal wr_addr_o_s:         std_logic_vector(11 downto 0);
    signal wr_data_o_s:         std_logic_vector(15 downto 0);

    signal wr_addr_o_tb_s:         std_logic_vector(11 downto 0);
    signal wr_data_o_tb_s:         std_logic_vector(15 downto 0);


    signal dia_s:         std_logic_vector(15 downto 0);
    signal addra_s:         std_logic_vector(11 downto 0);
    
    signal wr_o_s:              std_logic;
    signal wr_o_tb_s:           std_logic;

    signal wea_s:               std_logic;

    -- signal ena_s:               std_logic;
    -- signal enb_s:               std_logic;
    
    signal start_s:             std_logic := '0';
    signal hard_toggle_row_s:   std_logic := '0';
    signal ready_s:             std_logic;

begin
    -- before memory's interface
    wea_s <= wr_o_s or wr_o_tb_s;
    dia_s <= wr_data_o_s or wr_data_o_tb_s;
    addra_s <= wr_addr_o_s or wr_addr_o_tb_s;

    clk_gen: process
    begin
        clk_s <= '0', '1' after 15 ns;
        wait for 30 ns;
    end process;
    
    stim_gen: process
    begin
        reset_s <= '1';
        colsize_s <= (to_unsigned(5, 12));
        hard_toggle_row_s <= '1';
        wait for 500 ns;
        reset_s <= '0';
        wait until falling_edge(clk_s);
        
        wr_o_tb_s <= '0';
        wait until falling_edge(clk_s);

        --punjenje memorije
        wr_o_tb_s <= '1';
        wait until falling_edge(clk_s);
        for i in 0 to 4 loop
            for j in 0 to 4 loop
                wr_addr_o_tb_s <= std_logic_vector(to_unsigned(i*5 + j, 12));
                wr_data_o_tb_s <= MEM_CONTENT(i*5 + j);
                wait until falling_edge(clk_s);
            end loop;
        end loop;
        wr_o_tb_s <= '0';
        wr_addr_o_tb_s <= (others => '0');
        wr_data_o_tb_s <= (others => '0');
        wait until falling_edge(clk_s);
        --pocetak rada algoritma
        
        start_s <= '1';
        wait until falling_edge(clk_s);
        wait until falling_edge(clk_s);
        wait until falling_edge(clk_s);
        start_s <= '0';
        
        wait until ready_s = '1';
        
        --zavrsetak stimulus generatora
        wait; 
    end process;
    
    memorija: entity work.memory
        port map(
                clk   => clk_s,
                --reset => reset_s,
                addra => addra_s,
                addrb => rd_addr_o_s,
                dia   => dia_s,
                dob   => rd_data_i_s, 
                wea   => wea_s
                -- ena   => ena_s,
                -- enb   => enb_s
                );
        
    hard: entity work.cumulative_energy_map
        port map(
                clk             => clk_s,
                reset           => reset_s,
                rd_addr_o       => rd_addr_o_s,
                rd_data_i       => rd_data_i_s,
                wr_addr_o       => wr_addr_o_s,    
                wr_data_o       => wr_data_o_s,   
                wr_o            => wr_o_s,
                colsize         => colsize_s, 
                start           => start_s,
                hard_toggle_row => hard_toggle_row_s,
                ready           => ready_s);
end beh;
