-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author: Martin Kovacik (xkovacm01)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation
architecture behavioral of UART_RX is
signal clk_cnt   : std_logic_vector(4 downto 0);
signal bit_cnt   : std_logic_vector(3 downto 0);
signal rx_en     : std_logic;
signal cnt_en    : std_logic;
signal dout_vald : std_logic;
begin
	FSM: entity work.UART_RX_FSM(behavioral)
		port map (
			CLK      => CLK, -- todo změnit Prvni 3 stejny 
			RST 	   => RST,
			DIN    	=> DIN,
			CLK_CNT  => clk_cnt,
			BIT_CNT  => bit_cnt,
			RX_EN    => rx_en,
			CNT_EN 	=> cnt_en,
			DOUT_VLD => dout_vald
		);
	DOUT_VLD <= dout_vald;
	process(CLK) begin
		if rising_edge(CLK) then
			if (RST = '1') then
				DOUT <= "00000000";
				clk_cnt <= "00000";
				bit_cnt <= "0000";
			end if;
			if cnt_en = '1' then
				clk_cnt <= clk_cnt + 1;
			else 
				clk_cnt <= "00000";
			end if;
			if rx_en = '1' then		
				if clk_cnt(4) = '1' then
					clk_cnt <= "00001";
					if bit_cnt = "0000" then	
						DOUT(0) <= DIN; 
					elsif bit_cnt = "0001" then
						DOUT(1) <= DIN;			
					elsif bit_cnt = "0010" then
						DOUT(2) <= DIN;		
					elsif bit_cnt = "0011" then
						DOUT(3) <= DIN;
					elsif bit_cnt = "0100" then
						DOUT(4) <= DIN;
					elsif bit_cnt = "0101" then
						DOUT(5) <= DIN;
					elsif bit_cnt = "0110" then
						DOUT(6) <= DIN;
					elsif bit_cnt = "0111" then
						DOUT(7) <= DIN;
					end if;
					bit_cnt <= bit_cnt + 1;
				end if;
			else
				bit_cnt <= "0000";
			end if;
		end if;
	end process;
end behavioral;