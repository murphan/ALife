//
// Created by Rosa on 1/9/2023.
//

export module Gene;

import Genome;

export class Gene {
protected:
	/**
	 * each type of gene should implement this
	 * writes everything that's not the gene header
	 * do not call this directly, instead use write();
	 */
	virtual auto writeBody(Genome & genome) const -> void = 0;

public:
	enum Type {
		BODY,
		EYE,
		UPGRADE,
		MUTATION,
		MOVEMENT,
	};
};
