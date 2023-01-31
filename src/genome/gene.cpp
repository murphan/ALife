//
// Created by Rosa on 1/9/2023.
//

#include "gene.h"

auto Gene::write(Genome & genome) -> void {
	genome.write(headerBase());
	genome.write(headerBase());

	writeBody(genome);
}

/* encode / decode tables */

struct { Genome::Base a; Genome::Base b; } decoded2Table[4] {
		{ Genome::A, Genome::B }, //0
		{ Genome::C, Genome::D }, //1
};

struct { Genome::Base a; Genome::Base b; } decoded3Table[4] {
		{ Genome::A, Genome::C }, //0
		{ Genome::A, Genome::D }, //1
		{ Genome::D, Genome::B }, //2
};

struct { Genome::Base a; Genome::Base b; } decoded4Table[4] {
		{ Genome::A, Genome::B }, //0
		{ Genome::B, Genome::B }, //1
		{ Genome::C, Genome::C }, //2
		{ Genome::D, Genome::A }, //3
};

struct { Genome::Base a; Genome::Base b; } decoded5Table[5] {
		{ Genome::B, Genome::A }, //0
		{ Genome::B, Genome::B }, //1
		{ Genome::D, Genome::A }, //2
		{ Genome::B, Genome::D }, //3
		{ Genome::D, Genome::D }, //4
};

struct { Genome::Base a; Genome::Base b; Genome::Base c; } decoded16Table[16] {
		{ Genome::A, Genome::A, Genome::A }, //1
		{ Genome::D, Genome::D, Genome::A }, //2
		{ Genome::D, Genome::A, Genome::B }, //3
		{ Genome::A, Genome::D, Genome::C }, //4
		{ Genome::B, Genome::B, Genome::B }, //5
		{ Genome::D, Genome::B, Genome::A }, //6
		{ Genome::B, Genome::C, Genome::B }, //7
		{ Genome::A, Genome::C, Genome::A }, //8
		{ Genome::C, Genome::C, Genome::C }, //9
		{ Genome::B, Genome::A, Genome::C }, //10
		{ Genome::C, Genome::C, Genome::D }, //11
		{ Genome::B, Genome::A, Genome::D }, //12
		{ Genome::A, Genome::B, Genome::C }, //13
		{ Genome::A, Genome::C, Genome::C }, //14
		{ Genome::B, Genome::A, Genome::A }, //15
		{ Genome::B, Genome::D, Genome::A }, //16
};

i32 encoded5Table[16] {
		/* AA */ 0,
		/* AB */ 0,
		/* AC */ 1,
		/* AD */ 2,
		/* BA */ 0,
		/* BB */ 1,
		/* BC */ 2,
		/* BD */ 3,
		/* CA */ 1,
		/* CB */ 2,
		/* CC */ 3,
		/* CD */ 4,
		/* DA */ 2,
		/* DB */ 3,
		/* DC */ 4,
		/* DD */ 4,
};

i32 encoded4Table[16] {
		/* AA */ 0,
		/* AB */ 0,
		/* AC */ 1,
		/* AD */ 0,
		/* BA */ 0,
		/* BB */ 1,
		/* BC */ 1,
		/* BD */ 2,
		/* CA */ 1,
		/* CB */ 2,
		/* CC */ 2,
		/* CD */ 3,
		/* DA */ 3,
		/* DB */ 2,
		/* DC */ 3,
		/* DD */ 3,
};

/**
 * maps 2 bases to a number 0 - 1
 */
i32 encoded3Table[16] {
		/* AA */ 0,
		/* AB */ 0,
		/* AC */ 0,
		/* AD */ 1,
		/* BA */ 0,
		/* BB */ 0,
		/* BC */ 1,
		/* BD */ 1,
		/* CA */ 1,
		/* CB */ 1,
		/* CC */ 2,
		/* CD */ 2,
		/* DA */ 1,
		/* DB */ 2,
		/* DC */ 2,
		/* DD */ 2,
};

