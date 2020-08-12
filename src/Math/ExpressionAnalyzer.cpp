
#include "Math/ExpressionAnalyzer.h"
#include <iostream>
#include <cstring>
#include "math.h"

int numOfPredefFuncs = 13;
predefFunc predefFunctions[] = {
	{"sin", 3, sin},
	{"cos", 3, cos},
	{"tan", 3, tan},
	{"arcsin", 6, asin},
	{"arccos", 6, acos},
	{"arctan", 6, atan},
	{"sinh", 4, sinh},
	{"cosh", 4, cosh},
	{"tanh", 4, tanh},
	{"sqrt", 4, sqrt},
	{"abs", 3, abs},
	{"log", 3, log10},
	{"ln", 2, log},
};

int numOfVars = 7;
predefVar predefVariables[] = {
	{{9,0}, 1, 0},
	{{9,0}, 1, 0},
	{{9,0}, 1, 0},
	{{9,0}, 1, 0},
	{{9,0}, 1, 0},
	{"pi", 2, 3.1415926},
	{"e", 1, 2.718281828}
};

operation operations[] = {
	{"+", 1},
	{"-", 1},
	{"*", 1},
	{"/", 1},
	{"^", 1},
};

int getStringLen( char * text ) {
	int i;
	for ( i = 0; text[i] != 0; i++ ) {}
	return i;
}

bool compareDataNull( char * data, char * source ) {
	int i;
	for ( i = 0; source[i] != 0; i++ ) {
		if ( source[i] != data[i] ) {
			return false;
		}
	}
	return true;
}

void copyString( char * dest, char * source ) {
	int i;
	for ( i = 0; source[i] != 0; i++ ) {
		dest[i] = source[i];
	}
}

void clearPredefVars() {
	int i;
	for ( i = 0; i < 5; i++ ) {
		predefVariables[i] = {{9,0}, 1, 0};
	}
}

void printFuncComps( funcComponent * comps, int len ) {
	int i;
	for ( i = 0; i < len; i++ ) {
		int opType = comps[i].compType;
		if ( opType == valueType ) {	std::cout << "val"; }
		if ( opType == predefFuncType ) { std::cout << predefFunctions[comps[i].index].name; }
		if ( opType == predefVarType ) { std::cout << predefVariables[comps[i].index].name; }
		if ( opType == operationType ) { std::cout << operations[comps[i].index].name; }
		if ( opType == openGroupType ) { std::cout << "("; }
		if ( opType == closeGroupType ) { std::cout << ")"; }
		if ( opType == processType ) { std::cout << "[ITEM " << comps[i].index << "]"; }
	}

	std::cout << std::endl;
}

void deleteMathExpression( mathExpression * elem ) {
	delete elem -> data;
	delete elem -> results;
	delete elem -> funcComps;
	delete elem;
}

void deleteProcGroup( processedGroup * procGroup ) {
	delete procGroup -> g;
	delete procGroup -> g1;
	delete procGroup -> g2;
	delete procGroup -> g3;
	delete procGroup -> g4;
	delete procGroup -> g5;

	delete procGroup -> varRefs;

	int i;
	for ( i = 0; i < procGroup -> nSubGroups; i++ ) {
		delete (procGroup -> subGroups[i]);
	}
	delete procGroup -> subGroups;

	delete procGroup;
}

