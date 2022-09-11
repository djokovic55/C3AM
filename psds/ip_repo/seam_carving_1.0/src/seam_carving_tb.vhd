----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/10/2022 09:45:35 AM
-- Design Name: 
-- Module Name: seam_carving_tb - Behavioral
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

entity seam_carving_tb is
--  Port ( );
end seam_carving_tb;

architecture Behavioral of seam_carving_tb is

    component seam_carving_v1_0 is
	generic (
		-- Users to add parameters here

		DATA_WIDTH : integer := 16;
		ADDR_WIDTH : integer := 12;
		-- User parameters ends
		-- Do not modify the parameters beyond this line

		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 4
	);
	port (
		-- Users to add ports here

		-- Ports of Axi Slave Bus Interface axi_stream_cont

		s_axis_data_i : in std_logic_vector(DATA_WIDTH-1 downto 0);
		s_axis_valid_i : in std_logic;
		s_axis_ready_o : out std_logic;
		s_axis_last_i : in std_logic;

		-- Ports of Axi Master Bus Interface axi_stream_cont

		m_axis_data_o : out std_logic_vector(DATA_WIDTH-1 downto 0);
		m_axis_valid_o : out std_logic;
		m_axis_ready_i : in std_logic;
		m_axis_last_o : out std_logic;

		-- User ports ends
		-- Do not modify the ports beyond this line

		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	: in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	: out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	: out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic
	);
    end component;
    type mem_t is array (0 to 24) of std_logic_vector(15 downto 0);

    constant ROWSIZE_c : integer := 4;
    constant COLSIZE_c : integer := 5;

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

-- Matrix multiplier core's address map
     constant COL_REG_ADDR_C : integer := 0;
     constant ROW_REG_ADDR_C : integer := 4;
     constant CMD_REG_ADDR_C : integer := 8;
     constant STATUS_REG_ADDR_C : integer := 12;

