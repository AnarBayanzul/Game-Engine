#include "Engine.h"
#include "Utility.h"
#include "simplex.h"

#include <string>
#include <fstream>

#include <iostream>

SDL_KeyboardEvent lastKey;


collisionFunc collisionTable[(NUMOFTYPES * (NUMOFTYPES + 1)) / 2] = {};

void addToCollisionTable(objectType A, objectType B, collisionFunc inputFunc) {
	// Assuming A is the *lesser* objectType
	int index = (B * (B + 1)) / 2 + A;
	collisionTable[index] = inputFunc;
}
collisionFunc getFromCollisionTable(objectType A, objectType B) {
	int index = (B * (B + 1)) / 2 + A;
	return collisionTable[index];
}


size_t getDeltaTime() {
	return delta;
}

float getDeltaSeconds() {
	return deltaSec;
}

quat slerp(quat a, quat b, float t) {
	glm::vec4 result;
	glm::vec4 vA = a.getElements();
	glm::vec4 vB = b.getElements();

	float cosHalfTheta = glm::dot(vA, vB);
	if (std::abs(cosHalfTheta) >= 1.0) {
		return a;
	}
	float halfTheta = glm::acos(cosHalfTheta);
	float sinHalfTheta = glm::sqrt(1.0 - cosHalfTheta * cosHalfTheta);
	if (std::abs(sinHalfTheta) < 0.001) {
		result.x = 0.5 * vA.x + 0.5 * vB.x;
		result.y = 0.5 * vA.y + 0.5 * vB.y;
		result.z = 0.5 * vA.z + 0.5 * vB.z;
		result.w = 0.5 * vA.w + 0.5 * vB.w;
		return quat(result);
	}
	float ratioA = glm::sin((1 - t) * halfTheta) / sinHalfTheta;
	float ratioB = glm::sin(t * halfTheta) / sinHalfTheta;

	result.x = ratioA * vA.x + ratioB * vB.x;
	result.y = ratioA * vA.y + ratioB * vB.y;
	result.z = ratioA * vA.z + ratioB * vB.z;
	result.w = ratioB * vA.w + ratioB * vB.w;

	return quat(result);
}





glm::vec3 integrateLinear(float deltaTime, const glm::vec3& linear) {
	// s += t * v
	return deltaTime * linear;
}

quat integrateAngular(float deltaTime, const glm::vec3 angular) {
	// q = q' * q * q'^-

	if (glm::dot(angular, angular) == 0.0f) {
		return quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
	}
	return quat(glm::normalize(angular), glm::length(angular) * deltaTime); // not efficient? TODO
}