processedGroup * processGroup( funcComponent * group, int len, double * values ) {
	processedGroup * groupInfo = new processedGroup;

	//printFuncComps( group, len );

	int dataIndex = 0;

	bool inSubGroup = false;
	int groupDepth = 0;
	int subGroupStart;

	funcComponent * newGroup0 = new funcComponent[len];
	int len2 = 0;
	int nSubGroups = 0;

	int i;
	for ( i = 0; i < len; i++ ) {
		int type = group[i].compType;
		if ( type == openGroupType ) {
			if (groupDepth == 0) {
				subGroupStart = i;
				inSubGroup = true;

				newGroup0[len2].compType = processType;
				newGroup0[len2].index = dataIndex;
				dataIndex++;
				len2++;
				nSubGroups++;
			}
			groupDepth++;
		}
		else if ( type == closeGroupType ) {
			groupDepth--;
			if ( groupDepth == 0 ) {
				inSubGroup = false;
			}
		}
		else if ( groupDepth == 0 ) {
			newGroup0[len2].compType = group[i].compType;
			newGroup0[len2].index = group[i].index;
			len2++;
		}
	}

	//printFuncComps( newGroup0, len2 );
	int valBase = 2 * nSubGroups + 1;
	funcComponent * newGroup = new funcComponent[len];

	int nVals = 0;
	for ( i = 0; i < len2; i++ ) {
		int type = newGroup0[i].compType;
		if ( type == valueType ) {
			newGroup[i].compType = processType;
			newGroup[i].index = dataIndex;
			dataIndex++;
			nVals++;
		}
		else {
			newGroup[i].compType = newGroup0[i].compType;
			newGroup[i].index = newGroup0[i].index;	
		}
	}

	//printFuncComps( newGroup, len2 );
	int varBase = 2 + 2 * nSubGroups;

	//int nVars = 0;
	int * varRefs = new int[2 * numOfVars];

	int nVars = 0;
	for ( i = 0; i < len2; i++ ) {
		int type = newGroup[i].compType;
		if ( type == predefVarType ) {
			int varIndex = newGroup[i].index;
			bool foundVar = false;

			int j = 0;
			for ( j = 0; j < nVars; j++ ) {
				if ( varRefs[2 * j] == varIndex ) {
					newGroup[i].compType = processType;
					newGroup[i].index = varRefs[ 2 * j + 1 ];

					foundVar = true;
					break;
				}
			}
			if (!foundVar) {
				varRefs[2 * nVars] = varIndex;
				varRefs[2 * nVars + 1] = dataIndex;
				newGroup[i].compType = processType;
				newGroup[i].index = dataIndex;
				nVars++;
				dataIndex++;
			}
		}
	}

	//printFuncComps( newGroup, len2 );

	int funcBase = varBase + nVars + 1;
	int nFuncs = 0;
	int len3 = 0;
	
	funcComponent * newGroup2 = new funcComponent[len2];
	for ( i = 0; i < len2; i++ ) {
		int type = newGroup[i].compType;
		if ( type == predefFuncType ) {
			newGroup2[len3].compType = processType;
			newGroup2[len3].index = dataIndex;
			len3++;
			i++;
			dataIndex++;
			nFuncs++;
		}
		else {
			newGroup2[len3].compType = newGroup[i].compType;
			newGroup2[len3].index = newGroup[i].index;
			len3++;
		}
	}

	//printFuncComps( newGroup2, len3 );

	int powBase = funcBase + 2 * nFuncs + 1;
	int nPows = 0;
	int base3 = len3 - 1;
	funcComponent * newGroup3 = new funcComponent[len3];
	int powDIBase = dataIndex;

	for ( i = len3 - 1; i >= 0; i-- ) {
		int type = newGroup2[i].compType;
		if ( type == operationType && newGroup2[i].index == 4 ) {
			newGroup3[base3 + 1].compType = processType;
			newGroup3[base3 + 1].index = dataIndex;
			i--;
			nPows++;
			dataIndex++;
		}
		else {
			newGroup3[base3].compType = newGroup2[i].compType;
			newGroup3[base3].index = newGroup2[i].index;
			base3--;
		}
	}

	//printFuncComps( newGroup3 + base3 + 1, len3 - base3 - 1 );

	int prodBase = powBase + 2 * nPows + 1;
	int nProds = 0;
	int len4 = 0;
	funcComponent * newGroup4 = new funcComponent[len3];

	int sumBase = prodBase + 1;

	bool inProduct = false;
	for ( i = base3 + 1; i < len3; i++ ) {
		int type = newGroup3[i].compType;
		if ( type == operationType && ( newGroup3[i].index == 2 || newGroup3[i].index == 3 ) ) {
			if ( inProduct ) {
				sumBase += 2;
				i++;
			}
			else {
				len4--;
				newGroup4[len4].compType = processType;
				newGroup4[len4].index = dataIndex;
				len4++;
				i++;

				inProduct = true;
				nProds++;
				dataIndex += 1;			// first
				sumBase += 5;
			}
		}
		else {
			inProduct = false;
			newGroup4[len4].compType = newGroup3[i].compType;
			newGroup4[len4].index = newGroup3[i].index;
			len4++;
		}
	}

	//printFuncComps( newGroup4, len4 );

	int finalBase = sumBase + 1;
	int nSums = 0;
	int len5 = 0;
	funcComponent * newGroup5 = new funcComponent[len4];

	bool inSum = false;
	for ( i = 0; i < len4; i++ ) {
		int type = newGroup4[i].compType;
		if ( type == operationType && ( newGroup4[i].index == 0 || newGroup4[i].index == 1 ) ) {
			if ( inSum ) {
				finalBase += 2;
				i++;
				nSums++;
			}
			else {
				len5--;
				newGroup5[len5].compType = processType;
				newGroup5[len5].index = dataIndex;
				len5++;
				i++;

				inSum = true;
				nSums += 2;
				dataIndex += 1;
				finalBase += 4;
			}
		}
		else {
			inSum = false;
			newGroup5[len5].compType = newGroup4[i].compType;
			newGroup5[len5].index = newGroup4[i].index;
			len5++;
		}
	}

	int totalLen = finalBase;
	int totalDataLen = dataIndex;

	processedGroup ** subGroups = new processedGroup*[nSubGroups];

	groupDepth = 0;
	nSubGroups = 0;
	for ( i = 0; i < len; i++ ) {
		int type = group[i].compType;
		if ( type == openGroupType ) {
			if (groupDepth == 0) {
				subGroupStart = i;
				inSubGroup = true;
				nSubGroups++;
			}
			groupDepth++;
		}
		else if ( type == closeGroupType ) {
			groupDepth--;
			if ( groupDepth == 0 ) {
				subGroups[ nSubGroups - 1 ] = processGroup( group + subGroupStart + 1, i - subGroupStart - 1, values );
				totalLen += subGroups[ nSubGroups - 1 ] -> totalLen;
				totalDataLen += subGroups[ nSubGroups - 1 ] -> totalDataLen;
			}
		}
	} 

	groupInfo -> valBase = valBase;
	groupInfo -> varBase = varBase;
	groupInfo -> funcBase = funcBase;
	groupInfo -> powBase = powBase;
	groupInfo -> prodBase = prodBase;
	groupInfo -> sumBase = sumBase;
	groupInfo -> finalBase = finalBase;

	groupInfo -> nSubGroups = nSubGroups;
	groupInfo -> nVals = nVals;
	groupInfo -> nVars = nVars;
	groupInfo -> nFuncs = nFuncs;
	groupInfo -> nPows = nPows;
	groupInfo -> nProds = nProds;
	groupInfo -> nSums = nSums;

	groupInfo -> g = newGroup0;
	groupInfo -> g1 = newGroup;
	groupInfo -> g2 = newGroup2;
	groupInfo -> g3 = newGroup3;
	groupInfo -> g4 = newGroup4;
	groupInfo -> g5 = newGroup5;

	groupInfo -> len2 = len2;
	groupInfo -> len3 = len3;
	groupInfo -> len4 = len4;
	groupInfo -> len5 = len5;

	groupInfo -> base3 = base3;
	groupInfo -> powDIBase = powDIBase;

	groupInfo -> varRefs = varRefs;
	groupInfo -> subGroups = subGroups;
	groupInfo -> totalLen = totalLen;

	groupInfo -> dataIndex = dataIndex;
	groupInfo -> totalDataLen = totalDataLen;

	return groupInfo;
}