/**
 * maps 2 bases to a number 0 - 1
 */
i32 encoded2Table[16] {
		/* AA */ 0,
		/* AB */ 0,
		/* AC */ 0,
		/* AD */ 1,
		/* BA */ 0,
		/* BB */ 0,
		/* BC */ 1,
		/* BD */ 1,
		/* CA */ 0,
		/* CB */ 0,
		/* CC */ 1,
		/* CD */ 1,
		/* DA */ 0,
		/* DB */ 1,
		/* DC */ 1,
		/* DD */ 1,
};

/**
 * maps 3 bases to a number 0 - 15
 */
i32 encoded16Table[64] {
		/* AAA */ 0,
		/* AAB */ 0,
		/* AAC */ 0,
		/* AAD */ 0,
		/* ABA */ 5,
		/* ABB */ 4,
		/* ABC */ 12,
		/* ABD */ 12,
		/* ACA */ 7,
		/* ACB */ 6,
		/* ACC */ 13,
		/* ACD */ 13,
		/* ADA */ 3,
		/* ADB */ 3,
		/* ADC */ 3,
		/* ADD */ 3,
		/* BAA */ 14,
		/* BAB */ 14,
		/* BAC */ 9,
		/* BAD */ 11,
		/* BBA */ 5,
		/* BBB */ 4,
		/* BBC */ 9,
		/* BBD */ 11,
		/* BCA */ 7,
		/* BCB */ 6,
		/* BCC */ 11,
		/* BCD */ 9,
		/* BDA */ 15,
		/* BDB */ 15,
		/* BDC */ 9,
		/* BDD */ 11,
		/* CAA */ 14,
		/* CAB */ 14,
		/* CAC */ 8,
		/* CAD */ 10,
		/* CBA */ 5,
		/* CBB */ 4,
		/* CBC */ 8,
		/* CBD */ 10,
		/* CCA */ 7,
		/* CCB */ 6,
		/* CCC */ 8,
		/* CCD */ 10,
		/* CDA */ 15,
		/* CDB */ 15,
		/* CDC */ 8,
		/* CDD */ 10,
		/* DAA */ 2,
		/* DAB */ 2,
		/* DAC */ 2,
		/* DAD */ 2,
		/* DBA */ 5,
		/* DBB */ 4,
		/* DBC */ 12,
		/* DBD */ 12,
		/* DCA */ 7,
		/* DCB */ 6,
		/* DCC */ 13,
		/* DCD */ 13,
		/* DDA */ 1,
		/* DDB */ 1,
		/* DDC */ 1,
		/* DDD */ 1,
};

/* writers */

auto Gene::write2(Genome & genome, i32 value) -> void {
	auto [d0, d1] = decoded2Table[value];
	genome.write(d0);
	genome.write(d1);
}

auto Gene::write3(Genome & genome, i32 value) -> void {
	auto [d0, d1] = decoded3Table[value];
	genome.write(d0);
	genome.write(d1);
}

auto Gene::write4(Genome & genome, i32 value) -> void {
	auto [d0, d1] = decoded4Table[value];
	genome.write(d0);
	genome.write(d1);
}

auto Gene::write5(Genome & genome, i32 value) -> void {
	auto [d0, d1] = decoded5Table[value];
	genome.write(d0);
	genome.write(d1);
}

auto Gene::write16(Genome & genome, i32 value) -> void {
	auto [d0, d1, d2] = decoded16Table[value];
	genome.write(d0);
	genome.write(d1);
	genome.write(d2);
}

/* readers */

auto Gene::read2(GenomeView & gene, i32 offset) -> i32 {
	return encoded2Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto Gene::read3(GenomeView & gene, i32 offset) -> i32 {
	return encoded3Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto Gene::read4(GenomeView & gene, i32 offset) -> i32 {
	return encoded4Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto Gene::read5(GenomeView & gene, i32 offset) -> i32 {
	return encoded5Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto Gene::read16(GenomeView & gene, i32 offset) -> i32 {
	return encoded16Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}
