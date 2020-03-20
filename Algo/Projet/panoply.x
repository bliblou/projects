struct abonnement {
	int prix;
	int credit;
};

struct client {
	char nom[64];
	char prenom[64];
	char password[16];
	char mail[256];
	abonnement abo;
};

struct vetement {
	int prix;
	char marque[32];
};

program PANO_PROG {
	version PANO_VERSION_1 {
		client nouveau_client() = 1;
		void lister_abo() = 2;
		void affecter_abo(client) = 3;
		void modifier_abo(client) = 4;
		void lister_marques() = 5;
		void recherche_vetements_marque() = 6;
		void louer_vetement(vetement) = 7;
		int connexion() = 8;
	} = 1;
} = 0x11223344;