void makeEvaluatableGroup( processedGroup * group, double * values, int * data, double * results ) {
	int i;
	int nSubGroups = group -> nSubGroups;
	processedGroup ** subGroups = group -> subGroups;

	data[0] = nSubGroups;
	int totalLen = group -> totalLen;
	int subGroupIndex = (group -> finalBase);
	int subGroupDataIndex = group -> dataIndex;
	for ( i = 0; i < nSubGroups; i++ ) {
		makeEvaluatableGroup( subGroups[i], values, data + subGroupIndex, results + subGroupDataIndex );
		data[ 1 + 2 * i ] = subGroupIndex;
		data[ 2 + 2 * i ] = subGroupDataIndex;
		subGroupIndex += ( subGroups[i] -> totalLen);
		subGroupDataIndex += subGroups[i] -> totalDataLen;
	}

	int valBase = group -> valBase;
	data[ valBase ] = group -> nVals;
	int len2 = group -> len2;
	funcComponent * group0 = group -> g;
	int nVals = 0;
	for ( i = 0; i < len2; i++ ) {
		int type = group0[i].compType;
		if ( type == valueType ) {
			results[ nSubGroups + nVals ] = values[ group0[i].index ];
			nVals++;
		}
	}

	int varBase = group -> varBase;
	int nVars = group -> nVars;
	data[ varBase ] = nVars;
	int * varRefs = group -> varRefs;
	for ( i = 0; i < nVars; i++ ) {
		data[ varBase + i + 1 ] = varRefs[ 2 * i ]; //copy the index to access from predef vars
	}

	int funcBase = group -> funcBase;
	int nFuncs = group -> nFuncs;
	data[ funcBase ] = nFuncs;
	funcComponent * group1 = group -> g1;
	nFuncs = 0;
	for ( i = 0; i < len2; i++ ) {
		int type = group1[i].compType;
		if ( type == predefFuncType ) {
			data[ funcBase + 2 * nFuncs + 1 ] = group1[i].index;
			data[ funcBase + 2 * nFuncs + 2 ] = group1[i + 1].index;
			nFuncs++;
			i++;
		}
	}

	int powBase = group -> powBase;
	int nPows = group -> nPows;
	data[ powBase ] = nPows;
	nPows = 0;
	int len3 = group -> len3;
	int base3 = group -> base3;
	funcComponent * group2 = group -> g2;
	int powDIBase = group -> powDIBase;

	bool lastWasPower = false;
	for ( i = len3 - 1; i >= 0; i-- ) {
		int type = group2[i].compType;
		if ( type == operationType && group2[i].index == 4 ) {
			data[ powBase + 2 * nPows + 1 ] = group2[i - 1].index;
			if ( lastWasPower ) {
				data[ powBase + 2 * nPows + 2 ] = powDIBase + nPows - 1;
			}
			else {
				data[ powBase + 2 * nPows + 2 ] = group2[i + 1].index;
			}
			i--;
			nPows++;
			lastWasPower = true;
		}
		else {
			lastWasPower = false;
		}
	}

	int prodBase = group -> prodBase;
	int nProds = group -> nProds;
	data[ prodBase ] = nProds;
	funcComponent * group3 = group -> g3;

	bool inProduct = false;
	int subProdBase = prodBase + 1;
	int prodLen = 0;
	for ( i = base3 + 1; i < len3; i++ ) {
		int type = group3[i].compType;
		if ( type == operationType && ( group3[i].index == 2 || group3[i].index == 3 ) ) {
			if ( inProduct ) {
				data[ subProdBase + 2 * prodLen + 1 ] = group3[i + 1].index;
				if ( group3[i].index == 2 ) {
					data[ subProdBase + 2 * prodLen + 2 ] = 1;
				}
				else if ( group3[i].index == 3 ) {
					data[ subProdBase + 2 * prodLen + 2 ] = 0;
				}
				prodLen++;
				i++;
			}
			else {
				data[ subProdBase + 1 ] = group3[i - 1].index;
				data[ subProdBase + 2 ] = 1;
				data[ subProdBase + 3 ] = group3[i + 1].index;
				if ( group3[i].index == 2 ) {
					data[ subProdBase + 4 ] = 1;
				}
				else if ( group3[i].index == 3 ) {
					data[ subProdBase + 4 ] = 0;
				}
				prodLen += 2;
				inProduct = true;
				i++;
			}
		}
		else if (inProduct) {
			inProduct = false;
			data[ subProdBase ] = prodLen;
			subProdBase += 2 * prodLen + 1;
			prodLen = 0;
		}
	}
	if ( inProduct ) {
		data[ subProdBase ] = prodLen;
	}

	int sumBase = group -> sumBase;
	int nSums = group -> nSums;
	data[ sumBase ] = nSums;
	int len4 = group -> len4;
	funcComponent * group4 = group -> g4;

	bool inSum = false;
	int sumIndex = 0;
	if ( len4 > 1 ) {
	for ( i = 0; i < len4; i += 2 ) {
		data[ sumBase + 2 * sumIndex + 1] = group4[i].index;
		if ( i == 0 ) {
			data[ sumBase + 2 * sumIndex + 2 ] = 1;
		}
		else if ( group4[i-1].index == 0 ) {
			data[ sumBase + 2 * sumIndex + 2 ] = 1;			
		}
		else if ( group4[i-1].index == 1 ) {
			data[ sumBase + 2 * sumIndex + 2 ] = 0;		
		}
		sumIndex++;
	}
	}
}

