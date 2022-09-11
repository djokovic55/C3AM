library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity seam_carving_v1_0 is
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
end seam_carving_v1_0;

architecture arch_imp of seam_carving_v1_0 is

	-- component declaration
	component seam_carving_v1_0_S00_AXI is
		generic (
		DATA_WIDTH : integer := 16;
		ADDR_WIDTH : integer := 12;
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 4
		);
		port (

		-- Users to add ports here
		reg_data_o : out STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
		col_wr_o : out STD_LOGIC;
		row_wr_o : out STD_LOGIC;
		cmd_wr_o : out STD_LOGIC;

		colsize_axis_i : in STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
		rowsize_axis_i : in STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0);
		start_axis_i : in STD_LOGIC;
		status_axis_i : in STD_LOGIC;

		S_AXI_ACLK	: in std_logic;
		S_AXI_ARESETN	: in std_logic;
		S_AXI_AWADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_AWPROT	: in std_logic_vector(2 downto 0);
		S_AXI_AWVALID	: in std_logic;
		S_AXI_AWREADY	: out std_logic;
		S_AXI_WDATA	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_WSTRB	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		S_AXI_WVALID	: in std_logic;
		S_AXI_WREADY	: out std_logic;
		S_AXI_BRESP	: out std_logic_vector(1 downto 0);
		S_AXI_BVALID	: out std_logic;
		S_AXI_BREADY	: in std_logic;
		S_AXI_ARADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_ARPROT	: in std_logic_vector(2 downto 0);
		S_AXI_ARVALID	: in std_logic;
		S_AXI_ARREADY	: out std_logic;
		S_AXI_RDATA	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_RRESP	: out std_logic_vector(1 downto 0);
		S_AXI_RVALID	: out std_logic;
		S_AXI_RREADY	: in std_logic
		);
	end component seam_carving_v1_0_S00_AXI;
	
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

--------------------------------------------------------------------------------
	-- Signal declarations

	-- Common signals

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
	signal reg_data_s:     std_logic_vector(11 downto 0);
	signal col_wr_s:       std_logic;
	signal row_wr_s:       std_logic;
	signal cmd_wr_s:       std_logic;

	-- Status signals from axis to axil
	
	signal status_axis_s : STD_LOGIC;


begin

-- Instantiation of Axi Bus Interface S00_AXI
seam_carving_v1_0_S00_AXI_inst : seam_carving_v1_0_S00_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
		reg_data_o => reg_data_s,
		col_wr_o => col_wr_s,
		row_wr_o => row_wr_s,
		cmd_wr_o => cmd_wr_s,

		colsize_axis_i => colsize_s,
		rowsize_axis_i => rowsize_s,
		start_axis_i => axis_start_s,
		status_axis_i => status_axis_s,

		S_AXI_ACLK	=> s00_axi_aclk,
		S_AXI_ARESETN	=> s00_axi_aresetn,
		S_AXI_AWADDR	=> s00_axi_awaddr,
		S_AXI_AWPROT	=> s00_axi_awprot,
		S_AXI_AWVALID	=> s00_axi_awvalid,
		S_AXI_AWREADY	=> s00_axi_awready,
		S_AXI_WDATA	=> s00_axi_wdata,
		S_AXI_WSTRB	=> s00_axi_wstrb,
		S_AXI_WVALID	=> s00_axi_wvalid,
		S_AXI_WREADY	=> s00_axi_wready,
		S_AXI_BRESP	=> s00_axi_bresp,
		S_AXI_BVALID	=> s00_axi_bvalid,
		S_AXI_BREADY	=> s00_axi_bready,
		S_AXI_ARADDR	=> s00_axi_araddr,
		S_AXI_ARPROT	=> s00_axi_arprot,
		S_AXI_ARVALID	=> s00_axi_arvalid,
		S_AXI_ARREADY	=> s00_axi_arready,
		S_AXI_RDATA	=> s00_axi_rdata,
		S_AXI_RRESP	=> s00_axi_rresp,
		S_AXI_RVALID	=> s00_axi_rvalid,
		S_AXI_RREADY	=> s00_axi_rready
	);

	-- Add user logic here
  -- BUG !!!
  reset_s <= not s00_axi_aresetn;
  
    hard: cumulative_energy_map
        generic map(DATA_WIDTH => 16, ADDR_WIDTH => 12)
        port map(
                clk          => s00_axi_aclk,
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
        axis_clk        => s00_axi_aclk,
        axis_reset      => reset_s,
        axis_start      => axis_start_s,
        axis_done      => axis_done_s,
        colsize         => colsize_s,
        rowsize         => rowsize_s,
        m_axis_data     => m_axis_data_o,
        m_axis_valid    => m_axis_valid_o,
        m_axis_ready    => m_axis_ready_i,
        m_axis_last     => m_axis_last_o,
        s_axis_data     => s_axis_data_i,
        s_axis_valid    => s_axis_valid_i,
        s_axis_ready    => s_axis_ready_o,
        s_axis_last     => s_axis_last_i,
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
        clk              => s00_axi_aclk,
        reset            => reset_s,
        reg_data_i       => reg_data_s,
        col_wr_i         => col_wr_s,
        row_wr_i         => row_wr_s,
        cmd_wr_i         => cmd_wr_s,
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
	-- User logic ends

end arch_imp;
