entity testflipflop is 
end testflipflop;

architecture sim of testflipflop is 
	component flipflop 
		port (D, H : IN BIT;
			  Q : OUT BIT);
	end component;
	
	signal Ed : BIT;
	signal clock : BIT :='0';
	signal S : BIT;

	begin 
		f : flipflop port map(Ed, S);
		clock <= not clock after 50 ns;
		Ed <= '0', '1' after 12 ns, '0' after 52 ns, '1' after 89 ns, '0' after 100 ns, '1' after 189 ns;
end sim;
