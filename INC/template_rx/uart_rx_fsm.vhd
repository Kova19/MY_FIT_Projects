-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author: Martin Kovacik (xkovacm01)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port( 
        CLK         :   in std_logic;
        RST         :   in std_logic;
        DIN         :   in std_logic;
        -- Adding some more porst for program :) 
        CLK_CNT     :   in std_logic_vector(4 downto 0); -- Here I need counter for 16 bits
        BIT_CNT     :   in std_logic_vector(3 downto 0); -- Count loaded for bits
        -- output ports 
        RX_EN       :   out std_logic; -- after IDLE enable read
        CNT_EN      :   out std_logic; -- after IDLE bit, enable count
        DOUT_VLD    :   out std_logic -- data out is valid <3 
    );
end entity;



architecture behavioral of UART_RX_FSM is
    type AUTOMAT_STATES is (
        IDLE, -- starting state (contant signal 1)
        START_BIT, -- Starting bit always 0 to end IDLE
        RECEIVE_DATA, -- 
        STOP_BIT, 
        VALIDATE
    );
    signal state : AUTOMAT_STATES := IDLE;
begin
    process (CLK) begin
        if rising_edge(CLK) then
            if RST = '1' then
                state <= IDLE; 
            else
                case state is
                    when IDLE => 
                        if DIN = '0' then -- on DIN is first bit aka 0
                            state <= START_BIT;
                        end if;
                        RX_EN <= '0';
                        CNT_EN <= '0';
                        DOUT_VLD <= '0';
                    when START_BIT =>     
                        if CLK_CNT = "10000" then
                            state <= RECEIVE_DATA;
                        end if;
                        RX_EN <= '0';
                        CNT_EN <= '1';
                        DOUT_VLD <= '0';
                    when RECEIVE_DATA => 
                        if BIT_CNT = "1000" then    
                            state <= STOP_BIT;
                        end if;
                        RX_EN <= '1';
                        CNT_EN <= '1';
                        DOUT_VLD <= '0';
                    when STOP_BIT =>  
                        if DIN = '1' then
                                state <= VALIDATE;
                        end if;
                        RX_EN <= '0';
                        CNT_EN <= '0';
                        DOUT_VLD <= '0';
                    when VALIDATE =>        
                        state <= IDLE;             
                        RX_EN <= '0';
                        CNT_EN <= '0';
                        DOUT_VLD <= '1';
                end case;
            end if;
        end if;
    end process;
end architecture;
