struct operation {
	long a;
	long b;
};

program CALC_PROG {
	version CALC_VERSION_1 {
		long ajouter(operation) = 1;
		long soustraire(operation) = 2;
		long multiplier(operation) = 3;
		void memoriser(int) = 4;
		long extraire(int) = 5;
		void allumer() = 6;
	} = 1;
} = 0x69696969;