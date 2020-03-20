entity testmux is 
end testmux;

architecture sim of testmux is 
	component mux
		port(A, B, C : IN BIT;
			 S : OUT BIT);
	end component;
	
	signal Ea, Eb, Ec, Q : BIT;

	begin 
		m : mux port map (Ea, Eb, Ec, Q);
		Ea <= '0', '1' after 50 ms, '0' after 150 ms;
		Eb <= '0', '1' after 70 ms, '0' after 100 ms, '1' after 170 ms, '0' after 190 ms;
		Ec <= '0', '1' after 120 ms;
end sim;