bool GJK(Mesh* A, quat aQ, glm::vec3 sA, Mesh* B, quat bQ, glm::vec3 sB, float& dist) {
	// arbitrary inital direction
	glm::vec3 searchDir = glm::vec3(0.0, 0.0, 1.0);
	// initial minkowski diff in simplex
	simplex sim = { 1, 0, {} };
	glm::vec3 difference = A->findSupport(searchDir, aQ, sA) - B->findSupport(-searchDir, bQ, sB);
	sim.points[0] = difference;
	searchDir = -sim.points[0];
	sim.squareShortest = glm::dot(searchDir, searchDir);


	glm::vec3 normalizedSearchDir;
	float newSquareShortest;

	glm::vec3 AO; // all cases
	glm::vec3 AB;

	glm::vec3 AC;
	glm::vec3 ABC;

	glm::vec3 AD;
	glm::vec3 ACD;
	glm::vec3 ADB;

	while (1) {
		difference = A->findSupport(searchDir, aQ, sA) - B->findSupport(-searchDir, bQ, sB);
		if (glm::dot(difference, searchDir) < 0) {
			dist = std::sqrt(sim.squareShortest);
			return false; // no collision
		}
		sim.newest = (sim.newest + 1) % 4;
		sim.points[sim.newest] = difference;
		++sim.dimensions;


		switch (sim.dimensions) { // new simplex and set new search direction
		case 1:
			// one minkowski diff in simplex
			// no simplex change
			searchDir = -sim.points[sim.newest];
			break;
		case 2:
			// find closest point to origin along line
			AO = -sim.points[sim.newest];
			AB = sim.points[(sim.newest + 3) % 4] - sim.points[sim.newest];
			if (glm::dot(AB, AO) > 0) {
				// closest point on line
				// AB X AO X AB
				searchDir = glm::cross(AB, glm::cross(AO, AB));
			} else {
				// closest point is newest
				// simplex just a
				sim.dimensions = 1;
				searchDir = AO;
			}
			break;
		case 3:
			// find part of triangle closest to origin
			AO = -sim.points[sim.newest];
			AB = sim.points[(sim.newest + 3) % 4] - sim.points[sim.newest];
			AC = sim.points[(sim.newest + 2) % 4] - sim.points[sim.newest];
			ABC = glm::cross(AB, AC);
			if (glm::dot(glm::cross(ABC, AC), AO) > 0) {
				if (glm::dot(AC, AO) > 0) {
					// simplex is [A, C]
					// search dir is AC X AO X AC
					sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 2) % 4]; // overwrite B with C
					sim.dimensions = 2;
					searchDir = glm::cross(AC, glm::cross(AO, AC));
				} else {
					// star
					if (glm::dot(AB, AO) > 0) {
						// simplex [A, B]
						// search dir is AB X AO X AB
						sim.dimensions = 2;
						searchDir = glm::cross(AB, glm::cross(AO, AB));
					} else {
						sim.dimensions = 1;
						searchDir = AO;
					}
					// endstar
				}
			} else {
				if (glm::dot(glm::cross(AB, ABC), AO) > 0) {
					// star
					if (glm::dot(AB, AO) > 0) {
						// simplex [A, B]
						// search dir is AB X AO X AB
						sim.dimensions = 2;
						searchDir = glm::cross(AB, glm::cross(AO, AB));
					}
					else {
						sim.dimensions = 1;
						searchDir = AO;
					}
					// endstar
				} else {
					if (glm::dot(ABC, AO) > 0) {
						// simplex [A, B, C]
						// search dir ABC
						searchDir = ABC;
					} else {
						// simplex [A, C, B]
						// search dir -ABC
						std::swap(sim.points[(sim.newest + 3) % 4], sim.points[(sim.newest + 2) % 4]);
						searchDir = -ABC;
					}
				}
			}
			break;
		case 4:
			AO = -sim.points[sim.newest];
			AB = sim.points[(sim.newest + 3) % 4] - sim.points[sim.newest];
			AC = sim.points[(sim.newest + 2) % 4] - sim.points[sim.newest];
			AD = sim.points[(sim.newest + 1) % 4] - sim.points[sim.newest];
			ABC = glm::cross(AB, AC);
			ACD = glm::cross(AC, AD);
			ADB = glm::cross(AD, AB);
			if (glm::dot(ABC, AO) > 0) {
				if (glm::dot(glm::cross(ABC, AC), AO) > 0) {
					if (glm::dot(AC, AO) > 0) {
						// simplex is [A, C]
						// search dir is AC X AO X AC
						sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 2) % 4]; // overwrite B with C
						sim.dimensions = 2;
						searchDir = glm::cross(AC, glm::cross(AO, AC));
					}
					else {
						// star
						if (glm::dot(AB, AO) > 0) {
							// simplex [A, B]
							// search dir is AB X AO X AB
							sim.dimensions = 2;
							searchDir = glm::cross(AB, glm::cross(AO, AB));
						}
						else {
							sim.dimensions = 1;
							searchDir = AO;
						}
						// endstar
					}
				}
				else {
					if (glm::dot(glm::cross(AB, ABC), AO) > 0) {
						// star
						if (glm::dot(AB, AO) > 0) {
							// simplex [A, B]
							// search dir is AB X AO X AB
							sim.dimensions = 2;
							searchDir = glm::cross(AB, glm::cross(AO, AB));
						}
						else {
							sim.dimensions = 1;
							searchDir = AO;
						}
						// endstar
					}
					else {
						if (glm::dot(ABC, AO) > 0) {
							// simplex [A, B, C]
							// search dir ABC
							sim.dimensions = 3;
							searchDir = ABC;
						}
						else {
							// simplex [A, C, B]
							// search dir -ABC
							std::swap(sim.points[(sim.newest + 3) % 4], sim.points[(sim.newest + 2) % 4]);
							sim.dimensions = 3;
							searchDir = -ABC;
						}
					}
				}
			} else if (glm::dot(ACD, AO) > 0) {
				if (glm::dot(glm::cross(ACD, AD), AO) > 0) {
					if (glm::dot(AD, AO) > 0) {
						// simplex is [A, D]
						// search dir is AD X AO X AD
						sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 1) % 4]; // overwrite B with C
						sim.dimensions = 2;
						searchDir = glm::cross(AD, glm::cross(AO, AD));
					}
					else {
						// star
						if (glm::dot(AC, AO) > 0) {
							// simplex [A, C]
							// search dir is AC X AO X AC
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 2) % 2];
							sim.dimensions = 2;
							searchDir = glm::cross(AC, glm::cross(AO, AC));
						}
						else {
							sim.dimensions = 1;
							searchDir = AO;
						}
						// endstar
					}
				}
				else {
					if (glm::dot(glm::cross(AC, ACD), AO) > 0) {
						// star
						if (glm::dot(AC, AO) > 0) {
							// simplex [A, C]
							// search dir is AC X AO X AC
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 2) % 2];
							sim.dimensions = 2;
							searchDir = glm::cross(AC, glm::cross(AO, AC));
						}
						else {
							sim.dimensions = 1;
							searchDir = AO;
						}
						// endstar
					}
					else {
						if (glm::dot(ACD, AO) > 0) {
							// simplex [A, C, D]
							// search dir ACD
							sim.dimensions = 3;
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 2) % 4];
							sim.points[(sim.newest + 2) % 4] = sim.points[(sim.newest + 1) % 4];
							searchDir = ACD;
						}
						else {
							// simplex [A, D, C]
							// search dir -ACD
							sim.dimensions = 3;
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 1) % 4];
							searchDir = -ACD;
						}
					}
				}
			} else if (glm::dot(ADB, AO) > 0) {
				if (glm::dot(glm::cross(ADB, AB), AO) > 0) {
					if (glm::dot(AB, AO) > 0) {
						// simplex is [A, B]
						// search dir is AB X AO X AB
						sim.dimensions = 2;
						searchDir = glm::cross(AB, glm::cross(AO, AB));
					}
					else {
						// star
						if (glm::dot(AD, AO) > 0) {
							// simplex [A, D]
							// search dir is AD X AO X AD
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 1) % 4];
							sim.dimensions = 2;
							searchDir = glm::cross(AD, glm::cross(AO, AD));
						}
						else {
							sim.dimensions = 1;
							searchDir = AO;
						}
						// endstar
					}
				}
				else {
					if (glm::dot(glm::cross(AD, ADB), AO) > 0) {
						// star
						if (glm::dot(AD, AO) > 0) {
							// simplex [A, D]
							// search dir is AD X AO X AD
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 1) % 4];
							sim.dimensions = 2;
							searchDir = glm::cross(AD, glm::cross(AO, AD));
						}
						else {
							sim.dimensions = 1;
							searchDir = AO;
						}
						// endstar
					}
					else {
						if (glm::dot(ADB, AO) > 0) {
							// simplex [A, D, B]
							// search dir ADB
							sim.dimensions = 3;
							sim.points[(sim.newest + 2) % 4] = sim.points[(sim.newest + 3) % 4];
							sim.points[(sim.newest + 3) % 4] = sim.points[(sim.newest + 1) % 4];
							searchDir = ADB;
						}
						else {
							// simplex [A, B, D]
							// search dir -ADB
							sim.dimensions = 3;
							sim.points[(sim.newest + 2) % 4] = sim.points[(sim.newest + 1) % 4];
							searchDir = -ADB;
						}
					}
				}
			} else {
				dist = std::sqrt(sim.squareShortest);
				return true;
			}
			break;
		}
		normalizedSearchDir = glm::normalize(searchDir);
		newSquareShortest = (float)std::pow(glm::dot(AO, normalizedSearchDir), 2);
		if (newSquareShortest > sim.squareShortest) {
			dist = std::sqrt(sim.squareShortest);
			return false; // converged on closest
		} else {
			sim.squareShortest = newSquareShortest;
		}
	}
	dist = std::sqrt(sim.squareShortest);
	return true;
}