------------------- AXI Interfaces signals -------------------
 -- Parameters of Axi-Lite Slave Bus Interface S00_AXI
     constant C_S00_AXI_DATA_WIDTH_c : integer := 32;
     constant C_S00_AXI_ADDR_WIDTH_c : integer := 4;

     signal clk_s: std_logic;
     signal reset_s: std_logic;

 -- Ports of Axi-Lite Slave Bus Interface S00_AXI
    signal s00_axi_aclk_s : std_logic := '0';
    signal s00_axi_aresetn_s : std_logic := '1';
    signal s00_axi_awaddr_s : std_logic_vector(C_S00_AXI_ADDR_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_awprot_s : std_logic_vector(2 downto 0) := (others => '0');
    signal s00_axi_awvalid_s : std_logic := '0';
    signal s00_axi_awready_s : std_logic := '0';
    signal s00_axi_wdata_s : std_logic_vector(C_S00_AXI_DATA_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_wstrb_s : std_logic_vector((C_S00_AXI_DATA_WIDTH_c/8)-1 downto 0) := (others => '0');
    signal s00_axi_wvalid_s : std_logic := '0';
    signal s00_axi_wready_s : std_logic := '0';
    signal s00_axi_bresp_s : std_logic_vector(1 downto 0) := (others => '0');
    signal s00_axi_bvalid_s : std_logic := '0';
    signal s00_axi_bready_s : std_logic := '0';
    signal s00_axi_araddr_s : std_logic_vector(C_S00_AXI_ADDR_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_arprot_s : std_logic_vector(2 downto 0) := (others => '0');
    signal s00_axi_arvalid_s : std_logic := '0';
    signal s00_axi_arready_s : std_logic := '0';
    signal s00_axi_rdata_s : std_logic_vector(C_S00_AXI_DATA_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_rresp_s : std_logic_vector(1 downto 0) := (others => '0');
    signal s00_axi_rvalid_s : std_logic := '0';
    signal s00_axi_rready_s : std_logic := '0';


	-- Ports of Axi-stream master bus interface
	signal m_axis_data_s :     std_logic_vector(15 downto 0);
	signal m_axis_valid_s :    std_logic;
	signal m_axis_ready_s :    std_logic;
	signal m_axis_last_s :     std_logic;

	-- Ports of Axi-stream slave bus interface
	signal s_axis_data_s :     std_logic_vector(15 downto 0);
	signal s_axis_valid_s :    std_logic;
	signal s_axis_ready_s :    std_logic;
	signal s_axis_last_s :     std_logic;

begin

    clk_gen: process
    begin
        clk_s <= '0', '1' after 15 ns;
        wait for 30 ns;
    end process;

    driver: process

    variable axi_read_data_v : std_logic_vector(31 downto 0);

    begin
        -- reset AXI-lite interface. Reset will be 10 clock cycles wide
        s00_axi_aresetn_s <= '0';
        -- wait for 5 falling edges of AXI-lite clock signal
        for i in 1 to 5 loop
        wait until falling_edge(clk_s);
        end loop;
        -- release reset 
        s00_axi_aresetn_s <= '1';
        wait until falling_edge(clk_s);


        ----------------------------------------------------------------------
        -- Initialize the  seam carving core --
        ----------------------------------------------------------------------
        report "Loading the rowsize and colsize!";

        -- Set COLSIZE
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(COL_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '1';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(COLSIZE_c, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '1';
        s00_axi_wstrb_s <= "1111";
        s00_axi_bready_s <= '1';
        wait until s00_axi_awready_s = '1';
        wait until s00_axi_awready_s = '0';
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '0';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '0';
        s00_axi_wstrb_s <= "0000";
        wait until s00_axi_bvalid_s = '0';
        wait until falling_edge(clk_s); 
        s00_axi_bready_s <= '0';
        wait until falling_edge(clk_s); 
        -- wait for 5 falling edges of AXI-lite clock signal
        for i in 1 to 5 loop
        wait until falling_edge(clk_s);
        end loop;
        
        -- Set ROWSIZE
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(ROW_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '1';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(ROWSIZE_c, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '1';
        s00_axi_wstrb_s <= "1111";
        s00_axi_bready_s <= '1';
        wait until s00_axi_awready_s = '1';
        wait until s00_axi_awready_s = '0';
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '0';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '0';
        s00_axi_wstrb_s <= "0000";
        wait until s00_axi_bvalid_s = '0';
        wait until falling_edge(clk_s); 
        s00_axi_bready_s <= '0';
        wait until falling_edge(clk_s);
        -- wait for 5 falling edges of AXI-lite clock signal
        for i in 1 to 5 loop
        wait until falling_edge(clk_s);
        end loop;

        ----------------------------------------------------------------------
        -- Axi stream cont configuration --
        ----------------------------------------------------------------------

        -- Dma slave port should be always ready
        m_axis_ready_s <= '1';

        -- sending data from dma simulation, data is alway valid 
        wait until falling_edge(clk_s);
        s_axis_valid_s <= '1';


        -------------------------------------------------------------------------------------------
        -- Start the Seam Carving core --
        -------------------------------------------------------------------------------------------
        report "Starting the seam carving process!";
        -- Set the value start bit (bit 0 in the CMD register) to 1
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(CMD_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '1';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(1, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '1';
        s00_axi_wstrb_s <= "1111";
        s00_axi_bready_s <= '1';
        wait until s00_axi_awready_s = '1';
        wait until s00_axi_awready_s = '0';
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '0';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '0';
        s00_axi_wstrb_s <= "0000";
        wait until s00_axi_bvalid_s = '0';
        wait until falling_edge(clk_s); 
        s00_axi_bready_s <= '0';
        wait until falling_edge(clk_s); 
        -- wait for 5 falling edges of AXI-lite clock signal
        for i in 1 to 5 loop
        wait until falling_edge(clk_s);
        end loop;
        
        report "Clearing the start bit!";
        -- Set the value start bit (bit 0 in the CMD register) to 1
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(CMD_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '1';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '1';
        s00_axi_wstrb_s <= "1111";
        s00_axi_bready_s <= '1';
        wait until s00_axi_awready_s = '1';
        wait until s00_axi_awready_s = '0';
        wait until falling_edge(clk_s);
        s00_axi_awaddr_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_ADDR_WIDTH_c));
        s00_axi_awvalid_s <= '0';
        s00_axi_wdata_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_DATA_WIDTH_c));
        s00_axi_wvalid_s <= '0';
        s00_axi_wstrb_s <= "0000";
        wait until s00_axi_bvalid_s = '0';
        wait until falling_edge(clk_s); 
        s00_axi_bready_s <= '0';
        wait until falling_edge(clk_s); 
        -------------------------------------------------------------------------------------------
        -- Wait until Matrix Multiplier core finishes matrix multiplication --
        -------------------------------------------------------------------------------------------
        report "Waiting for the matric multiplication process to complete!";
        loop
            -- Read the content of the Status register
            wait until falling_edge(clk_s);
            s00_axi_araddr_s <= std_logic_vector(to_unsigned(STATUS_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c));
            s00_axi_arvalid_s <= '1';
            s00_axi_rready_s <= '1';
            wait until s00_axi_arready_s = '1';
            axi_read_data_v := s00_axi_rdata_s;
            wait until s00_axi_arready_s = '0';
            wait until falling_edge(clk_s);
            s00_axi_araddr_s <= std_logic_vector(to_unsigned(0, C_S00_AXI_ADDR_WIDTH_c));
            s00_axi_arvalid_s <= '0';
            s00_axi_rready_s <= '0';
    
            -- Check is the 1st bit of the Status register set to one
            if (axi_read_data_v(0) = '1') then
                -- Matrix multiplication process completed
                exit;
            else
                wait for 100 ns;
            end if;
        end loop;

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

    seam_carving_v1_0_inst: seam_carving_v1_0
      port map (
        s_axis_data_i   => s_axis_data_s,
        s_axis_valid_i  => s_axis_valid_s,
        s_axis_ready_o  => s_axis_ready_s,
        s_axis_last_i   => s_axis_last_s,
        m_axis_data_o   => m_axis_data_s,
        m_axis_valid_o  => m_axis_valid_s,
        m_axis_ready_i  => m_axis_ready_s,
        m_axis_last_o   => m_axis_last_s,

        s00_axi_aclk    => clk_s,
        s00_axi_aresetn => s00_axi_aresetn_s,
        s00_axi_awaddr  => s00_axi_awaddr_s,
        s00_axi_awprot  => s00_axi_awprot_s,
        s00_axi_awvalid => s00_axi_awvalid_s,
        s00_axi_awready => s00_axi_awready_s,
        s00_axi_wdata   => s00_axi_wdata_s,
        s00_axi_wstrb   => s00_axi_wstrb_s,
        s00_axi_wvalid  => s00_axi_wvalid_s,
        s00_axi_wready  => s00_axi_wready_s,
        s00_axi_bresp   => s00_axi_bresp_s,
        s00_axi_bvalid  => s00_axi_bvalid_s,
        s00_axi_bready  => s00_axi_bready_s,
        s00_axi_araddr  => s00_axi_araddr_s,
        s00_axi_arprot  => s00_axi_arprot_s,
        s00_axi_arvalid => s00_axi_arvalid_s,
        s00_axi_arready => s00_axi_arready_s,
        s00_axi_rdata   => s00_axi_rdata_s,
        s00_axi_rresp   => s00_axi_rresp_s,
        s00_axi_rvalid  => s00_axi_rvalid_s,
        s00_axi_rready  => s00_axi_rready_s
      );
end Behavioral;
