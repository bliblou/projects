library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity banc_registre is 
	port (	dest : IN STD_LOGIC_VECTOR(3 downto 0);
			src1 :  IN STD_LOGIC_VECTOR(3 downto 0);
			src2 :  IN STD_LOGIC_VECTOR(3 downto 0);
			portW: IN STD_LOGIC_VECTOR(31 downto 0);
			banc_ecr: IN BIT;
			ck : IN BIT;
			portA : OUT STD_LOGIC_VECTOR(31 downto 0);
			portB : OUT STD_LOGIC_VECTOR(31 downto 0));
end banc_registre;

architecture archi of banc_registre is
	type type_banc is array(0 to 15) of STD_LOGIC_VECTOR(31 downto 0);
	signal banc:type_banc;

	begin

		portA <= banc(to_integer(unsigned(src1)));
		portB <= banc(to_integer(unsigned(src2)));

		process(ck);
		begin
			if ck'event and ck = '0' then
				if banc_ecr = '1' then
					banc(to_integer(unsigned(dest))) <= portW;
				endif;
			endif;
		end process;
end archi;