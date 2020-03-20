entity mux is
	port(A, B, C : IN BIT;
		 S : OUT BIT);
end mux;

architecture arch of mux is 
	begin
		S <= A when c = '0' else B;
end arch;