double evaluateGroup( int * group, double * results ) {
	int resultIndex = 0;

	int nSubGroups = group[0];
	int i;
	for ( i = 0; i < nSubGroups; i++ ) {
		results[resultIndex] = evaluateGroup( group + group[2 * i + 1], results + group[2 * i + 2]);
		resultIndex++;
	}

	int nVals = group[ 1 + nSubGroups * 2 ];
	for ( i = 0; i < nVals; i++ ) {
		resultIndex++;
	}

	int varBase = 2 + nSubGroups * 2;
	int nVars = group[ varBase ];
	for ( i = 0; i < nVars; i++ ) {
		results[resultIndex] = predefVariables[ group[varBase + i + 1] ].val;
		resultIndex++;
	}

	int funcBase = varBase + nVars + 1;
	int nFuncs = group[ funcBase ];
	for ( i = 0; i < nFuncs; i++ ) {
		results[resultIndex] = predefFunctions[ group[funcBase + 2 * i + 1] ].func( results[ group[funcBase + 2 * i + 2] ] );
		resultIndex++;
	}

	int powBase = funcBase + 2 * nFuncs + 1;
	int nPows = group[ powBase ];
	for ( i = 0; i < nPows; i++ ) {
		results[resultIndex] = pow( results[ group[powBase + 2 * i + 1] ], results[ group[powBase + 2 * i + 2] ] );
		resultIndex++;
	}

	int prodBase = powBase + 2 * nPows + 1;
	int nProds = group[ prodBase ];
	int elemBase = 1;
	int nElems;
	for ( i = 0; i < nProds; i++ ) {
		double prod = 1;

		int nElems = group[ prodBase + elemBase ];
		int j;
		for ( j = 0; j < nElems; j++ ) {
			if ( group[ prodBase + elemBase + 2 * j + 2 ] ) {
				prod *= results[ group[prodBase + elemBase + 2 * j + 1 ] ];
			}
			else {
				prod /= results[ group[prodBase + elemBase + 2 * j + 1 ] ];
			}
		}

		results[resultIndex] = prod;
		resultIndex++;

		elemBase += 2 * nElems + 1;
	}

	int sumBase = prodBase + elemBase;
	int nSums = group[ sumBase ];
	double sum = 0;
	for ( i = 0; i < nSums; i++ ) {
		if ( group[ sumBase + 2 * i + 2 ] == 1 ) {
			sum += results[ group[ sumBase + 2 * i + 1 ] ];
		}
		else {
			sum -= results[ group[ sumBase + 2 * i + 1 ] ];
		}
	}
	if ( nSums > 0 ) {
		results[ resultIndex ] = sum;
		resultIndex++;
	}

	return results[resultIndex - 1];
}

