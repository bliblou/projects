entity flipflop is 
	port (D, H : IN BIT;
		  Q : OUT BIT);
end flipflop;

architecture a1 of flipflop is 
	begin
		process(H)
			begin
				if H'event and H = '0' then
					Q<=D;
				end if;
		end process;
end a1;
