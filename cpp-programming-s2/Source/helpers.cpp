#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int>::const_iterator move_pos(const vector<int>& vect, unsigned int target, char direction)
{
	/* This function wraps the functionality of the find() function for vectors
 	 * and furthermore takes a direction (r=right and l=left) to represent the movement
 	 * on the bi-directional vector. Its purpose is to handle the transition from
 	 * the beginning of the vector to the last useful content. */

	if (direction != 'r' && direction != 'l') {
		cout << "Error mov_pos(): invalid direction:" << (direction=='l');
		exit(1);
	}

	vector<int>::const_iterator iter;

	iter = find(vect.begin(), vect.end(), target);

	if (iter == (vect.end() - 1) && direction == 'r') { // In case the direction is positive (to the right)
		iter = vect.begin(); 

	} else if (iter == vect.begin() && direction == 'l'){ // In case the direction is negative (to the left)
		iter = vect.end() - 1;

	} else if (direction == 'r'){
		iter ++; 

	} else if (direction == 'l'){
		iter --;

	}

	return iter;

}