double evalMathExpression( mathExpression * function ) {
	return evaluateGroup( function -> data, function -> results );
}

mathExpression * analyzeMathExpression( char * text, int len ) {
	int lastItem = openGroupType;

	bool ERROR = false;

	int i;
	for ( i = 0; i < len; i++ ) {
		if ( text[i] == 0x20 ) {
			memcpy( text + i, text + i + 1, len - i - 1 );
			len--;
		}
	}

	int valueIndex = 1;
	double values[ 128 ];
	values[0] = 0;

	int funcCompIndex = 0;
	funcComponent * funcComps = new funcComponent[ 2*len ];

	int groupDepth = 0;

	for ( i = 0; i < len; i++ ) {
		char c = text[i];
		if ( c == 44 && groupDepth == 0 ) { break; }
		if ( (c >= 0x30 && c < 0x3A) || c == 46 ) {			// is a number
			char c = text[i];
			double val = 0;
			double mult = 1;
			bool reachedDecimal = false;
			while ( (c >= 0x30 && c < 0x3A) || c == 46 ) {
				if ( c == 46 ) {
					if ( reachedDecimal ) {
						ERROR = true;			// two decimals in the same number shouldn't happen
						i = len + 1;
						break;
					}
					reachedDecimal = true;
				}
				else {
					if (reachedDecimal) {
						mult *= 0.1;
					}
					else {
						val *= 10.0;
					}

					val += (c - 48) * mult;
				}
				i++;
				c = text[i];
			}
			if (ERROR) {break;}
			i--;

			if ( lastItem == predefVarType || lastItem == closeGroupType ) {
				funcComps[ funcCompIndex ] = { operationType, 2 };
				funcCompIndex++;
			}

			values[ valueIndex ] = val;
			funcComps[ funcCompIndex ] = { valueType, valueIndex };
			valueIndex++;
			funcCompIndex++;

			lastItem = valueType;
		}

		else if ( (c >= 0x41 && c < 0x5B) || (c >= 0x61 && c < 0x7B) ) {
			int j;
			bool isPredefFunc = false;
			for ( j = 0; j < numOfPredefFuncs; j++ ) {
				if (compareDataNull( text + i, predefFunctions[j].name)) {
					isPredefFunc = true;
					break;
				}
			}
			if (isPredefFunc) {
				if ( lastItem == predefVarType || lastItem == closeGroupType || lastItem == valueType ) {
					funcComps[ funcCompIndex ] = { operationType, 2 };
					funcCompIndex++;
				}

				i += predefFunctions[j].len - 1;

				if ( text[ i + 1 ] != 40 ) {
					ERROR = true;
					i = len;
					break;
				}

				funcComps[ funcCompIndex ] = { predefFuncType, j };
				funcCompIndex++;

				lastItem = predefFuncType;
			}
			else {
				bool isPredefVar = false;
				for ( j = 0; j < numOfVars; j++ ) {
					if (compareDataNull( text + i, predefVariables[j].name)) {
						isPredefVar = true;
						break;
					}
				}
				if (isPredefVar) {
					i += predefVariables[j].len - 1;

					if ( lastItem == predefVarType || lastItem == closeGroupType || lastItem == valueType ) {
						funcComps[ funcCompIndex ] = { operationType, 2 };
						funcCompIndex++;
					}

					funcComps[ funcCompIndex ] = { predefVarType, j };
					funcCompIndex++;

					lastItem = predefVarType;
				}
				else {
					//std::cout << "ERROR" << std::endl;
					ERROR = true;
					i = len;
					break;	
				}	
			}
		}
		else if ( c == 40 || c == 41 ) {
			if ( c == 40 ) {
				if ( lastItem == predefVarType || lastItem == closeGroupType || lastItem == valueType ) {
					funcComps[ funcCompIndex ] = { operationType, 2 };
					funcCompIndex++;
				}

				funcComps[ funcCompIndex ] = { openGroupType, 0 };
				funcCompIndex++;

				lastItem = openGroupType;
				groupDepth++;
			}
			if ( c == 41 ) {
				if (groupDepth == 0) { break; }
				if ( lastItem == operationType || lastItem == openGroupType ) {
					ERROR = true;
					i = len;
					break;
				}

				funcComps[ funcCompIndex ] = { closeGroupType, 0 };
				funcCompIndex++;

				lastItem = closeGroupType;
				groupDepth--;
				if ( groupDepth < 0 ) { ERROR = true; i = len; break; }
			}
		}
		else if ( c == 42 || c == 43 || c == 45 || c == 47 || c == 94 ) {	// operations +,-,*,/,^
			int j;
			bool isOp;
			for ( j = 0; j < 5; j++ ) {
				if (compareDataNull( text + i, operations[j].name)) {
					isOp = true;
					break;
				}
			}
			if (isOp) {
				i += operations[j].len - 1;

				if ( lastItem == operationType ) {
					ERROR = true;
					i = len;
					break;	
				}
				else if ( lastItem == openGroupType ) {
					if ( j == 1 ) {
						funcComps[ funcCompIndex ] = { valueType, 0 };
						funcCompIndex++;
					}
					else {
						ERROR = true;
						i = len;
						break;
					}
				}

				funcComps[ funcCompIndex ] = { operationType, j };
				funcCompIndex++;

				lastItem = operationType;
			}
		}
	}

	if (groupDepth != 0 || lastItem == operationType || len == 0) { ERROR = true; }

	if (ERROR) {
		delete funcComps;
		return NULL;
	}
	else {

	processedGroup * procGroup = processGroup( funcComps, funcCompIndex, values );

	int * groupData = new int[ procGroup -> totalLen ];
	double * groupResults = new double[ procGroup -> totalDataLen ];
	makeEvaluatableGroup( procGroup, values, groupData, groupResults );

	deleteProcGroup( procGroup );

	mathExpression * newExpression = new mathExpression;
	newExpression -> data = groupData;
	newExpression -> results = groupResults;
	newExpression -> funcComps = funcComps;
	newExpression -> funcCompLen = funcCompIndex;
	newExpression -> textLen = i;
	return newExpression;

	}
}

numberAnalysis analyzeNumber( char * text, int len ) {
	numberAnalysis num;

	mathExpression * elem = analyzeMathExpression( text, len );
	if (elem != NULL) {
		double val = evalMathExpression( elem );
		num.val = val;
		num.textLen = elem -> textLen;
		num.sucess = true;
		deleteMathExpression( elem );
	}
	else {
		num.val = 0;
		num.sucess = false;
	}
	return num;
}
